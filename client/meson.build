project('client', 'c')

gtk_dep = dependency('gtk+-3.0')
soup_dep = dependency('libsoup-2.4')
json_dep = dependency('json-glib-1.0')

gnome = import('gnome')

gresources = gnome.compile_resources(
    'gresources', 'gresource.xml'
)

executable(
    'client',
    ['client.c', 'bingimg_app.c', 'bingimg_win.c', 'network.c', gresources],
    dependencies : [gtk_dep, soup_dep, json_dep]
)