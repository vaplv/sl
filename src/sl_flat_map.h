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
#ifndef SL_FLAT_MAP
#define SL_FLAT_MAP

#include "sl.h"
#include "sl_error.h"
#include <stddef.h>

struct mem_allocator;
struct sl_flat_map;
struct sl_pair;

SL_API enum sl_error
sl_create_flat_map
  (size_t key_size,
   size_t key_alignment,
   size_t data_size,
   size_t data_alignment,
   int (*cmp_key)(const void*, const void*),
   struct mem_allocator* allocator, /* May be NULL. */
   struct sl_flat_map** out_map);

SL_API enum sl_error
sl_free_flat_map
  (struct sl_flat_map* map);

SL_API enum sl_error
sl_clear_flat_map
  (struct sl_flat_map* map);

SL_API enum sl_error
sl_flat_map_insert
  (struct sl_flat_map* map,
   const void* key,
   const void* data,
   size_t* insert_id); /* May be NULL. */

SL_API enum sl_error
sl_flat_map_erase
  (struct sl_flat_map* map,
   const void* key,
   size_t* erase_id); /* May be NULL. */

SL_API enum sl_error
sl_flat_map_find
  (struct sl_flat_map* map,
   const void* key,
   void** data);

SL_API enum sl_error
sl_flat_map_find_pair
  (struct sl_flat_map* map,
   const void* key,
   struct sl_pair* pair);

SL_API enum sl_error
sl_flat_map_clear
  (struct sl_flat_map* map);

SL_API enum sl_error
sl_flat_map_lower_bound
  (struct sl_flat_map* map,
   const void* key,
   size_t* lower_bound);

SL_API enum sl_error
sl_flat_map_upper_bound
  (struct sl_flat_map* map,
   const void* key,
   size_t* upper_bound);

SL_API enum sl_error
sl_flat_map_length
  (struct sl_flat_map* map,
   size_t* out_length);

SL_API enum sl_error
sl_flat_map_at
  (struct sl_flat_map* map,
   size_t at,
   struct sl_pair* pair);

SL_API enum sl_error
sl_flat_map_key_buffer
  (struct sl_flat_map* map,
   size_t* length, /* May be NULL. */
   size_t* key_size, /* May be NULL. */
   size_t* key_alignment, /* May be NULL. */
   void** key_buffer); /* May be NULL. */

SL_API enum sl_error
sl_flat_map_data_buffer
  (struct sl_flat_map* map,
   size_t* length, /* May be NULL. */
   size_t* data_size, /* May be NULL. */
   size_t* data_alignment, /* May be NULL. */
   void** data_buffer); /* May be NULL. */

#endif /* SL_FLAT_MAP */

