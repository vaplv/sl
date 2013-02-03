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
#ifndef SL_LOGGER_H
#define SL_LOGGER_H

#include "sl.h"
#include "sl_error.h"
#include <sys/sys.h>
#include <stdarg.h>

struct sl_log_stream {
  void (*func)(const char* msg, void* data);
  void* data;
};

struct mem_allocator;
struct sl_logger;

#ifdef __cplusplus
extern "C" {
#endif

SL_API enum sl_error
sl_create_logger
  (struct mem_allocator* allocator, /* May be NULL. */
   struct sl_logger** out_logger);

SL_API enum sl_error
sl_free_logger
  (struct sl_logger* sl_logger);

SL_API enum sl_error
sl_logger_add_stream
  (struct sl_logger* logger,
   struct sl_log_stream* stream);

SL_API enum sl_error
sl_logger_remove_stream
  (struct sl_logger* logger,
   struct sl_log_stream* stream);

SL_API enum sl_error
sl_clear_logger
  (struct sl_logger* logger);

SL_API enum sl_error
sl_logger_print
  (struct sl_logger* logger,
   const char* fmt,
   ...) FORMAT_PRINTF(2, 3);

SL_API enum sl_error
sl_logger_vprint
  (struct sl_logger* logger,
   const char* fmt,
   va_list list);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_LOGGER_H */
