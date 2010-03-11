/* GObject - GLib Type, Object, Parameter and Signal Library
 * Copyright (C) 1998-1999, 2000-2001 Tim Janik and Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/* This code is included from the GLib-2.23 git repository */

#ifndef GINTERFACE_H
#define GINTERFACE_H

G_BEGIN_DECLS

/**
 * G_DEFINE_INTERFACE:
 * @TN: The name of the new type, in Camel case.
 * @t_n: The name of the new type, in lowercase, with words separated by '_'.
 * @T_P: The #GType of the prerequisite type for the interface, or 0
 * (%G_TYPE_INVALID) for no prerequisite type.
 *
 * A convenience macro for #GTypeInterface definitions, which declares
 * a default vtable initialization function and defines a *_get_type()
 * function.
 *
 * The macro expects the interface initialization function to have the
 * name <literal>t_n ## _default_init</literal>, and the interface
 * structure to have the name <literal>TN ## Interface</literal>.
 *
 * Since: 2.24
 */
#define G_DEFINE_INTERFACE(TN, t_n, T_P)		    G_DEFINE_INTERFACE_WITH_CODE(TN, t_n, T_P, ;)

/**
 * G_DEFINE_INTERFACE_WITH_CODE:
 * @TN: The name of the new type, in Camel case.
 * @t_n: The name of the new type, in lowercase, with words separated by '_'.
 * @T_P: The #GType of the prerequisite type for the interface, or 0
 * (%G_TYPE_INVALID) for no prerequisite type.
 * @_C_: Custom code that gets inserted in the *_get_type() function.
 *
 * A convenience macro for #GTypeInterface definitions. Similar to
 * G_DEFINE_INTERFACE(), but allows you to insert custom code into the
 * *_get_type() function, e.g. additional interface implementations
 * via G_IMPLEMENT_INTERFACE(), or additional prerequisite types. See
 * G_DEFINE_TYPE_EXTENDED() for a similar example using
 * G_DEFINE_TYPE_WITH_CODE().
 *
 * Since: 2.24
 */
#define G_DEFINE_INTERFACE_WITH_CODE(TN, t_n, T_P, _C_)     _G_DEFINE_INTERFACE_EXTENDED_BEGIN(TN, t_n, T_P) {_C_;} _G_DEFINE_INTERFACE_EXTENDED_END()

#define _G_DEFINE_INTERFACE_EXTENDED_BEGIN(TypeName, type_name, TYPE_PREREQ) \
\
static void     type_name##_default_init        (TypeName##Interface *klass); \
\
GType \
type_name##_get_type (void) \
{ \
  static volatile gsize g_define_type_id__volatile = 0; \
  if (g_once_init_enter (&g_define_type_id__volatile))  \
    { \
      GType g_define_type_id = \
        g_type_register_static_simple (G_TYPE_INTERFACE, \
                                       g_intern_static_string (#TypeName), \
                                       sizeof (TypeName##Interface), \
                                       (GClassInitFunc)type_name##_default_init, \
                                       0, \
                                       (GInstanceInitFunc)NULL, \
                                       (GTypeFlags) 0); \
      if (TYPE_PREREQ) \
        g_type_interface_add_prerequisite (g_define_type_id, TYPE_PREREQ); \
      { /* custom code follows */
#define _G_DEFINE_INTERFACE_EXTENDED_END()	\
        /* following custom code */		\
      }						\
      g_once_init_leave (&g_define_type_id__volatile, g_define_type_id); \
    }						\
  return g_define_type_id__volatile;			\
} /* closes type_name##_get_type() */

G_END_DECLS

#endif /* GINTERFACE_H */
