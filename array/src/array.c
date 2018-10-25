#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "array.h"

Array* _clib_new_Array(unsigned int type_size, _clib_array_init args) {
  Array* array = malloc(sizeof(Array));
  array->_data = malloc(args.initial_size*type_size);
  array->_type_size = type_size;
  array->_size = args.initial_size;
  array->_length = 0;
  // HACK 1.0 Fix at some point
  array->_is_primitive_typed = args.is_primitive_typed;

  array->_resize = clib_Array__resize;
  array->_cast = args.element_cast_function; // TODO
  array->insert = clib_Array_insert;
  array->_set = clib_Array__set;
  array->prepend = clib_Array_prepend;
  array->append = clib_Array_append;
  array->get = clib_Array_get;
  array->concat = clib_Array_concat;
  array->concat_and_take = clib_Array_concat_and_take; // TODO
  array->find = clib_Array_find; // TODO
  array->remove = clib_Array_remove; // TODO
  array->contains = clib_Array_contains; // TODO
  array->reverse = clib_Array_reverse; // TODO
  array->mod_reverse = clib_Array_mod_reverse; // TODO

  return array;
}
int clib_Array_insert(struct Array* self, int index, void* element) {
  // inserting past valid array segment
  if (index > self->_length || index < 0) {
    return -1;
  }

  // TODO update to better algorithm
  if (self->_length == self->_size) {
    self->_resize(self, self->_size + 5);
  }

  // shift right elements
  for(int i = self->_length; i > index; i--) {
    void* voidptr = CLIB_ARRAY_GET_VOIDPTR(self->_data, self->_type_size, i-1);
    int index = CLIB_BYTE_SIZE(self->_type_size, i-1);
    self->_set(self, i, CLIB_ARRAY_GET_VOIDPTR(self->_data, self->_type_size, i-1));
  }

  // HACK 1.0 Fix at some point
  // only for types less than
  // the size of a ptr which
  // in our case is int or long
  self->_set(self, index, self->_is_primitive_typed ? &element : element);
  self->_length++;

  return 0;
}
int clib_Array_append(struct Array* self, void** element){
  return self->insert(self, self->_length, element);
}
int clib_Array_prepend(struct Array* self, void** element){
  return self->insert(self, 0, element);
}
void* clib_Array_get(const struct Array* self, int index){
  if (index > self->_length || index < 0) {
    return NULL;
  }

  void* element = malloc(self->_type_size);
  // HACK 1.0 Fix at some point
  memcpy(self->_is_primitive_typed ? &element : element,
         CLIB_ARRAY_GET_VOIDPTR(self->_data, self->_type_size, index),
         self->_type_size);

  return element;
}
int clib_Array__set(struct Array* self, int index, void* element) {
  memcpy(CLIB_ARRAY_GET_VOIDPTR(self->_data, self->_type_size, index),
         element, self->_type_size);

  return 0;
}
int clib_Array_set(struct Array* self, int index, void* element) {
  if (index > self->_length || index < 0) {
    return -1;
  }

  self->_set(self, index, element);
  return 0;
}
int clib_Array__resize(struct Array* self, unsigned int new_size) {
  self->_data = realloc(self->_data, CLIB_BYTE_SIZE(self->_type_size, new_size));
  self->_size = new_size;

  return 0;
}
int clib_Array_concat(struct Array* self, const struct Array* other) {
  if (self->_type_size != other->_type_size) {
    return -1;
  }

  unsigned int remaining_capacity = self->_size - self->_length;
  if (remaining_capacity < other->_length) {
    unsigned int capacity_needed = other->_length - remaining_capacity;
    // TODO better algorithm
    self->_resize(self, self->_size + capacity_needed + 5);
  }

  for(int i = 0; i < other->_length; i++) {
    self->append(self, other->get(other, i));
  }

  return 0;
}
int clib_Array_concat_and_take(struct Array* self, struct Array* other){
  return 0;
}
// TODO test
int clib_Array_find(const struct Array* self, const void* element){
  void* canidate = NULL;
  for (int i = 0; i < self->_length; i++) {
    // TODO refactor GET VOIDPTR as (array, index)
    canidate = CLIB_ARRAY_GET_VOIDPTR(self->_data, self->_size, i);
    if ((memcmp(canidate, element, self->_size) == 0))
      return i;
  }

  return -1;
}
int clib_Array_remove(struct Array* self, const void* element){
  return 0;
}
// TODO test
bool clib_Array_contains(const struct Array* self, const void* element){
  int element_index = self->find(self, element);
  return (element_index != -1) ? true : false;
}
// TODO test
Array* clib_Array_reverse(const struct Array* self){
  // TODO refactor into copy method or macro
  _clib_array_init args = {.initial_size=self->_size,
                           .element_cast_function=self->_cast,
                           .element_free_function=self->_free_element,
                           .is_primitive_typed=self->_is_primitive_typed};
  Array* reversed = _clib_new_Array(self->_type_size, args);

  for (int i = self->_length - 1; i >= 0; i++) {
    // TODO only shallow pointer copy (should it be deep?)
    reversed->append(reversed, self->get(self, i));
  }

  return reversed;
}
int clib_Array_mod_reverse(struct Array* self){
  return 0;
}
