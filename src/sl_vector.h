#ifndef SL_VECTOR_H
#define SL_VECTOR_H

#include "sl.h"
#include "sl_error.h"
#include <stddef.h>

struct mem_allocator;
struct sl_vector;

#ifdef __cplusplus
extern "C" {
#endif

SL_API enum sl_error
sl_create_vector
  (size_t data_size,
   size_t data_alignment,
   struct mem_allocator* allocator, /* May be NULL. */
   struct sl_vector** out_vector);

SL_API enum sl_error
sl_free_vector
  (struct sl_vector* vector);

SL_API enum sl_error
sl_clear_vector
  (struct sl_vector* vector);

SL_API enum sl_error
sl_vector_push_back
  (struct sl_vector* vector,
   const void* data);

SL_API enum sl_error
sl_vector_push_back_n
  (struct sl_vector* vector,
   size_t count,
   const void* data);

SL_API enum sl_error
sl_vector_pop_back
  (struct sl_vector* vector);

SL_API enum sl_error
sl_vector_insert
  (struct sl_vector* vector,
   size_t id,
   const void* data);

SL_API enum sl_error
sl_vector_insert_n
  (struct sl_vector* vector,
   size_t id,
   size_t count,
   const void* data);

SL_API enum sl_error
sl_vector_erase
  (struct sl_vector* vector,
   size_t id);

SL_API enum sl_error
sl_vector_erase_n
  (struct sl_vector* vector,
   size_t id,
   size_t count);

SL_API enum sl_error
sl_vector_resize
  (struct sl_vector* vector,
   size_t size,
   const void* data); /* May be NULL. */

SL_API enum sl_error
sl_vector_reserve
  (struct sl_vector* vector,
   size_t capacity);

SL_API enum sl_error
sl_vector_capacity
  (struct sl_vector* vector,
   size_t* out_capacity);

SL_API enum sl_error
sl_vector_at
  (struct sl_vector* vector,
   size_t id,
   void** out_data);

SL_API enum sl_error
sl_vector_length
  (struct sl_vector* vector,
   size_t* out_length);

SL_API enum sl_error
sl_vector_buffer
  (struct sl_vector* vector,
   size_t* out_length,
   size_t* out_data_size,
   size_t* out_data_alignment,
   void** out_buffer);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* SL_VECTOR_H */

