#include <gtk/gtk.h>

#include "bingimg_app.h"
#include "bingimg_win.h"

struct _BingimgWin
{
    GtkApplicationWindow parent;
};

G_DEFINE_TYPE(BingimgWin, bingimg_win, GTK_TYPE_APPLICATION_WINDOW)


static void bingimg_win_class_init(BingimgWinClass *class)
{
    //gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class),"/com/github/CktD/bingimg/window.ui");
    //gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BingimgWin, stack);
}


gboolean on_button_event(GtkWidget *eventbox, GdkEventButton *event, gpointer data)
{
    switch (event->type)
    {
        case GDK_BUTTON_PRESS:
            g_print("Button Pressed! (%.4f,%.4f)\n",event->x, event->y);
            break;
        case GDK_BUTTON_RELEASE:
            g_print("Button Release! (%.4f,%.4f)\n",event->x, event->y);
            break;
        case GDK_MOTION_NOTIFY:
            g_print("Curosr Move! (%.4f,%.4f)\n",event->x, event->y);
            break;     
        default:
            break;
    }
}

static gpointer cell_new(const gchar *path)
{
  GtkWidget *eventbox;
  GtkWidget *image;
  GdkPixbuf *pixbuf;
  GdkPixbuf *scalled_pixbuf;

  eventbox = gtk_event_box_new();
  pixbuf = gdk_pixbuf_new_from_file(path, NULL);
  scalled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, gdk_pixbuf_get_width(pixbuf)/6 , gdk_pixbuf_get_height(pixbuf)/6, GDK_INTERP_BILINEAR);
  image = gtk_image_new_from_pixbuf(scalled_pixbuf);
  gtk_container_set_border_width(GTK_CONTAINER(eventbox),10);
  gtk_container_add(GTK_CONTAINER(eventbox), image);

  g_object_unref(pixbuf);
  g_object_unref(scalled_pixbuf);

  g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(on_button_event), NULL);
  g_signal_connect(G_OBJECT(eventbox), "button_release_event", G_CALLBACK(on_button_event), NULL);
  g_signal_connect(G_OBJECT(eventbox), "motion-notify-event",G_CALLBACK(on_button_event), NULL);


  gtk_widget_show_all(GTK_WIDGET(eventbox));
  return eventbox;
}

static GArray  *get_image_paths(const gchar *dir_path)
{
  GFile           *dir;
  GFileEnumerator *dir_enumerator;
  GFileInfo       *fileinfo;
  GArray          *paths;
  GString         *name;
  gchar           *c;

  paths = g_array_new(FALSE, FALSE, sizeof(gchar *));

  dir = g_file_new_for_path(dir_path);
  dir_enumerator = g_file_enumerate_children(dir,G_FILE_ATTRIBUTE_STANDARD_DISPLAY_NAME,G_FILE_QUERY_INFO_NONE,NULL,NULL);

  while(fileinfo = g_file_enumerator_next_file(dir_enumerator, NULL,NULL))
  {
    name = g_string_new(dir_path);
        
    g_string_append(name,"/");
    g_string_append(name, g_file_info_get_display_name(fileinfo));
    c = g_string_free(name, FALSE);
    g_array_append_val(paths, c);
    g_object_unref(fileinfo);
  }


  g_object_unref(dir);
  g_object_unref(dir_enumerator);

  return paths;
}

static void bingimg_win_init(BingimgWin *win)
{
    //gtk_widget_init_template(GTK_WIDGET(win));
  GtkWidget *flowbox;
  GtkWidget *scrolled;
  GtkWidget *cell;
  GArray    *paths;
  gint      i;
  gchar     *c;


  gtk_window_set_title(GTK_WINDOW(win), "BingImgs");
  gtk_container_set_border_width(GTK_CONTAINER(win), 20);
  gtk_window_set_default_size(GTK_WINDOW(win), 950, 500);

  scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),GTK_POLICY_NEVER, GTK_POLICY_AUTOMATIC);

  flowbox = gtk_flow_box_new();
  gtk_flow_box_set_max_children_per_line (GTK_FLOW_BOX (flowbox), 10);
  gtk_flow_box_set_selection_mode (GTK_FLOW_BOX (flowbox), GTK_SELECTION_NONE);

  gtk_container_add(GTK_CONTAINER(scrolled), flowbox);
  gtk_container_add(GTK_CONTAINER(win), scrolled);
  gtk_widget_show_all(GTK_WIDGET(win));

  paths = get_image_paths("./imgs");

  for(i = 0; i< paths->len; i++)
  {
      c = g_array_index(paths,gchar * ,i);
      cell = cell_new(c);
      gtk_container_add(GTK_CONTAINER(flowbox), cell);

    //   GdkWindow *w = gtk_widget_get_window(cell);
    //   GdkCursor *cursor = gdk_cursor_new_from_name(gdk_display_get_default(), "pointer");
    //   gdk_window_set_cursor(w, cursor);
      
      g_free(c);
  }
  g_array_unref(paths);
}


BingimgWin * bingimg_win_new(BingimgApp *app)
{
    return g_object_new(BINGIMG_WINDOW_TYPE, 
                        "application", app,
                        NULL);
}

