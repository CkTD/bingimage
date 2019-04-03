#include <libsoup/soup.h>
#include <json-glib/json-glib.h>
#include <gtk/gtk.h>

#include "network.h"

struct ptrs
{
    BingimgWin  *win;
    gpointer     callback;
    gpointer     data;
};

static SoupSession *session;

static GString *base_url = NULL;
static GString *api_months_url = NULL;
static GString *api_images_base_url = NULL; 
static GString *api_image_base_url = NULL;


void init_network(const char* url)
{
    if (base_url)
        return;
    session = soup_session_new();

    base_url = g_string_new(url);

    api_months_url = g_string_new(base_url->str);
    g_string_append(api_months_url, "/api/months");

    api_images_base_url = g_string_new(base_url->str);
    g_string_append(api_images_base_url, "/api/imgs/");
}



// get all months...
static GArray* get_months_json_parse(const gchar *data, gint length)
{
    JsonParser  *parser;
    JsonNode    *node;
    JsonObject  *object;
    JsonArray   *array;
    GValue       value = G_VALUE_INIT;
    GError      *error;
    gboolean     retval;
    gint         i;
    gchar       *c;

    GArray      *months;


    parser = json_parser_new();
    retval = json_parser_load_from_data(parser, data, length, &error);
    if(!retval)
    {
        g_print ("Unable to parse  %s\n", error->message);
        g_error_free (error);
        g_object_unref (parser);
        return NULL;
    }
    node = json_parser_get_root(parser);
    object = json_node_get_object(node);
    node = json_object_get_member(object, "months");
    array = json_node_get_array(node);

    months =  g_array_new(FALSE, FALSE, sizeof(gchar *));
    for(i=0;i<json_array_get_length(array);i++)
    {
        node = json_array_get_element(array, i);
        json_node_get_value(node, &value);
        g_assert (G_VALUE_HOLDS_STRING (&value));
        c = g_strdup(g_value_get_string(&value));
        g_array_append_val(months, c);
        g_value_unset(&value);

    }

    g_object_unref(parser);
    return months;
}

static void get_months_callback(SoupSession *session, SoupMessage * msg, gpointer p)
{
    network_callback    callback;
    BingimgWin         *win;
    gpointer           *data;
    gpointer            months;

    callback = (network_callback)(((struct ptrs *)p)->callback);
    win = (BingimgWin *)(((struct ptrs *)p)->win);
    data = ((struct ptrs *)p)->data;

    if (msg->status_code != 200)
    {
        callback(win, NULL, data);
    }
    else
    {
        months = get_months_json_parse(msg->response_body->data, msg->response_body->length);
        callback(win, months, data);
    }

    g_free(p);
}

void network_get_months(BingimgWin *win, network_callback func, gpointer data)
{
    SoupMessage  *msg;
    struct ptrs  *p;

    msg = soup_message_new("GET", api_months_url->str);
    p = (struct ptrs *)g_malloc(sizeof(struct ptrs));
    p->callback = func;
    p->data = data;
    p->win = win;
    soup_session_queue_message(session, msg, get_months_callback, p);
}


// get images metadata from one months
static GArray* get_images_json_parse(const gchar *data, gint length)
{
    JsonParser        *parser;
    JsonNode          *node;
    JsonObject        *object;
    JsonArray         *array;
    GValue             value = G_VALUE_INIT;
    GError            *error;
    gboolean           retval;
    gint               i;

    GArray            *images;
    ImageMeta         *meta;


    parser = json_parser_new();
    retval = json_parser_load_from_data(parser, data, length, &error);
    if(!retval)
    {
        g_print ("Unable to parse  %s\n", error->message);
        g_error_free (error);
        g_object_unref (parser);
        return NULL;
    }

    node = json_parser_get_root(parser);
    object = json_node_get_object(node);
    node = json_object_get_member(object, "images");
    array = json_node_get_array(node);

    images =  g_array_new(FALSE, FALSE, sizeof(ImageMeta *));
    for(i=0;i<json_array_get_length(array);i++)
    {
        meta = g_malloc(sizeof(ImageMeta));
        node = json_array_get_element(array, i);
        object = json_node_get_object(node);
        
        node = json_object_get_member(object, "date");
        json_node_get_value(node, &value);
        g_assert (G_VALUE_HOLDS_STRING (&value));
        meta->date = g_strdup(g_value_get_string(&value));
        g_value_unset(&value);

        node = json_object_get_member(object, "copyright");
        json_node_get_value(node, &value);
        g_assert (G_VALUE_HOLDS_STRING (&value));
        meta->copyright = g_strdup(g_value_get_string(&value));
        g_value_unset(&value);

        node = json_object_get_member(object, "url");
        json_node_get_value(node, &value);
        g_assert (G_VALUE_HOLDS_STRING (&value));
        meta->url = g_strdup(g_value_get_string(&value));
        g_value_unset(&value);

        g_array_append_val(images, meta);
    }

    g_object_unref(parser);
    return images;
}

static void get_images_callback(SoupSession *session, SoupMessage * msg, gpointer p)
{
    network_callback    callback;
    BingimgWin         *win;
    gpointer           *data;
    gpointer            images;

    callback = (network_callback)(((struct ptrs *)p)->callback);
    win = (BingimgWin *)(((struct ptrs *)p)->win);
    data = ((struct ptrs *)p)->data;

    if (msg->status_code != 200)
    {
        callback(win, NULL, data);
    }
    else
    {
        images = get_images_json_parse(msg->response_body->data, msg->response_body->length);
        callback(win, images, data);
    }

    g_free(p);
}

void network_get_images(const gchar *month, BingimgWin *win, network_callback func, gpointer data)
{
    SoupMessage  *msg;
    struct ptrs  *p;
    GString      *url;

    url = g_string_new(api_images_base_url->str);
    g_string_append(url, month);
    msg = soup_message_new("GET", url->str );
    p = (struct ptrs *)g_malloc(sizeof(struct ptrs));
    p->callback = func;
    p->data = data;
    p->win = win;
    soup_session_queue_message(session, msg, get_images_callback, p);

    g_string_free(url, TRUE);
}




// download a single image
static void get_image_callback(SoupSession *session, SoupMessage * msg, gpointer p)
{
    network_callback    callback;
    BingimgWin         *win;
    gpointer           *data;
    GdkPixbuf          *pixbuf;
    GdkPixbufLoader    *loader;


    callback = (network_callback)(((struct ptrs *)p)->callback);
    win = (BingimgWin *)(((struct ptrs *)p)->win);
    data = ((struct ptrs *)p)->data;

    if (msg->status_code != 200)
    {
        callback(win, NULL, data);
    }
    else
    {
        loader = gdk_pixbuf_loader_new();
        gdk_pixbuf_loader_write(loader,msg->response_body->data,msg->response_body->length,NULL);
        gdk_pixbuf_loader_close(loader, NULL);
        pixbuf = gdk_pixbuf_loader_get_pixbuf(loader);
        g_object_ref(pixbuf);
        callback(win, pixbuf, data);
        g_object_unref(G_OBJECT(loader));
    }

    g_free(p);

}

void network_get_image(const gchar *r_url, BingimgWin *win, network_callback func, gpointer data)
{
    SoupMessage  *msg;
    struct ptrs  *p;
    GString      *url;

    url = g_string_new(base_url->str);
    g_string_append(url, r_url);
    msg = soup_message_new("GET", url->str );
    p = (struct ptrs *)g_malloc(sizeof(struct ptrs));
    p->callback = func;
    p->data = data;
    p->win = win;
    soup_session_queue_message(session, msg, get_image_callback, p);

    g_string_free(url, TRUE);
}