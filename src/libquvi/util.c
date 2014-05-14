/* libquvi
 * Copyright (C) 2009-2011  Toni Gundogdu <legatvs@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301  USA
 */

#include "config.h"

#include <string.h>
#include <errno.h>
#include <stdarg.h>
#include <assert.h>

#ifdef HAVE_ICONV
#include <iconv.h>
#endif

#include "quvi/quvi.h"
#include "quvi/net.h"
#include "quvi/llst.h"

#include "internal.h"
#include "curl_wrap.h"
#include "util.h"

char *vafreprintf(char **dst, const char *fmt, va_list args)
{
  _free(*dst);
  assert(*dst == NULL);

  vasprintf(dst, fmt, args);
  va_end(args);

  return (*dst);
}

char *freprintf(char **dst, const char *fmt, ...)
{
  va_list args;

  va_start(args, fmt);
  vafreprintf(dst, fmt, args);

  return (*dst);
}

#ifdef HAVE_ICONV
QUVIcode to_utf8(_quvi_media_t media)
{
  static const char to[] = "UTF-8";
  size_t insize, avail, iconv_code;
  char inbuf[1024], outbuf[1024];
  ICONV_CONST char *inptr;
  char *from, *wptr;
  iconv_t cd;

  assert(media != 0);
  assert(media->quvi != 0);
  assert(media->title != 0);
  assert(media->charset != 0);

  wptr = outbuf;
  inptr = inbuf;
  avail = sizeof(outbuf);
  insize = strlen(media->title);

  if (insize >= sizeof(inbuf))
    insize = sizeof(inbuf);

  memset(wptr, 0, sizeof(outbuf));

  snprintf(inbuf, sizeof(inbuf), "%s", media->title);

  /* Try with TRANSLIT first. */
  asprintf(&from, "%s//TRANSLIT", media->charset);
  cd = iconv_open(to, from);

  /* If that fails, then without TRANSLIT. */
  if (cd == (iconv_t) - 1)
    {
      _free(from);
      asprintf(&from, "%s", media->charset);
      cd = iconv_open(to, from);
    }

  if (cd == (iconv_t) - 1)
    {
      if (errno == EINVAL)
        {
          freprintf(&media->quvi->errmsg,
                    "conversion from %s to %s unavailable", from, to);
        }
      else
        {
#ifdef HAVE_STRERROR
          freprintf(&media->quvi->errmsg, "iconv_open: %s",
                    strerror(errno));
#else
          perror("iconv_open");
#endif
        }

      _free(from);

      return (QUVI_ICONV);
    }

  iconv_code = iconv(cd, &inptr, &insize, &wptr, &avail);
  iconv_close(cd);
  cd = 0;

  if (iconv_code == (size_t) - 1)
    {
      freprintf(&media->quvi->errmsg,
                "converting characters from '%s' to '%s' failed", from,
                to);
      _free(from);
      return (QUVI_ICONV);
    }
  else
    {
      freprintf(&media->title, "%s", outbuf);
    }

  _free(from);

  return (QUVI_OK);
}
#endif /* HAVE_ICONV */

char *unescape(_quvi_t quvi, char *s)
{
  return curl_unescape_url(quvi, s);
}

char *from_html_entities(char *src)
{
  struct lookup_s
  {
    const char *from;
    const char *to;
  };

  static const struct lookup_s conv[] =
  {
    {"&quot;", "\""},
    {"&#34;", "\""},
    {"&#034;", "\""},
    {"&amp;", "&"},
    {"&#38;", "&"},
    {"&#038;", "&"},
    {"&apos;", "'"},
    {"&#39;", "'"},
    {"&#039;", "'"},
    {"&lt;", "<"},
    {"&#60;", "<"},
    {"&#060;", "<"},
    {"&gt;", ">"},
    {"&#62;", ">"},
    {"&#062;", ">"},
    {0, 0}
  };

  int i;

  for (i = 0; conv[i].from; ++i)
    src = strepl(src, conv[i].from, conv[i].to);

  return (src);
}

static int new_media_url(_quvi_media_url_t * dst)
{
  struct _quvi_media_url_s *qvl;

  qvl = calloc(1, sizeof(*qvl));
  if (!qvl)
    return (QUVI_MEM);

  *dst = qvl;

  return (QUVI_OK);
}

int add_media_url(_quvi_llst_node_t * lst, const char *fmt, ...)
{
  _quvi_media_url_t qvl;
  va_list args;
  int rc;

  rc = new_media_url(&qvl);
  if (rc != QUVI_OK)
    return (rc);

  va_start(args, fmt);
  vasprintf((char **)&qvl->url, fmt, args);
  va_end(args);

  if (!qvl->url)
    {
      _free(qvl);
      return (QUVI_MEM);
    }

  return (quvi_llst_append((quvi_llst_node_t*)lst, qvl));
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
