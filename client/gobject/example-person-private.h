#include "example-person.h"

void example_person_emit_yo(ExamplePerson *self);
struct _ExamplePerson
{
    GObject  parent_instance;
    gchar   *name;
    /* other members */
};