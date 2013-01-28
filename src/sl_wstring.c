/*
 * Copyright (c) 2013 Vincent Forest
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
#define SL_STRING_TYPE SL_STRING_WIDE
  #include "sl_string.c.def"
#undef SL_STRING_TYPE

#include "sl_wstring.h"
#include <stdlib.h>

EXPORT_SYM enum sl_error
sl_wstring_insert_cstr(struct sl_wstring* str, size_t id, const char* cstr)
{
  enum sl_error sl_err = SL_NO_ERROR;

  if(!str
  || !cstr
  || id > str->len
  || IS_MEMORY_OVERLAPPED(str->cstr, str->allocated, cstr, strlen(cstr)+1)) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  if(id == str->len) {
    sl_err = sl_wstring_append_cstr(str, cstr);
    if(sl_err != SL_NO_ERROR)
      goto error;
  } else {
    const size_t len = strlen(cstr);
    sl_err = ensure_allocated(str, len + str->len + 1, true);
    if(sl_err != SL_NO_ERROR)
      goto error;
    memmove
      (str->cstr + id + len,
       str->cstr + id,
       (str->len - id) * sizeof(wchar_t));
    mbstowcs(str->cstr + id, cstr, len);
    str->len = str->len + len;
    str->cstr[str->len] = L'\0';
  }
exit:
  return sl_err;
error:
  goto exit;
}

EXPORT_SYM enum sl_error
sl_wstring_append_cstr(struct sl_wstring* str, const char* cstr)
{
  enum sl_error sl_err = SL_NO_ERROR;
  const size_t len = cstr ? strlen(cstr) : 0;

  if(!str
  || !cstr
  || IS_MEMORY_OVERLAPPED(str->cstr, str->allocated, cstr, len + 1)) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  sl_err = ensure_allocated(str, len + str->len + 1, true);
  if(sl_err != SL_NO_ERROR)
    goto error;
  mbstowcs(str->cstr + str->len, cstr, len);
  str->len = str->len + len;
  str->cstr[str->len] = L'\0';

exit:
  return sl_err;
error:
  goto exit;
}
