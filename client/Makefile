CFLAGS=`pkg-config --cflags gtk+-3.0`
LDFLAGS=`pkg-config --libs gtk+-3.0`

.PHONY: all clean

all: client

OBJECTS=client.o bingimg_app.o bingimg_win.o resource.o
DEPS=bingimg_app.h bingimg_win.h

resource.c: gresource.xml window.ui
	glib-compile-resources $< --target=$@ --generate-source

%.o: %.c $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $< 

client: $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f resource.c $(OBJECTS) client