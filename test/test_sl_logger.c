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
#include "sl_logger.h"
#include <sys/mem_allocator.h>
#include <assert.h>
#include <string.h>

#define BAD_ARG SL_INVALID_ARGUMENT
#define OK SL_NO_ERROR

STATIC_ASSERT(sizeof(void*) >= 4, Unexpected_pointer_size);

struct data {
  float f;
  char c;
};

static int func_mask = 0;

static void
stream_func0(const char* msg, void* data)
{
  int* i = data;
  func_mask ^= 1;
  CHECK((uintptr_t)i, 0xDEADBEEF);
  CHECK(strcmp(msg, "logger 0"), 0);
}

static void
stream_func1(const char* msg, void* data)
{
  struct data* d = data;
  func_mask ^= 2;
  CHECK(d->f, 3.14159f);
  CHECK(d->c, 'a');
  CHECK(strcmp(msg, "logger 0"), 0);
}

static void
stream_func2(const char* msg, void* data)
{
  func_mask ^= 4;
  CHECK(data, NULL);
  CHECK(strcmp(msg, "logger 0"), 0);
}

static void
vlog(struct sl_logger* logger, const char* fmt, ...)
{
  va_list list;

  assert(logger && fmt);

  va_start(list, fmt);
  func_mask = 0;
  CHECK(sl_logger_vprint(logger, fmt, list), OK);
  CHECK(func_mask, 7);
  va_end(list);
}

int
main(int argc UNUSED, char** argv UNUSED)
{
  struct sl_logger* logger = NULL;
  struct sl_log_stream stream = { NULL, NULL };
  struct data data = { 0, '\0' };

  CHECK(sl_create_logger(NULL, NULL), BAD_ARG);
  CHECK(sl_create_logger(NULL, &logger), OK);

  stream.data = (void*)0xDEADBEEF;
  stream.func = stream_func0;
  CHECK(sl_logger_add_stream(NULL, NULL), BAD_ARG);
  CHECK(sl_logger_add_stream(logger, NULL), BAD_ARG);
  CHECK(sl_logger_add_stream(NULL, &stream), BAD_ARG);
  CHECK(sl_logger_add_stream(logger, &stream), OK);
  CHECK(sl_logger_add_stream(logger, &stream), BAD_ARG);

  CHECK(sl_logger_remove_stream(NULL, NULL), BAD_ARG);
  CHECK(sl_logger_remove_stream(logger, NULL), BAD_ARG);
  CHECK(sl_logger_remove_stream(NULL, &stream), BAD_ARG);
  CHECK(sl_logger_remove_stream(logger, &stream), OK);
  CHECK(sl_logger_remove_stream(logger, &stream), BAD_ARG);

  CHECK(sl_logger_add_stream(logger, &stream), OK);
  data.f = 3.14159f;
  data.c = 'a';
  stream.data = &data;
  stream.func = stream_func1;
  CHECK(sl_logger_add_stream(logger, &stream), OK);
  CHECK(sl_logger_add_stream(logger, &stream), BAD_ARG);
  stream.data = NULL;
  stream.func = stream_func2;
  CHECK(sl_logger_add_stream(logger, &stream), OK);

  CHECK(sl_logger_print(NULL, NULL), BAD_ARG);
  CHECK(sl_logger_print(logger, NULL), BAD_ARG);
  CHECK(sl_logger_print(NULL, "logger 0"), BAD_ARG);
  CHECK(sl_logger_print(logger, "logger 0"), OK);
  CHECK(func_mask, 7);
  CHECK(sl_logger_print(logger, "%c%s %d", 'l', "ogger", 0), OK);
  CHECK(func_mask, 0);
  CHECK(sl_logger_remove_stream(logger, &stream), OK);
  CHECK(sl_logger_print(logger, "logger 0"), OK);
  CHECK(func_mask, 3);
  data.f = 3.14159f;
  data.c = 'a';
  stream.data = &data;
  stream.func = stream_func1;
  CHECK(sl_logger_remove_stream(logger, &stream), OK);
  CHECK(sl_logger_print(logger, "%s%s 0", "log", "ger"), OK);
  CHECK(func_mask, 2);
  stream.data = (void*)0xDEADBEEF;
  stream.func = stream_func0;
  CHECK(sl_logger_remove_stream(logger, &stream), OK);
  CHECK(sl_logger_print(logger, "pouet"), OK);
  CHECK(func_mask, 2);

  CHECK(sl_logger_add_stream(logger, &stream), OK);
  data.f = 3.14159f;
  data.c = 'a';
  stream.data = &data;
  stream.func = stream_func1;
  CHECK(sl_logger_add_stream(logger, &stream), OK);
  CHECK(sl_logger_add_stream(logger, &stream), BAD_ARG);
  stream.data = NULL;
  stream.func = stream_func2;
  CHECK(sl_logger_add_stream(logger, &stream), OK);
  func_mask = 0;
  CHECK(sl_logger_print(logger, "logger 0"), OK);
  CHECK(func_mask, 7);

  vlog(logger, "log%s %d", "ger", 0);

  CHECK(sl_clear_logger(NULL), BAD_ARG);
  CHECK(sl_clear_logger(logger), OK);
  CHECK(sl_logger_print(logger, "Awesom-o"), OK);
  CHECK(func_mask, 7);


  CHECK(sl_free_logger(NULL), BAD_ARG);
  CHECK(sl_free_logger(logger), OK);

  CHECK(MEM_ALLOCATED_SIZE(&mem_default_allocator), 0);

  return 0;
}

