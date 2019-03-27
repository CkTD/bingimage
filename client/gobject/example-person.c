/* exampe-person.c
 *
 * Copyright 2019 blu <y4812@hotmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */


#include "example-person-private.h"

/* in myobject.c */



G_DEFINE_TYPE (ExamplePerson, example_person, G_TYPE_OBJECT)


enum {
    PROP_0,
    PROP_NAME,
    LAST_PROP
};

enum {
    YO,
    LAST_SIGNAL
};

static GParamSpec *properties[LAST_PROP];
static guint       signals[LAST_SIGNAL];

static void
example_person_get_property(GObject    *object,
                            guint       prop_id,
                            GValue     *value,
                            GParamSpec *pspec)
{
  ExamplePerson *self = (ExamplePerson *)object;
  switch (prop_id)
  {
      case PROP_NAME:
          g_value_set_string(value, example_person_get_name(self));
          break;
  }
}


static void
example_person_set_property(GObject       *object,
                            guint          prop_id,
                            const GValue  *value,
                            GParamSpec    *pspec)
{
  ExamplePerson *self = (ExamplePerson *)object;
  switch (prop_id)
  {
      case PROP_NAME:
        example_person_set_name(self, g_value_get_string(value));
          break;
  }
}

static void
example_person_class_init(ExamplePersonClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS(klass);
    object_class->get_property = example_person_get_property;
    object_class->set_property = example_person_set_property;
    properties[PROP_NAME] = g_param_spec_string("name",
                                    "Name",
                                    "The name of the person",
                                NULL,
                                (G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS));

    g_object_class_install_properties(object_class, LAST_PROP, properties);

    signals[YO] = g_signal_new("YO", G_TYPE_FROM_CLASS(klass), G_SIGNAL_RUN_LAST, 0, NULL, NULL, NULL, G_TYPE_NONE, 0);
}

void example_person_emit_yo(ExamplePerson *self)
{
    g_signal_emit(self, signals[YO], 0);
}

static void 
example_person_init(ExamplePerson *self)
{
    self->name = g_strdup("dummy");
}

const gchar *
example_person_get_name(ExamplePerson *self)
{
    return self->name;
}

void         
example_person_set_name(ExamplePerson *self, const gchar *name)
{
    if (g_strcmp0(self->name, name)==0)
    {
        g_free(self->name);
        self->name = g_strdup(name);
    }
}