/*
  Inspiration was taken from the Javascript Array API interface for its beautiful interface.
 */
#include <stdbool.h>

#ifndef CLIB_ARRAY_H
#define CLIB_ARRAY_H

// local and global level type assertions
#define CLIB__STRUCTIZE(name) struct name;
#define CLIB_NEW_LTYPE_ASSERT(QUALIFIED_NAME, TYPE) CLIB__STRUCTIZE(QUALIFIED_NAME ## TYPE)
#define CLIB_NEW_GTYPE_ASSERT(FUNCTION, QUALIFIED_NAME, TYPE) CLIB__STRUCTIZE(FUNCTION ## QUALIFIED_NAME ## TYPE)
#define CLIB_LTCAST(QUALIFIED_NAME, TYPE, ELEM) ((((CLIB__STRUCTIZE(QUALIFIED_NAME ## TYPE))*) NULL), (TYPE) ELEM)
#define CLIB_GTCAST(QUALIFIED_NAME, TYPE) // TODO

// TODO more stuff to implement once other functionality implemented;
struct Array {
  char* _data;
  unsigned int _length;
  unsigned int _type_size;
  unsigned int _size;
  // HACK 1.0 Fix at some point
  bool _is_primitive_typed;
  /* char* _type; implement optional later for additional run-time checks */
  int (*_cast)(void* element);
  int (*_resize)(struct Array* self, unsigned int new_size);
  void* (*get)(const struct Array* self, int index);
  int (*_set)(struct Array* self, int index, void* element);
  int (*_free_element)(struct Array* self, void* element);
  int (*free)(struct Array* self);
  int (*insert)(struct Array* self, int index, void* element);
  int (*append)(struct Array* self, void** element);
  int (*prepend)(struct Array* self, void** element);
  int (*get_front)(const struct Array* self);
  int (*get_rear)(const struct Array* self);
  int (*take_front)(struct Array* self);
  int (*take_rear)(struct Array* self);
  /* take TODO */
  int (*concat)(struct Array* self, const struct Array* other);
  int (*concat_and_take)(struct Array* self, struct Array* other);
  int (*find)(const struct Array* self, const void* element);
  int (*remove)(struct Array* self, const void* element);
  bool (*contains)(const struct Array* self, const void* element);
  struct Array* (*reverse)(const struct Array* self);
  int (*mod_reverse)(struct Array* self);
  /* Will be implemented once Function type is created. */
  // TODO function type planning because I want to have javascript
  //      style map and reduce methods etc. so that we can use
  //      elements and index and other things depending on function
  //      length. I do not want people to have to worry about
  //      programming va_args functions
  //
  //      -- FUNCTION PLANNING:
  //         have six function pointers (0 args through 5 args)
  //         size to cast to proper
  //         if functions of more than 5 args then one function of
  //            va_args which is called for any function greater than 5 args
  //         the default function call is a va_args function which maps to the appropriate function
  //         -- then map can call the appropriate length function
  //      -- END PLANNING:
  /* struct Array* (map)(const struct Array* self); */
  /* int (mod_map)(struct Array* self); */
  /* struct Array* (filter)(const struct Array* self); */
  /* int (mod_filter)(struct Array* self); */
  /* struct Array* (reduce)(const struct Array* self); */
} typedef Array;

int _clib_free_wrapper(void* element) {
  free(element);
}

struct {
  unsigned int initial_size;
  int (*element_cast_function)(void* element);
  int (*element_free_function)(void* element);
  // HACK 1.0 Fix at some point
  // since primitive types point to (when void* cast)
  // objects in memory that we may not have access to
  // thus for now special logic
  bool is_primitive_typed;
  // convenience initial values
  // fill
  // ... TODO
} typedef _clib_array_init;

int _clib_void_ptr_cast(void* element) {
  return 0;
}

struct Array* _clib_new_Array(unsigned int type_size, _clib_array_init args);
// HACK 1.0 Fix at some point -- assumes default is pointer objects
// is-primitive-typed
// only works for primitive types with size leq pointer size
#define clib_new_Array(type, ...)                \
  _clib_new_Array(sizeof(type),                                   \
                  (_clib_array_init){                             \
                    .initial_size=8,                              \
                    .element_cast_function=_clib_void_ptr_cast,   \
                    .is_primitive_typed=false,                    \
                    .element_free_function=_clib_free_wrapper     \
                    __VA_ARGS__})

#define CLIB_BYTE_SIZE(type_size, index) (index)*(type_size)
#define CLIB_ARRAY_GET_VOIDPTR(data, type_size, index) \
  (void*) &data[CLIB_BYTE_SIZE(type_size, index)]

int clib_Array_insert(struct Array* self, int index, void* element);
int clib_Array_append(struct Array* self, void** element);
int clib_Array_prepend(struct Array* self, void** element);
int clib_Array__resize(struct Array* self, unsigned int new_size);
void* clib_Array_get(const struct Array* self, int index);
int clib_Array__set(struct Array* self, int index, void* element);
int clib_Array_concat(struct Array* self, const struct Array* other);
int clib_Array_concat_and_take(struct Array* self, struct Array* other);
int clib_Array_find(const struct Array* self, const void* element);
int clib_Array_remove(struct Array* self, const void* element);
bool clib_Array_contains(const struct Array* self, const void* element);
struct Array* clib_Array_reverse(const struct Array* self);
int clib_Array_mod_reverse(struct Array* self);

#endif /* CLIB_ARRAY_H */
