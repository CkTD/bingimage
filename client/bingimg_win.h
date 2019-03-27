#ifndef __BINGIMG_WIN_H
#define __BINGIMG_WIN_H

#include <gtk/gtk.h>
#include "bingimg_app.h"

G_DECLARE_FINAL_TYPE(BingimgWin, bingimg_win, BINGIMG, WIN, GtkApplicationWindow)
#define BINGIMG_WINDOW_TYPE bingimg_win_get_type()

BingimgWin    *bingimg_win_new(BingimgApp *app);

#endif  /* __BINGIMG_WIN_H */