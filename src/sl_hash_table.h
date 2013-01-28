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
#ifndef SL_HASH_TABLE_H
#define SL_HASH_TABLE_H

#include "sl.h"
#include "sl_error.h"
#include "sl_pair.h"
#include <stdbool.h>
#include <stddef.h>

struct mem_allocator;
struct sl_hash_table;

struct sl_hash_table_it {
  struct sl_hash_table* hash_table;
  struct sl_pair pair;
  /*Private data. */
  struct entry* entry;
  size_t bucket;
};

SL_API enum sl_error
sl_create_hash_table
  (size_t key_size,
   size_t key_alignment,
   size_t data_size,
   size_t data_alignment,
   size_t (*hash_fcn)(const void*),
   bool (*eq_key)(const void*, const void*),
   struct mem_allocator* allocator, /* May be NULL. */
   struct sl_hash_table** out_hash_table);

SL_API enum sl_error
sl_free_hash_table
  (struct sl_hash_table* hash_table);

SL_API enum sl_error
sl_hash_table_insert
  (struct sl_hash_table* hash_table,
   const void* key,
   const void* data);

SL_API enum sl_error
sl_hash_table_erase
  (struct sl_hash_table* hash_table,
   const void* key,
   size_t* out_nb_erased); /* May be NULL. */

SL_API enum sl_error
sl_hash_table_find
  (struct sl_hash_table* hash_table,
   const void* key,
   void** data);

SL_API enum sl_error
sl_hash_table_find_pair
  (struct sl_hash_table* hash_table,
   const void* key,
   struct sl_pair* pair);

SL_API enum sl_error
sl_hash_table_data_count
  (struct sl_hash_table* hash_table,
   size_t *nb_data);

SL_API enum sl_error
sl_hash_table_resize
  (struct sl_hash_table* hash_table,
   size_t hint_nb_buckets);

SL_API enum sl_error
sl_hash_table_bucket_count
  (const struct sl_hash_table* hash_table,
   size_t* nb_buckets);

SL_API enum sl_error
sl_hash_table_used_bucket_count
  (const struct sl_hash_table* hash_table,
   size_t* nb_used_buckets);

SL_API enum sl_error
sl_hash_table_clear
  (struct sl_hash_table* hash_table);

SL_API enum sl_error
sl_hash_table_begin
  (struct sl_hash_table* hash_table,
   struct sl_hash_table_it* it,
   bool* is_end_reached);

SL_API enum sl_error
sl_hash_table_it_next
  (struct sl_hash_table_it* it,
   bool* is_end_reached);

/* Generic hash function. */
SL_API size_t
sl_hash
  (const void* data,
   size_t len);

#endif /* SL_HASH_TABLE_H */

