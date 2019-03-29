#include <gtk/gtk.h>

#include "bingimg_app.h"
#include "bingimg_win.h"
#include "network.h"

struct _BingimgWin
{
  GtkApplicationWindow   parent;
  GtkWidget             *flowbox;
  GtkWidget             *previous_button;
  GtkWidget             *next_button;
  GtkWidget             *month_label;
  
  GArray                *cells;
  GArray                *images;

  GArray                *months;
  glong                  current_index;
  
};

G_DEFINE_TYPE(BingimgWin, bingimg_win, GTK_TYPE_APPLICATION_WINDOW)




static void set_cells(BingimgWin *win ,GArray *metas)
{
  GtkBuilder *builder;

  GtkWidget  *box;
  GtkWidget  *eventbox;
  GtkWidget  *image;
  GtkWidget  *label;
  GdkPixbuf  *pixbuf;
  GdkPixbuf  *scalled_pixbuf;
   
  ImageMeta  *meta;
  int         i;
  
  for(i=0;i< win->cells->len ; i++)
    gtk_widget_destroy(gtk_widget_get_parent( g_array_index(win->cells, GtkWidget *, i) ));

  g_array_remove_range(win->cells, 0, win->cells->len);
  g_array_remove_range(win->images, 0, win->images->len);
  

  for(i=0; i<metas->len; i++)
  {
    meta = g_array_index(metas, ImageMeta *,i);

    builder = gtk_builder_new_from_resource("/com/github/CktD/bingimg/cell.ui");

    box = GTK_WIDGET(gtk_builder_get_object(builder, "box"));
    eventbox = GTK_WIDGET(gtk_builder_get_object(builder, "eventbox"));
    image = GTK_WIDGET(gtk_builder_get_object(builder, "image"));
    label = GTK_WIDGET(gtk_builder_get_object(builder, "label"));

    gtk_label_set_label(GTK_LABEL(label), meta->date);
    gtk_container_add(GTK_CONTAINER(win->flowbox), box);
    gtk_widget_show_all(GTK_WIDGET(box));

    g_array_append_val(win->cells, box);
    g_array_append_val(win->images, image);
    //g_print("%s | %s | %s\n", meta->date, meta->copyright, meta->url);
  }

/*
  eventbox = gtk_event_box_new();
  pixbuf = gdk_pixbuf_new_from_file(path, NULL);
  scalled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, gdk_pixbuf_get_width(pixbuf) / 6, gdk_pixbuf_get_height(pixbuf) / 6, GDK_INTERP_BILINEAR);
  image = gtk_image_new_from_pixbuf(scalled_pixbuf);
  gtk_container_set_border_width(GTK_CONTAINER(eventbox), 10);
  gtk_container_add(GTK_CONTAINER(eventbox), image);

  g_object_unref(pixbuf);
  g_object_unref(scalled_pixbuf);

  g_signal_connect(G_OBJECT(eventbox), "button_press_event", G_CALLBACK(on_image_button_event), NULL);
  g_signal_connect(G_OBJECT(eventbox), "button_release_event", G_CALLBACK(on_image_button_event), NULL);
  g_signal_connect(G_OBJECT(eventbox), "motion-notify-event", G_CALLBACK(on_image_button_event), NULL);

  gtk_widget_show_all(GTK_WIDGET(eventbox));
  return eventbox;
*/
}

typedef struct
{
  gint current_index;
  gint image_index;
}ImageDoneCallbackData;
static void on_get_image_done(BingimgWin *win, gpointer result, gpointer data)
{
  ImageDoneCallbackData *idcd;
  GdkPixbuf             *pixbuf;
  GdkPixbuf             *scalled_pixbuf;
  GtkWidget             *image;

  idcd = (ImageDoneCallbackData *)data;
  pixbuf = GDK_PIXBUF(result);

  if(win->current_index == idcd->current_index)
  {
    image = g_array_index(win->images, GtkWidget *, idcd->image_index);

    scalled_pixbuf = gdk_pixbuf_scale_simple(pixbuf, gdk_pixbuf_get_width(pixbuf) / 6, gdk_pixbuf_get_height(pixbuf) / 6, GDK_INTERP_BILINEAR);
    gtk_image_set_from_pixbuf(GTK_IMAGE(image), scalled_pixbuf);
    g_object_unref(pixbuf);
    g_object_unref(scalled_pixbuf);
  }

  g_free(data);
}

static void on_get_images_done(BingimgWin *win, gpointer result, gpointer data)
{
  GArray     *metas;

  ImageMeta  *meta;
  gint        i;
  ImageDoneCallbackData *idcd;

  metas = (GArray *)result;
  if (win->current_index == (glong)data)
  {
    set_cells(win, metas);

    for(i=0;i<metas->len;i++)
    {
      meta = g_array_index(metas, ImageMeta*, i);
      idcd = g_malloc(sizeof(ImageDoneCallbackData));
      idcd->current_index = win->current_index;
      idcd->image_index = i;
      network_get_image(meta->url, win, on_get_image_done, idcd );
    }
  }

  for(i=0;i<metas->len;i++)
  {
    meta = g_array_index(metas, ImageMeta*, i);
    g_free(meta->date);
    g_free(meta->copyright);
    g_free(meta->url);
    g_free(meta);
  }

  g_array_unref(metas);
}

