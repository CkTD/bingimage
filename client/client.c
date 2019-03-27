#include <gtk/gtk.h>

#include "bingimg_app.h"

int
main (int argc, char *argv[])
{
  return g_application_run (G_APPLICATION (bingimg_app_new ()), argc, argv);
}