/* libquvi
 * Copyright (C) 2012-2013  Toni Gundogdu <legatvs@gmail.com>
 *
 * This file is part of libquvi <http://quvi.sourceforge.net/>.
 *
 * This library is free software: you can redistribute it and/or
 * modify it under the terms of the GNU Affero General Public
 * License as published by the Free Software Foundation, either
 * version 3 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General
 * Public License along with this library.  If not, see
 * <http://www.gnu.org/licenses/>.
 */

/** @file errmsg.c */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"

static const gchar *msg[] =
{
  N_("Not an error"),
  N_("Operation aborted by a callback"),
  N_("Could not find any subtitle export scripts in the path"),
  N_("Could not find any subtitle scripts in the path"),
  N_("Could not find any playlist scripts in the path"),
  N_("Could not find any media scripts in the path"),
  N_("Could not find any scan scripts in the path"),
  N_("Could not find any the utility scripts in the path"),
  N_("Operation aborted by the \"croak\" keyword"),
  N_("An invalid argument to the function"),
  N_("libgcrypt version mismatch"),
  N_("Initialization of libproxy failed"),
  N_("Initialization of libcurl failed"),
  N_("Initialization of liblua failed")
};

static const gchar *inv_code_msg = N_("An invalid error code");

/** @return NULL-terminated error string
@note Do not attempt to free the returned string
@sa @ref getting_started
@ingroup convenience
*/
const char *quvi_errmsg(quvi_t handle)
{
  const gchar *s;
  QuviError c;
  _quvi_t q;
  gint i;

  if (handle == NULL)
    return (g_dgettext(GETTEXT_PACKAGE, msg[QUVI_ERROR_INVALID_ARG]));

  q = (_quvi_t) handle;
  c = q->status.rc;

  for (i=1; msg[i] != NULL; ++i);

  s = msg[0];

  if ( (gint) c<0 || c>i)
    {
      if (q->status.errmsg->len >0)
        s = q->status.errmsg->str;
      else
        s = inv_code_msg;
    }
  else
    s = msg[c];

  return (g_dgettext(GETTEXT_PACKAGE, s));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
