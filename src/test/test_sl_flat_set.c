#include "../sl_flat_set.h"
#include <snlsys/mem_allocator.h>
#include <snlsys/snlsys.h>
#include <stdbool.h>
#include <stdlib.h>

/* Shortened expressions. */
#define SZ sizeof
#define AL ALIGNOF
#define BAD_ARG SL_INVALID_ARGUMENT
#define BAD_ALIGN SL_ALIGNMENT_ERROR
#define OK SL_NO_ERROR

static int
cmp(const void* i0, const void* i1)
{
  int a, b;
  ASSERT(i0 && i1);

  a = *(int*)i0;
  b = *(int*)i1;

  if(a < b)
    return -1;
  else if(a > b)
    return 1;
  else
    return 0;
}

int
main(int argc UNUSED, char** argv UNUSED)
{
  ALIGN(16) int array[4];
  struct sl_flat_set* vec = NULL;
  void* buffer = NULL;
  void* data = NULL;
  size_t capacity = 0;
  size_t capacity2 = 0;
  size_t id = 0;
  size_t len = 0;
  size_t sz = 0;
  size_t al = 0;

  CHECK(sl_create_flat_set(0, 0, NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), 0, NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(0, AL(int), NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), AL(int), NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(0, 0, cmp, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), 0, cmp, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(0, AL(int), cmp, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), AL(int), cmp, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(0, 0, NULL, NULL, &vec), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), 0, NULL, NULL, &vec), BAD_ARG);
  CHECK(sl_create_flat_set(0, AL(int), NULL, NULL, &vec), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), AL(int), NULL, NULL, &vec), BAD_ARG);
  CHECK(sl_create_flat_set(0, 0, cmp, NULL, NULL), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), 0, cmp, NULL, &vec), BAD_ALIGN);
  CHECK(sl_create_flat_set(0, AL(int), cmp, NULL, &vec), BAD_ARG);
  CHECK(sl_create_flat_set(SZ(int), AL(int), cmp, NULL, &vec), OK);

  CHECK(sl_flat_set_insert(NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_insert(NULL, (int[]){9}, NULL), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){9}, NULL), OK);

  CHECK(sl_flat_set_buffer(NULL, NULL, NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_buffer(vec, NULL, NULL, NULL, NULL), OK);
  CHECK(sl_flat_set_buffer(NULL, &len, NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, NULL), OK);
  CHECK(len, 1);
  CHECK(sl_flat_set_buffer(NULL, NULL, &sz, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_buffer(vec, &len, &sz, NULL, NULL), OK);
  CHECK(len, 1);
  CHECK(sz, SZ(int));
  CHECK(sl_flat_set_buffer(vec, NULL, &sz, NULL, NULL), OK);
  CHECK(sz, SZ(int));
  CHECK(sl_flat_set_buffer(NULL, NULL, NULL, &al, NULL), BAD_ARG);
  CHECK(sl_flat_set_buffer(vec, NULL, NULL, &al, NULL), OK);
  CHECK(al, AL(int));
  CHECK(sl_flat_set_buffer(vec, &len, NULL, &al, NULL), OK);
  CHECK(len, 1);
  CHECK(al, AL(int));
  CHECK(sl_flat_set_buffer(vec, NULL, &sz, &al, NULL), OK);
  CHECK(sz, SZ(int));
  CHECK(al, AL(int));
  CHECK(sl_flat_set_buffer(vec, &len, &sz, &al, NULL), OK);
  CHECK(len, 1);
  CHECK(sz, SZ(int));
  CHECK(al, AL(int));
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 1);
  CHECK(((int*)buffer)[0], 9);

  CHECK(sl_flat_set_insert(vec, (int[]){9}, &id), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){0}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_insert(vec, (int[]){-1}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_insert(vec, (int[]){5}, &id), OK);
  CHECK(id, 2);
  CHECK(sl_flat_set_insert(vec, (int[]){3}, &id), OK);
  CHECK(id, 2);
  CHECK(sl_flat_set_insert(vec, (int[]){2}, &id), OK);
  CHECK(id, 2);
  CHECK(sl_flat_set_insert(vec, (int[]){0}, &id), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){7}, &id), OK);
  CHECK(id, 5);
  CHECK(sl_flat_set_insert(vec, (int[]){5}, &id), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){1}, &id), OK);
  CHECK(id, 2);
  CHECK(sl_flat_set_insert(vec, (int[]){8}, &id), OK);
  CHECK(id, 7);
  CHECK(sl_flat_set_insert(vec, (int[]){6}, &id), OK);
  CHECK(id, 6);
  CHECK(sl_flat_set_insert(vec, (int[]){8}, &id), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){-1},&id), BAD_ARG);
  CHECK(sl_flat_set_insert(vec, (int[]){4}, &id), OK);
  CHECK(id, 5);
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 11);
  CHECK(((int*)buffer)[0], -1);
  CHECK(((int*)buffer)[1], 0);
  CHECK(((int*)buffer)[2], 1);
  CHECK(((int*)buffer)[3], 2);
  CHECK(((int*)buffer)[4], 3);
  CHECK(((int*)buffer)[5], 4);
  CHECK(((int*)buffer)[6], 5);
  CHECK(((int*)buffer)[7], 6);
  CHECK(((int*)buffer)[8], 7);
  CHECK(((int*)buffer)[9], 8);
  CHECK(((int*)buffer)[10], 9);

  CHECK(sl_flat_set_lower_bound(NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(vec, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(NULL, (int[]){2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(NULL, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(vec, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(NULL, (int[]){2}, &id), BAD_ARG);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){2}, &id), OK);
  CHECK(id, 3);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){-3}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){7}, &id), OK);
  CHECK(id, 8);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){-1}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){9}, &id), OK);
  CHECK(id, 10);
  CHECK(sl_flat_set_lower_bound(vec, (int[]){10}, &id), OK);
  CHECK(id, 11);

  CHECK(sl_flat_set_upper_bound(NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(vec, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(NULL, (int[]){2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(NULL, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(vec, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(NULL, (int[]){2}, &id), BAD_ARG);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){2}, &id), OK);
  CHECK(id, 4);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){-3}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){7}, &id), OK);
  CHECK(id, 9);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){-1}, &id), OK);
  CHECK(id, 1);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){9}, &id), OK);
  CHECK(id, 11);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){10}, &id), OK);
  CHECK(id, 11);

  CHECK(sl_flat_set_length(NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_length(vec, NULL), BAD_ARG);
  CHECK(sl_flat_set_length(NULL, &len), BAD_ARG);
  CHECK(sl_flat_set_length(vec, &len), OK);
  CHECK(len, 11);
  CHECK(sl_flat_set_insert(vec, (int[]){-5}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_length(vec, &len), OK);
  CHECK(len, 12);
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 12);
  CHECK(((int*)buffer)[0], -5);
  CHECK(((int*)buffer)[1], -1);
  CHECK(((int*)buffer)[2], 0);
  CHECK(((int*)buffer)[3], 1);
  CHECK(((int*)buffer)[4], 2);
  CHECK(((int*)buffer)[5], 3);
  CHECK(((int*)buffer)[6], 4);
  CHECK(((int*)buffer)[7], 5);
  CHECK(((int*)buffer)[8], 6);
  CHECK(((int*)buffer)[9], 7);
  CHECK(((int*)buffer)[10], 8);
  CHECK(((int*)buffer)[11], 9);

  CHECK(sl_flat_set_find(NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_find(vec, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_find(NULL, (int[]){-2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_find(vec, (int[]){-2}, NULL), BAD_ARG);
  CHECK(sl_flat_set_find(NULL, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_find(vec, NULL, &id), BAD_ARG);
  CHECK(sl_flat_set_find(NULL, (int[]){-2}, &id), BAD_ARG);
  CHECK(sl_flat_set_find(vec, (int[]){-2}, &id), OK);
  CHECK(id, len);
  CHECK(sl_flat_set_find(vec, (int[]){-1}, &id), OK);
  CHECK(id, 1);
  CHECK(sl_flat_set_find(vec, (int[]){7}, &id), OK);
  CHECK(id, 9);
  CHECK(sl_flat_set_find(vec, (int[]){-5}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_find(vec, (int[]){9}, &id), OK);
  CHECK(id, 11);
  CHECK(sl_flat_set_find(vec, (int[]){0}, &id), OK);
  CHECK(id, 2);
  CHECK(sl_flat_set_find(vec, (int[]){33}, &id), OK);
  CHECK(id, len);

  CHECK(sl_flat_set_at(NULL, SIZE_MAX, NULL), BAD_ARG);
  CHECK(sl_flat_set_at(vec, SIZE_MAX, NULL), BAD_ARG);
  CHECK(sl_flat_set_at(NULL, 0, NULL), BAD_ARG);
  CHECK(sl_flat_set_at(vec, 0, NULL), BAD_ARG);
  CHECK(sl_flat_set_at(NULL, SIZE_MAX, &data), BAD_ARG);
  CHECK(sl_flat_set_at(vec, SIZE_MAX, &data), BAD_ARG);
  CHECK(sl_flat_set_at(NULL, 0, &data), BAD_ARG);

  for(id = 0; id < len; ++id) {
    CHECK(sl_flat_set_at(vec, id, &data), OK);
    CHECK(((int*)buffer)[id], *(int*)data);
  }

  CHECK(sl_flat_set_erase(NULL, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_erase(vec, NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_erase(NULL, (int[]){-3}, NULL), BAD_ARG);
  CHECK(sl_flat_set_erase(vec, (int[]){-3}, NULL), BAD_ARG);
  CHECK(sl_flat_set_erase(vec, (int[]){7}, NULL), OK);

  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 11);
  CHECK(((int*)buffer)[0], -5);
  CHECK(((int*)buffer)[1], -1);
  CHECK(((int*)buffer)[2], 0);
  CHECK(((int*)buffer)[3], 1);
  CHECK(((int*)buffer)[4], 2);
  CHECK(((int*)buffer)[5], 3);
  CHECK(((int*)buffer)[6], 4);
  CHECK(((int*)buffer)[7], 5);
  CHECK(((int*)buffer)[8], 6);
  CHECK(((int*)buffer)[9], 8);
  CHECK(((int*)buffer)[10], 9);

  CHECK(sl_flat_set_lower_bound(vec, (int[]){-3}, &id), OK);
  CHECK(id, 1);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){-3}, &id), OK);
  CHECK(id, 1);

  CHECK(sl_flat_set_erase(vec, (int[]){-5}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_erase(vec, (int[]){-1}, &id), OK);
  CHECK(id, 0);
  CHECK(sl_flat_set_erase(vec, (int[]){9}, &id), OK);
  CHECK(id, 8);
  CHECK(sl_flat_set_erase(vec, (int[]){3}, &id), OK);
  CHECK(id, 3);
  CHECK(sl_flat_set_erase(vec, (int[]){3}, &id), BAD_ARG);
  CHECK(sl_flat_set_erase(vec, (int[]){6}, &id), OK);
  CHECK(id, 5);
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 6);
  CHECK(((int*)buffer)[0], 0);
  CHECK(((int*)buffer)[1], 1);
  CHECK(((int*)buffer)[2], 2);
  CHECK(((int*)buffer)[3], 4);
  CHECK(((int*)buffer)[4], 5);
  CHECK(((int*)buffer)[5], 8);

  CHECK(sl_flat_set_lower_bound(vec, (int[]){3}, &id), OK);
  CHECK(id, 3);
  CHECK(sl_flat_set_upper_bound(vec, (int[]){6}, &id), OK);
  CHECK(id, 5);

  CHECK(sl_flat_set_capacity(NULL, NULL), BAD_ARG);
  CHECK(sl_flat_set_capacity(vec, NULL), BAD_ARG);
  CHECK(sl_flat_set_capacity(NULL, &capacity), BAD_ARG);
  CHECK(sl_flat_set_capacity(vec, &capacity), OK);
  CHECK(capacity >= len, true);

  CHECK(sl_flat_set_reserve(NULL, 0), BAD_ARG);
  CHECK(sl_flat_set_reserve(vec, 0), OK);
  CHECK(sl_flat_set_capacity(vec, &capacity), OK);
  NCHECK(capacity, 0);
  CHECK(sl_flat_set_reserve(vec, 2 * capacity), OK);
  CHECK(sl_flat_set_capacity(vec, &capacity2), OK);
  CHECK(capacity2, 2 * capacity);
  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  CHECK(len, 6);
  CHECK(((int*)buffer)[0], 0);
  CHECK(((int*)buffer)[1], 1);
  CHECK(((int*)buffer)[2], 2);
  CHECK(((int*)buffer)[3], 4);
  CHECK(((int*)buffer)[4], 5);
  CHECK(((int*)buffer)[5], 8);

  CHECK(sl_flat_set_capacity(vec, &capacity), OK);
  for(id = 9; len != capacity; ++id) {
    CHECK(sl_flat_set_insert(vec, &id, NULL), OK);
    CHECK(sl_flat_set_length(vec, &len), OK);
  }

  CHECK(sl_flat_set_capacity(vec, &capacity2), OK);
  CHECK(capacity2, capacity);
  CHECK(sl_flat_set_insert(vec, &id, NULL), OK);
  CHECK(sl_flat_set_capacity(vec, &capacity2), OK);
  CHECK(capacity2, 2 * capacity);

  CHECK(sl_flat_set_buffer(vec, &len, NULL, NULL, &buffer), OK);
  for(id = 1; id < len; ++id) {
    CHECK(((int*)buffer)[id - 1] < ((int*)buffer)[id], true);
  }

  CHECK(sl_flat_set_capacity(vec, &capacity), OK);
  CHECK(sl_clear_flat_set(NULL), BAD_ARG);
  CHECK(sl_clear_flat_set(vec), OK);
  CHECK(sl_flat_set_length(vec, &len), OK);
  CHECK(len, 0);
  CHECK(sl_flat_set_buffer(vec, &len, &sz, &al, &buffer), OK);
  CHECK(len, 0);
  CHECK(sz, SZ(int));
  CHECK(al, AL(int));
  CHECK(sl_flat_set_capacity(vec, &capacity2), OK);
  CHECK(capacity, capacity2);

  CHECK(sl_free_flat_set(NULL), BAD_ARG);
  CHECK(sl_free_flat_set(vec), OK);

  CHECK(sl_create_flat_set
        (SZ(int), 16, cmp, &mem_default_allocator, &vec), OK);
  array[0] = 0;
  array[1] = 1;
  array[2] = 2;
  array[3] = 3;
  CHECK(sl_flat_set_insert(vec, &array[0], NULL), OK);
  CHECK(sl_flat_set_insert(vec, &array[1], NULL), BAD_ALIGN);
  CHECK(sl_free_flat_set(vec), OK);

  CHECK(MEM_ALLOCATED_SIZE(&mem_default_allocator), 0);

  return 0;
}

