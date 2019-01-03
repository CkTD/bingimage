# -*- coding:utf-8 -*-
import sys

config={'log-file':'./www.log',
		'pid-file':'./.www.pid',
		'daemon':sys.argv[1] if len(sys.argv)>1 else None}
if config['daemon']:
	import daemon
	daemon.daemon_exec(config)



from bottle import run, route
from bottle import static_file
from bottle import request
from bottle import template
from bottle import install

import math

from bottle_sqlite import SQLitePlugin
install(SQLitePlugin(dbfile='./db/imgs.db'))

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
    return static_file(filename,root="./db/imgs")


run(host='',port=88,debug=None)
