#include <gtk/gtk.h>

#include "bingimg_app.h"
#include "network.h"

int
main (int argc, char *argv[])
{
  init_network("http://127.0.0.1:8888");
  return g_application_run (G_APPLICATION (bingimg_app_new ()), argc, argv);
}