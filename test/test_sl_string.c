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
#include "sys/mem_allocator.h"
#include "sys/sys.h"

#define SL_STRING_TYPE SL_STRING_STD
  #include "test_sl_string_def.c"
#undef SL_STRING_TYPE

#define SL_STRING_TYPE SL_STRING_WIDE
  #include "test_sl_string_def.c"
#undef SL_STRING_TYPE

#include "sl_wstring.h"

static void
test_wstring_specific_func()
{
  struct sl_wstring* str = NULL;
  const wchar_t* cstr = NULL;

  CHECK(sl_create_wstring(L"Heo!", NULL, &str), SL_NO_ERROR);

  CHECK(sl_wstring_insert_cstr(NULL, 2, NULL), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_insert_cstr(str, 2, NULL), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_insert_cstr(NULL, 2, "ll"), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_insert_cstr(str, 2, "ll"), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Hello!"), 0);

  CHECK(sl_wstring_insert_cstr(str, 5, " wrld"), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Hello wrld!"), 0);

  CHECK(sl_wstring_insert_wchar(str, 7, (wchar_t)'o'), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Hello world!"), 0);

  CHECK(sl_wstring_insert_cstr(str, 13, "..."), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_insert_cstr(str, 12, "..."), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Hello world!..."), 0);

  CHECK(sl_wstring_insert_cstr(str, 0, "Foo. "), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Foo. Hello world!..."), 0);

  CHECK(sl_wstring_append_cstr(NULL, NULL), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_append_cstr(str, NULL), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_append_cstr(NULL, " abcd"), SL_INVALID_ARGUMENT);
  CHECK(sl_wstring_append_cstr(str, " abcd"), SL_NO_ERROR);
  CHECK(sl_wstring_get(str, &cstr), SL_NO_ERROR);
  CHECK(wcscmp(cstr, L"Foo. Hello world!... abcd"), 0);

  CHECK(sl_free_wstring(str), SL_NO_ERROR);
}

int
main(int argc UNUSED, char** argv UNUSED)
{
  test_string();
  test_wstring();
  test_wstring_specific_func();

  CHECK(MEM_ALLOCATED_SIZE(&mem_default_allocator), 0);
  return 0;
}

