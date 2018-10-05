#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "array.h"

/* #define assert_array_equal(type, val1, val2, size) \ */
/*   for (int i = 0; i < size; i++) \ */
/*     if (strcmp(#type, "char*") == 0) \ */
/*       assert_string_equal(((type) val1)[i], ((type) val2)[i]); \ */
/*     else \ */
/*       assert_memory_equal(((type) val1)[i], ((type) val2)[i], sizeof(type)) */
#define TEST(test) puts("... " test)

int _test_cast_function(void* element) {
  char* temp = (char*) element;
  temp = "string";
}
void test_clib_new_Array(void** state) {
  /* make an array of ints with defaults */
  Array* array = clib_new_Array(int);
  int* raw_array = (int*) array->_data;
  assert_int_equal(0, array->_length);
  assert_int_equal(8, array->_size);
  assert_int_equal(sizeof(int), array->_type_size);
  assert_ptr_equal(array->_cast,
                   _clib_void_ptr_cast);
  assert_memory_equal(((int[]){0,0,0,0,0,0,0,0}), raw_array, 8*sizeof(int));

  Array* array2 =
    clib_new_Array(char*,
                   .initial_size=12,
                   .element_cast_function=_test_cast_function);
  char** raw_array2 = (char**) array2->_data;
  assert_int_equal(0, array2->_length);
  assert_int_equal(12, array2->_size);
  assert_int_equal(sizeof(char*), array2->_type_size);
  assert_ptr_equal(array2->_cast,
                   _test_cast_function);
  assert_memory_equal(((int[]){0,0,0,0,0,0,0,0,0,0,0,0}), raw_array2, 12*sizeof(int));
}
void test_clib_Array__resize(void** state) {
  Array* array =
    clib_new_Array(char*,
                   .initial_size=12,
                   .element_cast_function=_test_cast_function);

  array->_resize(array, 1);
  assert_int_equal(1, array->_size);
}
void test_clib_Array_insert(void** state) {
  Array* array = clib_new_Array(int, .is_primitive_typed=true);
  // inserting into non-available spot
  int err = array->insert(array, 1, 10);
  assert_int_equal(-1, err);

  err = array->insert(array, 0, 10);
  assert_int_equal(0, err);
  assert_int_equal(1, array->_length);
  assert_memory_equal(((int[]){10,0,0,0,0,0,0,0}), array->_data, array->_length*sizeof(int));

  err = array->insert(array, 0, 11);
  assert_int_equal(0, err);
  assert_int_equal(2, array->_length);
  assert_memory_equal(((int[]){11,10,0,0,0,0,0,0}), array->_data, array->_length*sizeof(int));

  err = array->insert(array, 2, 12);
  assert_int_equal(0, err);
  assert_int_equal(3, array->_length);
  assert_memory_equal(((int[]){11,10,12,0,0,0,0,0}), array->_data, array->_length*sizeof(int));
  //TODO array.free

  // test for proper working of primitive type smaller than intptr_t
  array = clib_new_Array(uint8_t, .is_primitive_typed=true);
  err = array->insert(array, 0, 10);
  assert_int_equal(0, err);
  assert_int_equal(1, array->_length);
  assert_memory_equal(((uint8_t[]){10,0,0,0,0,0,0,0}), array->_data, array->_length*sizeof(uint8_t));

  // test for proper working of primitive type smaller than intptr_t
  array = clib_new_Array(char*);
  err = array->insert(array, 0, "Hello");
  assert_int_equal(0, err);
  assert_int_equal(1, array->_length);

  err = array->insert(array, 1, "World");
  assert_int_equal(0, err);
  assert_int_equal(2, array->_length);

  char* temp[] = {"Hello","World",0,0,0,0,0,0};
  /* assert_array_equal(char**, temp, array->_data, array->_size); */
  /* assert_string_equal(temp[0], array->_data); */
  for (int i = 0; i < array->_length; i++)
    assert_string_equal(((char**) temp)[i], (char*)((char**) array->_data + i));

  // TODO Resize test
  array = clib_new_Array(char*, .initial_size=1);
  err = array->insert(array, 0, "Hello");
  assert_int_equal(1, array->_length);
  assert_int_equal(1, array->_size);
  err = array->insert(array, 0, "Hello");
  assert_int_equal(2, array->_length);
  assert_int_equal(6, array->_size);
}
void test_clib_Array_get(void** state) {
  TEST("Primitive typed array get");
  Array* array = clib_new_Array(int, .is_primitive_typed=true);
  void* element_vptr;
  // inserting into non-available spot
  element_vptr = array->get(array, 1);
  assert_int_equal(NULL, element_vptr);
  element_vptr = array->get(array, -1);
  assert_int_equal(NULL, element_vptr);

  array->append(array, 10);
  array->append(array, 11);
  assert_int_equal(10, (int) array->get(array, 0));
  assert_int_equal(11, (int) array->get(array, 1));

  TEST("Primitive typed array get (less than size intptr)");
  array = clib_new_Array(uint8_t, .is_primitive_typed=true);
  // inserting into non-available spot
  element_vptr = array->get(array, 1);
  assert_int_equal(NULL, element_vptr);
  element_vptr = array->get(array, -1);
  assert_int_equal(NULL, element_vptr);

  array->append(array, 10);
  array->append(array, 11);
  assert_int_equal(10, (uint8_t) array->get(array, 0));
  assert_int_equal(11, (uint8_t) array->get(array, 1));

  TEST("Non-primitive typed array get");
  array = clib_new_Array(char*);
  // inserting into non-available spot
  element_vptr = array->get(array, 1);
  assert_int_equal(NULL, element_vptr);
  element_vptr = array->get(array, -1);
  assert_int_equal(NULL, element_vptr);

  array->append(array, "Hello");
  array->append(array, "World");
  assert_string_equal("Hello", (char*) array->get(array, 0));
  assert_string_equal("World", (char*) array->get(array, 1));
}
void test_clib_Array_concat(void** state) {
  TEST("Concat with large enough size to hold result");
  Array* array = clib_new_Array(char*, .initial_size=8);
  int err = array->insert(array, 0, "Hello");
  err = array->insert(array, 1, "World");

  Array* array2 = clib_new_Array(char*);
  err = array->insert(array2, 0, "World");
  err = array->insert(array2, 1, "Hello");

  array->concat(array, array2);
  assert_int_equal(4, array->_length);
  assert_int_equal(8, array->_size);
  assert_string_equal("Hello", (char*) array->get(array, 0));
  assert_string_equal("World", (char*) array->get(array, 1));
  assert_string_equal("World", (char*) array->get(array, 2));
  assert_string_equal("Hello", (char*) array->get(array, 3));
  assert_string_equal("World", (char*) array->get(array2, 0));
  assert_string_equal("Hello", (char*) array->get(array2, 1));

  TEST("Concat with necessary expansion to hold result");
  array = clib_new_Array(char*, .initial_size=2);
  err = array->insert(array, 0, "Hello");
  err = array->insert(array, 1, "World");

  array2 = clib_new_Array(char*);
  err = array->insert(array2, 0, "World");
  err = array->insert(array2, 1, "Hello");

  array->concat(array, array2);
  assert_int_equal(4, array->_length);
  assert_int_equal(9, array->_size);
  assert_string_equal("Hello", (char*) array->get(array, 0));
  assert_string_equal("World", (char*) array->get(array, 1));
  assert_string_equal("World", (char*) array->get(array, 2));
  assert_string_equal("Hello", (char*) array->get(array, 3));
  assert_string_equal("World", (char*) array->get(array2, 0));
  assert_string_equal("Hello", (char*) array->get(array2, 1));

  TEST("Concat with unequally sized types");
  array2 = clib_new_Array(uint8_t, .is_primitive_typed=true);
  err = array2->insert(array2, 0, 10);
  err = array->concat(array, array2);
  assert_int_equal(-1, err);
}

int main() {
  puts("CLIB ARRAY UNIT TESTS");

  const struct CMUnitTest tests[] =
    {
      cmocka_unit_test(test_clib_new_Array),
      cmocka_unit_test(test_clib_Array_insert),
      cmocka_unit_test(test_clib_Array__resize),
      cmocka_unit_test(test_clib_Array_get),
      cmocka_unit_test(test_clib_Array_concat)
    };

  int failed_tests =
    cmocka_run_group_tests(tests, NULL, NULL);

  return 0;
}
