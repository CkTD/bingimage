#ifndef __NETWORK_H
#define __NETWORK_H

#include <glib.h>
#include "bingimg_win.h"


void init_network(const char* url);

typedef void (*network_callback)(BingimgWin *win, gpointer result, gpointer data);
void network_get_months(BingimgWin *win, network_callback func, gpointer data);


typedef struct 
{
    gchar   *date;
    gchar   *copyright;
    gchar   *url;
}ImageMeta;
void network_get_images(const gchar *month, BingimgWin *win, network_callback func, gpointer data);


void network_get_image(const gchar *name, BingimgWin *win, network_callback func, gpointer data);

#endif /* __NETWORK_H */