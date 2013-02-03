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
#include "sl.h"
#include "sl_flat_map.h"
#include "sl_flat_set.h"
#include "sl_pair.h"
#include "sl_vector.h"
#include <sys/math.h>
#include <sys/mem_allocator.h>
#include <sys/sys.h>
#include <stdbool.h>
#include <stdlib.h>

struct sl_flat_map {
  struct sl_flat_set* key_set;
  struct sl_vector* data_list;
  struct mem_allocator* allocator;
};

/*******************************************************************************
 *
 * Flat map functions.
 *
 ******************************************************************************/
EXPORT_SYM enum sl_error
sl_create_flat_map
  (size_t key_size,
   size_t key_alignment,
   size_t data_size,
   size_t data_alignment,
   int (*cmp_key)(const void*, const void*),
   struct mem_allocator* specific_allocator,
   struct sl_flat_map** out_map)
{
  struct mem_allocator* allocator = NULL;
  struct sl_flat_map* map = NULL;
  enum sl_error err = SL_NO_ERROR;

  if(!data_size
  || !key_size
  || !cmp_key
  || !out_map) {
    err = SL_INVALID_ARGUMENT;
    goto error;
  }
  if(!IS_POWER_OF_2(data_alignment) || !IS_POWER_OF_2(key_alignment)) {
    err = SL_ALIGNMENT_ERROR;
    goto error;
  }
  allocator = specific_allocator ? specific_allocator : &mem_default_allocator;
  map = MEM_CALLOC(allocator, 1, sizeof(struct sl_flat_map));
  if(map == NULL) {
    err = SL_MEMORY_ERROR;
    goto error;
  }
  err = sl_create_flat_set
    (key_size, key_alignment, cmp_key, allocator, &map->key_set);
  if(err != SL_NO_ERROR)
    goto error;
  err = sl_create_vector(data_size, data_alignment, allocator, &map->data_list);
  if(err != SL_NO_ERROR)
    goto error;
  map->allocator = allocator;

exit:
  if(out_map)
    *out_map = map;
  return err;
error:
  if(map) {
    if(map->key_set)
      SL(free_flat_set(map->key_set));
    if(map->data_list)
      SL(free_vector(map->data_list));
    MEM_FREE(allocator, map);
    map = NULL;
  }
  goto exit;
}

EXPORT_SYM enum sl_error
sl_free_flat_map(struct sl_flat_map* map)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  SL(free_flat_set(map->key_set));
  SL(free_vector(map->data_list));
  MEM_FREE(map->allocator, map);
  return SL_NO_ERROR;
}

EXPORT_SYM enum sl_error
sl_flat_map_insert
  (struct sl_flat_map* map,
   const void* key,
   const void* data,
   size_t* insert_id)
{
  size_t id = 0;
  enum sl_error sl_err = SL_NO_ERROR;
  bool is_key_inserted = false;

  if(!map || !key|| !data) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  sl_err = sl_flat_set_insert(map->key_set, key, &id);
  if(sl_err != SL_NO_ERROR)
    goto error;
  is_key_inserted = true;
  sl_err = sl_vector_insert(map->data_list, id, data);
  if(sl_err != SL_NO_ERROR)
    goto error;
  if(insert_id)
    *insert_id = id;

exit:
  return sl_err;
error:
  if(is_key_inserted)
    SL(flat_set_erase(map->key_set, key, NULL));
  goto exit;
}

EXPORT_SYM enum sl_error
sl_flat_map_erase(struct sl_flat_map* map, const void* key, size_t* erase_id)
{
  size_t id = 0;
  enum sl_error sl_err = SL_NO_ERROR;
  bool is_key_erased = false;

  if(!map || !key) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  sl_err = sl_flat_set_erase(map->key_set, key, &id);
  if(sl_err != SL_NO_ERROR)
    goto error;
  is_key_erased = true;
  sl_err = sl_vector_erase(map->data_list, id);
  if(sl_err != SL_NO_ERROR)
    goto error;
  if(erase_id)
    *erase_id = id;

exit:
  return sl_err;
error:
  if(is_key_erased)
    SL(flat_set_insert(map->key_set, key, NULL));
  goto exit;
}

