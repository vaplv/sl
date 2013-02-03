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
#ifndef SL_FLAT_SET_H
#define SL_FLAT_SET_H

#include "sl.h"
#include "sl_error.h"
#include <stddef.h>

struct mem_allocator* allocator;

/* Associative container in which the elements themselves are the key. */
struct sl_flat_set;

#ifdef __cplusplus
extern "C" {
#endif

SL_API enum sl_error
sl_create_flat_set
  (size_t data_size,
   size_t data_alignment,
   int (*data_comparator)(const void*, const void*),
   struct mem_allocator* allocator, /* May be NULL. */
   struct sl_flat_set** set);

SL_API enum sl_error
sl_free_flat_set
  (struct sl_flat_set* set);

SL_API enum sl_error
sl_clear_flat_set
  (struct sl_flat_set* set);

SL_API enum sl_error
sl_flat_set_insert
  (struct sl_flat_set* set,
   const void* data,
   size_t* insert_id); /* May be NULL. */

SL_API enum sl_error
sl_flat_set_erase
  (struct sl_flat_set* set,
   const void* data,
   size_t* erase_id); /* May be NULL. */

SL_API enum sl_error
sl_flat_set_find
  (struct sl_flat_set* set,
   const void* data,
   size_t* out_id); /* set to set_length if the data is not found. */

SL_API enum sl_error
sl_flat_set_at
  (struct sl_flat_set* set,
   size_t id,
   void** data);

SL_API enum sl_error
sl_flat_set_reserve
  (struct sl_flat_set* set,
   size_t capacity);

SL_API enum sl_error
sl_flat_set_capacity
  (struct sl_flat_set* set,
   size_t* out_capacity);

SL_API enum sl_error
sl_flat_set_length
  (struct sl_flat_set* set,
   size_t* out_length);

SL_API enum sl_error
sl_flat_set_lower_bound
  (struct sl_flat_set* set,
   const void* data,
   size_t* lower_bound);

SL_API enum sl_error
sl_flat_set_upper_bound
  (struct sl_flat_set* set,
   const void* data,
   size_t* upper_bound);

SL_API enum sl_error
sl_flat_set_buffer
  (struct sl_flat_set* set,
   size_t* out_length,
   size_t* out_data_size,
   size_t* out_data_alignment,
   void** out_buffer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_FLAT_SET_H */

