/*
* Copyright (C) 2009 Toni Gundogdu.
* Copyright (C) Laird Shaw.
*
* Public domain.
*
* http://creativeandcritical.net/sites/default/files/replacebench.c
*/

/*
* Changes to the original code:
*   - This revision _releases_ the source string
*   - Rename "replace_opt" -> "strepl"
*   - Minor style changes
*/

#include "config.h"

#include <stdlib.h>
#include <stddef.h>
#include <string.h>

char *strepl(const char *str, const char *old, const char *new)
{
  size_t oldlen, count, retlen, newlen;
  const char *p, *q;
  char *ret, *r;

  oldlen = strlen(old);
  newlen = strlen(new);

  if (oldlen != newlen)
    {
      for (count = 0, p = str; (q = strstr(p, old)) != 0; p = q + oldlen)
        count++;
      retlen = p - str + strlen(p) + count * (newlen - oldlen);
    }
  else
    retlen = strlen(str);

  if ((ret = malloc(retlen + 1)) == 0)
    return 0;

  for (r = ret, p = str; (q = strstr(p, old)) != 0; p = q + oldlen)
    {
#ifdef HAVE_PTRDIFF_T
      ptrdiff_t l = q - p;
#else
      long l = q - p;
#endif
      memcpy(r, p, l);
      r += l;
      memcpy(r, new, newlen);
      r += newlen;
    }
  strcpy(r, p);

  /* See the notes above. */
  free((void *)str);

  return (ret);
}

/* vim: set ts=2 sw=2 tw=72 expandtab: */