EXPORT_SYM enum sl_error
sl_flat_map_find(struct sl_flat_map* map, const void* key, void** data)
{
  size_t len = 0;
  size_t id = 0;
  enum sl_error sl_err = SL_NO_ERROR;

  if(!map || !data) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  sl_err = sl_flat_set_find(map->key_set, key, &id);
  if(sl_err != SL_NO_ERROR)
    goto error;
  SL(flat_set_length(map->key_set, &len));
  if(id == len) {
    *data = NULL;
  } else {
    SL(vector_at(map->data_list, id, data));
  }

exit:
  return sl_err;
error:
  goto exit;
}

EXPORT_SYM enum sl_error
sl_flat_map_find_pair
  (struct sl_flat_map* map, 
   const void* key, 
   struct sl_pair* pair)
{
  size_t id = 0;
  size_t len = 0;
  enum sl_error sl_err = SL_NO_ERROR;

  if(!map || !key || !pair) {
    sl_err = SL_INVALID_ARGUMENT;
    goto error;
  }
  sl_err = sl_flat_set_find(map->key_set, key, &id);
  if(sl_err != SL_NO_ERROR)
    goto error;
  SL(flat_set_length(map->key_set, &len));
  if(id == len) {
    pair->key = NULL;
    pair->data = NULL;
  } else {
    SL(vector_at(map->data_list, id, &(pair->data)));
    SL(flat_set_at(map->key_set, id, &(pair->key)));
  }
exit:
  return sl_err;
error:
  goto exit;
}

EXPORT_SYM enum sl_error
sl_clear_flat_map(struct sl_flat_map* map)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  SL(clear_flat_set(map->key_set));
  SL(clear_vector(map->data_list));
  return SL_NO_ERROR;
}

EXPORT_SYM enum sl_error
sl_flat_map_lower_bound
  (struct sl_flat_map* map,
   const void* key,
   size_t* lower_bound)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  return sl_flat_set_lower_bound(map->key_set, key, lower_bound);
}

EXPORT_SYM enum sl_error
sl_flat_map_upper_bound
  (struct sl_flat_map* map,
   const void* key,
   size_t* upper_bound)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  return sl_flat_set_upper_bound(map->key_set, key, upper_bound);
}

EXPORT_SYM enum sl_error
sl_flat_map_length(struct sl_flat_map* map, size_t* out_length)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  return sl_vector_length(map->data_list, out_length);
}

EXPORT_SYM enum sl_error
sl_flat_map_at(struct sl_flat_map* map, size_t at, struct sl_pair* pair)
{
  size_t len = 0;

  if(!map || !pair)
    return SL_INVALID_ARGUMENT;
  SL(vector_length(map->data_list, &len));
  if(at >= len)
    return SL_INVALID_ARGUMENT;
  SL(flat_set_at(map->key_set, at, &pair->key));
  SL(vector_at(map->data_list, at, &pair->data));
  return SL_NO_ERROR;
}

EXPORT_SYM enum sl_error
sl_flat_map_key_buffer
  (struct sl_flat_map* map,
   size_t* length,
   size_t* key_size,
   size_t* key_alignment,
   void** key_buffer)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  return sl_flat_set_buffer
    (map->key_set, length, key_size, key_alignment, key_buffer);
}

EXPORT_SYM enum sl_error
sl_flat_map_data_buffer
  (struct sl_flat_map* map,
   size_t* length,
   size_t* data_size,
   size_t* data_alignment,
   void** data_buffer)
{
  if(!map)
    return SL_INVALID_ARGUMENT;
  return sl_vector_buffer
    (map->data_list, length, data_size, data_alignment, data_buffer);
}

