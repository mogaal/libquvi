/* libquvi
 * Copyright (C) 2013  Toni Gundogdu <legatvs@gmail.com>
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

/** @file subtitle_select.c */

#include "config.h"

#include <glib/gi18n-lib.h>
#include <glib.h>

#include "quvi.h"
/* -- */
#include "_quvi_s.h"
#include "_quvi_subtitle_s.h"
/* -- */
#include "misc/re.h"

static _quvi_subtitle_lang_t _match(_quvi_subtitle_t qsub, const gchar *id)
{
  _quvi_subtitle_type_t qst;
  _quvi_subtitle_lang_t qsl;

  quvi_subtitle_type_reset(qsub);

  while ( (qst = quvi_subtitle_type_next(qsub)) != NULL)
    {
      quvi_subtitle_lang_reset(qst);
      while ( (qsl = quvi_subtitle_lang_next(qst)) != NULL)
        {
          if (m_match(qsl->id->str, id) == TRUE)
            return (qsl);
        }
    }
  return (NULL);
}

/* Return the first available language of the first available type. */
static _quvi_subtitle_lang_t _default(_quvi_subtitle_t qsub)
{
  _quvi_subtitle_type_t qst;

  quvi_subtitle_type_reset(qsub);
  qst = quvi_subtitle_type_next(qsub);

  if (qst == NULL)
    return (NULL);

  quvi_subtitle_lang_reset(qst);
  return (quvi_subtitle_lang_next(qst));
}

static _quvi_subtitle_lang_t _select(_quvi_subtitle_t qsub, const gchar *id)
{
  _quvi_subtitle_lang_t qsl;
  gchar **r;
  _quvi_t q;
  gint i;

  r = g_strsplit(id, ",", 0);
  q = qsub->handle.quvi;

  q->status.rc = QUVI_OK;
  qsl = NULL;

  for (i=0; (r[i] != NULL && qsl == NULL); ++i)
    {
      if (g_strcmp0(r[i], "croak") ==0)
        {
          q->status.rc = QUVI_ERROR_KEYWORD_CROAK;
          break;
        }
      else
        qsl = _match(qsub, r[i]);
    }
  g_strfreev(r);
  return ((qsl == NULL && q->status.rc == QUVI_OK)
          ? _default(qsub)
          : qsl);
}

/** @brief Select a @ref sub_lang matching a @ref sub_lang_id

Matches the @ref sub_lang_id (pattern) to the available subtitle
language IDs and selects the language. This function returns immediately
if a matching ID was found.  The ID value may be a comma-separated value
(e.g. "foo,bar,baz"). The ID may also contain the keyword 'croak' (see
the notes below).
@note
  - ID value is used as regular expression pattern
  - ID may contain the keyword 'croak'
    - This will cause the function to exit immediately when it is reached
    - The result may be checked with @ref quvi_ok
      - The code may be retrieved using @ref quvi_get
      - The error message may be retrieved using @ref quvi_errmsg
  - If nothing matched (and the 'croak' keyword was not specified) the
    function will either:
      - return the first (default) available language, or
      - NULL if the library failed to find any subtitle languages for
        the media
  - Always confirm the result with @ref quvi_ok
  - Calling this function will reset the list pointers for both
    @ref sub_type and @ref sub_lang
@sa @ref parse_subtitle
@sa quvi_subtitle_type_reset
@sa quvi_subtitle_lang_reset
@sa quvi_subtitle_type_next
@sa quvi_subtitle_lang_next
@sa quvi_subtitle_new
@ingroup subprop
*/
const quvi_subtitle_lang_t
quvi_subtitle_select(quvi_subtitle_t handle, const char *id)
{
  /* If G_DISABLE_CHECKS is defined then the check is not performed. */
  g_return_val_if_fail(handle != NULL, NULL);
  g_return_val_if_fail(id != NULL, NULL);

  return (_select(handle, id));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
