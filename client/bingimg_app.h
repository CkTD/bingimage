#ifndef __BINGIMG_APP_H
#define __BINGIMG_APP_H

#include <gtk/gtk.h>

G_DECLARE_FINAL_TYPE(BingimgApp, bingimg_app, BINGIMG, APP, GtkApplication)

/*  Expands to: 
    typedef struct _BingimgApp BingimgApp;
    typedef struct
    {
        GtkApplicationClass parent_class;
    } BingimgAppClass;
    typedef BingimgApp *BingimgApp_autoptr;
    static inline void glib_autoptr_cleanup_BingimgApp(BingimgApp **_ptr) { glib_autoptr_cleanup_GtkApplication((GtkApplication **)_ptr); }
    static inline BingimgApp *BINGIMG_APP(gpointer ptr) { return (((BingimgApp *)g_type_check_instance_cast((GTypeInstance *)(ptr), (bingimg_app_get_type())))); }
    static inline gboolean BINGIMG_IS_APP(gpointer ptr)
    {
        return ((__extension__({ GTypeInstance *__inst = (GTypeInstance*) (ptr); GType __t = (bingimg_app_get_type ()); gboolean __r; if (!__inst) __r = (0); else if (__inst->g_class && __inst->g_class->g_type == __t) __r = (!(0)); else __r = g_type_check_instance_is_a (__inst, __t); __r; })));
    }
*/

BingimgApp    *bingimg_app_new(void);
#define BINGIMG_APP_TYPE bingimg_app_get_type()


#endif