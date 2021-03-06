#!/usr/bin/env python3
# -*- coding:utf-8 -*-

import sys
import math


from bottle import run, route
from bottle import static_file
from bottle import request
from bottle import template
from bottle import install
from bottle_sqlite import SQLitePlugin



config={'log-file':'./www.log',
		'pid-file':'./.www.pid',
		'db-file': '../db/imgs.db',
		'img-path': '../db/imgs',
		'daemon':sys.argv[1] if len(sys.argv)>1 else None}
if config['daemon']:
	import daemon
	daemon.daemon_exec(config)

install(SQLitePlugin(dbfile=config['db-file']))


templatestr="""<!DOCTYPE html>
<html>
<head>
<title>BING IMAGE</title>
<style>
body{
	max-width: 90%;
	margin: auto;
	padding-bottom:2em;
	background-color: #333333;
	color: #eeeeee; 
	font-family: "Helvetica";
	font-size: 2em;
}
div.image{
	margin-top: 3em;
	margin-right: 0em;
	margin-bottom: 0em;
	margin-left: 0em;
	padding: 0em;
}
div.bottom{
	margin-top: 2em;
	margin-bottom: 2em;
	text-align: center;
	vertical-align: middle;
}
.bt-left{
	float: left;
}
.bt-right{
	float: right;
}
a.bt-right:link,
a.bt-left:link,
a.bt-right:visited,
a.bt-left:visited{
	color: #999999;
	padding: 14px 25px;
	text-align: center;
	text-decoration: none;
	display: inline-box;
}
a.bt-right:hover,
a.bt-left:hover,
a.bt-right:active,
a.bt-left:active{
	color: #ffffff;
}
</style>
</head>
<body>
% for img in imgs:
<div class='image'>
<p>{{img[0]}}</p>
<a href="{{img[2].strip()}}">
<img src="/db/imgs/{{img[0]}}.jpg" title ="{{img[1]}}" width="100%">
</a>
</div>
% end

<div class='bottom'>
% if pgup:
<a class="bt-left" href="{{pgup}}">上一页</a>
% end
<span class="text-middle"> 共{{num}}张 &ensp;&ensp;&ensp; 第{{page}}页 共{{page_total}}页</span>
% if pgdn:
<a class="bt-right" href="{{pgdn}}">下一页</a>
% end
</div>
</body>
</html>"""

@route("/index")
@route("/")
def index(db):
    #print(request.query_string)
    offset = request.query.offset or 0
    limit = request.query.limit or 5
    try:
        offset=int(offset)
        limit=int(limit)
    except:
        offset = 0
        limit = 5
    else:
        if offset < 0:
            offset = 0
        if limit <= 0:
            limit = 5

    num = db.execute('select count(*) from imgs').fetchone()[0]

    datas = db.execute('select * from imgs order by startdate DESC limit ? offset ?',(limit,offset*limit)).fetchall()
    #print(datas)
    m = {'num':num,
        'imgs':datas,
        'page':offset+1,
        'page_total':int(math.ceil(float(num)/float(limit))),
        'pgup':'index?offset=%s&limit=%s'%(offset-1,limit) if offset else None,
        'pgdn':'index?offset=%s&limit=%s'%(offset+1,limit) if (offset+1)*limit < num else None}
    return template(templatestr,**m)

@route("/db/imgs/<filename>")
def server_imgs(filename):
    return static_file(filename,root=config['img-path'])


@route("/api/months")
def months(db):
	months = db.execute("select distinct startdate/100 from imgs order by startdate DESC").fetchall()
	months = [ str(x[0]) for x in months]
	return {"months": months}

@route("/api/imgs/<month>")
def images(month ,db):
	if month.isnumeric() and len(month) == 6:
		imgs = db.execute("select * from imgs where startdate/100=?  order by startdate ASC", (int(month),)).fetchall()
		imgs = [ {'date': str(x[0]), 'copyright':x[1], 'url': "/db/imgs/%s.jpg" % str(x[0])} for x in imgs ]
		imgs = {'images': imgs}
	else:
		imgs = {'images':[]}
	return imgs

run(host='',port=8888,debug=None)