static void on_get_months_done(BingimgWin *win, gpointer result, gpointer data)
{
  GArray  *months;
  gint     length;
  gchar   *c;

  if (result)
  {
    months = (GArray *)result;
    win->months = months;
    length = months->len;

    if (length > 0)
    {
      win->current_index = 0;
      c = g_array_index(months, gchar *, win->current_index);
      gtk_label_set_label(GTK_LABEL(win->month_label), c);
      network_get_images(c, win, on_get_images_done, (gpointer)win->current_index);
    }
    if (length > 1)
      gtk_widget_set_sensitive(GTK_WIDGET(win->previous_button), TRUE);
  }
  else
  {
    /* code */
  }
  
}



static void on_next_button_event(GtkWidget *button, GdkEvent *event, gpointer data)
{
  BingimgWin *win;

  win = BINGIMG_WIN(data);
  g_assert(win->current_index > 0);
  win->current_index -= 1;
  gtk_label_set_label(GTK_LABEL(win->month_label), g_array_index(win->months, gchar *, win->current_index));
  if(win->current_index == 0)
    gtk_widget_set_sensitive(GTK_WIDGET(win->next_button), FALSE);
  if(win->current_index < win->months->len -1)
    gtk_widget_set_sensitive(GTK_WIDGET(win->previous_button), TRUE);
  network_get_images(g_array_index(win->months, gchar* , win->current_index), win, on_get_images_done, (gpointer)win->current_index);
}

static void on_previous_button_event(GtkWidget *button, GdkEvent *event, gpointer data)
{
  BingimgWin *win;

  win = BINGIMG_WIN(data);
  g_assert(win->current_index < win->months->len - 1);
  win->current_index += 1;
  gtk_label_set_label(GTK_LABEL(win->month_label), g_array_index(win->months, gchar *, win->current_index));
  if(win->current_index > 0)
    gtk_widget_set_sensitive(GTK_WIDGET(win->next_button), TRUE);
  if(win->current_index == win->months->len -1)
    gtk_widget_set_sensitive(GTK_WIDGET(win->previous_button), FALSE);

  network_get_images(g_array_index(win->months, gchar* , win->current_index), win, on_get_images_done, (gpointer)win->current_index);
}


gboolean on_image_button_event(GtkWidget *eventbox, GdkEventButton *event, gpointer data)
{
  switch (event->type)
  {
  case GDK_BUTTON_PRESS:
    g_print("Button Pressed! (%.4f,%.4f)\n", event->x, event->y);
    break;
  case GDK_BUTTON_RELEASE:
    g_print("Button Release! (%.4f,%.4f)\n", event->x, event->y);
    break;
  case GDK_MOTION_NOTIFY:
    g_print("Curosr Move! (%.4f,%.4f)\n", event->x, event->y);
    break;
  default:
    break;
  }
}

static void bingimg_win_class_init(BingimgWinClass *class)
{
  gtk_widget_class_set_template_from_resource(GTK_WIDGET_CLASS(class), "/com/github/CktD/bingimg/window.ui");
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BingimgWin, flowbox);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BingimgWin, previous_button);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BingimgWin, next_button);
  gtk_widget_class_bind_template_child(GTK_WIDGET_CLASS(class), BingimgWin, month_label);
}

static void bingimg_win_init(BingimgWin *win)
{
  gtk_widget_init_template(GTK_WIDGET(win));
  g_signal_connect(G_OBJECT(win->next_button), "button_press_event", G_CALLBACK(on_next_button_event), win);
  g_signal_connect(G_OBJECT(win->previous_button), "button_press_event", G_CALLBACK(on_previous_button_event), win);

  gtk_widget_set_sensitive(GTK_WIDGET(win->next_button), FALSE);
  gtk_widget_set_sensitive(GTK_WIDGET(win->previous_button), FALSE);

  gtk_widget_show_all(GTK_WIDGET(win));

  win->cells = g_array_new(FALSE, FALSE, sizeof(GtkWidget *));
  win->images = g_array_new(FALSE, FALSE, sizeof(GtkWidget *));

  network_get_months(BINGIMG_WIN(win), on_get_months_done,NULL);
}

BingimgWin *bingimg_win_new(BingimgApp *app)
{
  return g_object_new(BINGIMG_WINDOW_TYPE,
                      "application", app,
                      NULL);
}











/*
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

static void add_images(GtkWidget *flowbox)
{
  GtkWidget *cell;
  GArray    *paths;
  gint      i;
  gchar     *c;

  paths = get_image_paths("./imgs");

  for(i = 0; i< paths->len; i++)
  {
      c = g_array_index(paths,gchar * ,i);
      cell = cell_new(c);
      gtk_container_add(GTK_CONTAINER(flowbox), cell);

    GdkWindow *w = gtk_widget_get_window(cell);
       GdkCursor *cursor = gdk_cursor_new_from_name(gdk_display_get_default(), "pointer");
       gdk_window_set_cursor(w, cursor);
      
      g_free(c);
  }
  g_array_unref(paths);
}
*/
