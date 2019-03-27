#include "bingimg_app.h"
#include "bingimg_win.h"

struct _BingimgApp
{
    GtkApplication parent;
};

G_DEFINE_TYPE(BingimgApp, bingimg_app, GTK_TYPE_APPLICATION)

/*  Expands to: 
    static void bingimg_app_init(BingimgApp *self);
    static void bingimg_app_class_init(BingimgAppClass *klass);
    static GType bingimg_app_get_type_once(void);
    static gpointer bingimg_app_parent_class = ((void *)0);
    static gint BingimgApp_private_offset;
    static void bingimg_app_class_intern_init(gpointer klass)
    {
        bingimg_app_parent_class = g_type_class_peek_parent(klass);
        if (BingimgApp_private_offset != 0)
            g_type_class_adjust_private_offset(klass, &BingimgApp_private_offset);
        bingimg_app_class_init((BingimgAppClass *)klass);
    }
    __attribute__((__unused__)) static inline gpointer bingimg_app_get_instance_private(BingimgApp *self) { return (((gpointer)((guint8 *)(self) + (glong)(BingimgApp_private_offset)))); }
    GType bingimg_app_get_type(void)
    {
        static volatile gsize g_define_type_id__volatile = 0;
        if ((__extension__({ typedef char _GStaticAssertCompileTimeAssertion_1[(sizeof *(&g_define_type_id__volatile) == sizeof (gpointer)) ? 1 : -1] __attribute__((__unused__)); (void) (0 ? (gpointer) *(&g_define_type_id__volatile) : 0); (!(__extension__ ({ typedef char _GStaticAssertCompileTimeAssertion_2[(sizeof *(&g_define_type_id__volatile) == sizeof (gpointer)) ? 1 : -1] __attribute__((__unused__)); guint64 gapg_temp = __atomic_load_8 ((&g_define_type_id__volatile), 5); (gpointer) gapg_temp; })) && g_once_init_enter (&g_define_type_id__volatile)); })))
        {
            GType g_define_type_id = bingimg_app_get_type_once();
            (__extension__({ typedef char _GStaticAssertCompileTimeAssertion_3[(sizeof *(&g_define_type_id__volatile) == sizeof (gpointer)) ? 1 : -1] __attribute__((__unused__)); (void) (0 ? *(&g_define_type_id__volatile) = (g_define_type_id) : 0); g_once_init_leave ((&g_define_type_id__volatile), (gsize) (g_define_type_id)); }));
        }
        return g_define_type_id__volatile;
    }
    __attribute__((noinline)) static GType bingimg_app_get_type_once(void)
    {
        GType g_define_type_id = g_type_register_static_simple((gtk_application_get_type()), g_intern_static_string("BingimgApp"), sizeof(BingimgAppClass), (GClassInitFunc)(void (*)(void))bingimg_app_class_intern_init, sizeof(BingimgApp), (GInstanceInitFunc)(void (*)(void))bingimg_app_init, (GTypeFlags)0);
        {
            {
                {};
            }
        }
        return g_define_type_id;
    }
*/

static void bingimg_app_activate(GApplication *app);




static void bingimg_app_class_init(BingimgAppClass *class)
{
    G_APPLICATION_CLASS(class)->activate = bingimg_app_activate;
}

static void bingimg_app_init(BingimgApp *app)
{

}

static void bingimg_app_activate(GApplication *app)
{
    BingimgWin *win;
    win = bingimg_win_new(BINGIMG_APP(app));
    gtk_window_present(GTK_WINDOW(win));
}

BingimgApp* bingimg_app_new(void)
{
    return g_object_new(BINGIMG_APP_TYPE,
                        "application-id","com.github.CkTD.bingimg",
                        "flags", G_APPLICATION_FLAGS_NONE,
                        NULL);
}