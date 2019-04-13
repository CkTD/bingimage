# Download background image from bing.com

## db
download images  
getbingimage.py -d start 

## webserver
show images on web page [link](http://65.49.232.31:8888)

## client
GTK3 application to view images by webserver's api  

### dependencies
- gtk+-3.0
- libsoup-2.4
- json-glib-1.0

### build
```bash
meson builddir && cd builddir && ninja
```