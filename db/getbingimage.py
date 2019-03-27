#!/usr/bin/python3
import urllib.request
import re
import sys
import os
import threading
import time
import getopt
import logging
import daemon
import json
import sqlite3 as lite


def createdb(config):
    con = lite.connect(config['db-file'])
    cur = con.cursor()
    cur.execute(
        'CREATE TABLE imgs(startdate INTEGER PRIMARY KEY,copyright TEXT, url TEXT)')
    print('create db %s.' % config['db-file'])
    con.commit()
    cur.close()
    con.close()


def getbingbgimg(config):
    json_response = download(
        'https://www.bing.com/HPImageArchive.aspx?format=js&idx=0&n=1', config['retry-time'])
    if not json_response:
        print(time.ctime(), 'Error! Can not get api response!')
        return
    j = json.loads(json_response.decode('utf-8'))

    startdate = j['images'][0]['startdate']
    copyright = j['images'][0]['copyright']
    url = 'http://www.bing.com' + j['images'][0]['url']
    img = download(url, config['retry-time'])

    filename = startdate + '.jpg'
    filepath = os.path.join(config['img-path'], filename)

    if not img:
        print(time.ctime(), 'Error! can not get img!')
        return
    f = open(filepath, 'wb')
    f.write(img)
    f.close()

    con = lite.connect(config['db-file'])
    try:
        cur = con.cursor()
        cur.execute("INSERT INTO imgs VALUES(?,?,?)" ,
                    (startdate, copyright, url))
        con.commit()
    except Exception as e:
        print(time.ctime(), 'Cant save %s to sqlite,[ %s ]' % (
            startdate, e.args))
    else:
        print(time.ctime(), 'Success')


def download(url, num_retries):
    if num_retries <= 0:
        return None
    try:
        return urllib.request.urlopen(url).read()
    except urllib.error.URLError as e:
        print("Download error, url: %s, reason: %s" % (url, e.reason))
        return download(url, num_retries-1)


def get_config():
    shortopts = 'd:h'
    longopts = ['log-file=', 'db-file=', 'img-path=', 'pid-file=']
    try:
        optlist, args = getopt.getopt(sys.argv[1:], shortopts, longopts)
        config = {}
        for key, value in optlist:
            if key == '-d':
                config['daemon'] = value
            elif key == '-h':
                help()
                exit(0)
            elif key == '--pid-file':
                config['pid-file'] = value
            elif key == '--log-file':
                config['log-file'] = value

            elif key == '--db-file':
                config['db-file'] = value
            elif key == '--img-path':
                config['img-path'] = value

        config['daemon'] = config.get('daemon', False)
        config['pid-file'] = config.get('pid-file', '/var/run/%s.pid' %
                                        os.path.splitext(sys.argv[0])[0])
        config['log-file'] = config.get('log-file', '/var/log/%s.log' %
                                        os.path.splitext(sys.argv[0])[0])

        config['img-path'] = config.get('img-path', './imgs')
        config['db-file'] = config.get('db-file', './imgs.db')
        config['retry-time'] = 5

        if not os.path.isdir(config['img-path']):
            os.mkdir(config['img-path'])
        
        if not os.path.isfile(config['db-file']):
            createdb(config)

    except getopt.GetoptError as e:
        # print(e.args)
        help()
        sys.exit(1)
    else:
        return config


def help():
    print('Help Text\n'*10)


if __name__ == '__main__':
    config = get_config()
    print(config)

    if config['daemon']:
        daemon.daemon_exec(config)
        while True:
            threading.Thread(
                target=getbingbgimg, args=(config,)).start()
            time.sleep(86400)
    else:
        print('print one time download')
        getbingbgimg(config)
