/* example-person.h
 *
 * Copyright 2019 blu <y4812@hotmail.com>
 *
 * This file is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This file is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * SPDX-License-Identifier: LGPL-3.0-or-later
 */


#include <glib-object.h>




G_DECLARE_FINAL_TYPE (ExamplePerson, example_person, EXAMPLE, PERSON, GObject)
#define MY_TYPE_OBJECT my_object_get_type ()


const gchar *example_person_get_name(ExamplePerson *self);
void         example_person_set_name(ExamplePerson *self, const gchar *name);
void example_person_emit_yo(ExamplePerson *self);