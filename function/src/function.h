#include <stdlib.h>
#include <stdarg.h>

struct Function;

union _FUnion {
  void* (*func_0)();
  void* (*func_1)(void*);
  void* (*func_2)(void*,void*);
  void* (*func_3)(void*,void*,void*);
  void* (*func_4)(void*,void*,void*,void*);
  void* (*func_5)(void*,void*,void*,void*,void*);
  // number of args not needed since list already initialized
  // but would it be better for programmer to have ??? TODO
  // NOTE: this function does not free args but call does
  void* (*func_n)(va_list args);
};

struct Function {
  unsigned int number_args;
  union _FUnion funion;
  void* (*call)(struct Function*, ...);
} typedef Function;

#define num_args_0 func_0
#define num_args_1 func_1
#define num_args_2 func_2
#define num_args_3 func_3
#define num_args_4 func_4
#define num_args_5 func_5
#define num_args_6 func_n
#define num_args_7 func_n
#define num_args_8 func_n
#define num_args_9 func_n
#define num_args_10 func_n
#define num_args_11 func_n
#define num_args_12 func_n
#define num_args_13 func_n
#define num_args_14 func_n
#define num_args_16 func_n
#define num_args_17 func_n
#define num_args_18 func_n
#define num_args_19 func_n
#define num_args_20 func_n
#define num_args_21 func_n
#define num_args_22 func_n
#define num_args_23 func_n
#define num_args_24 func_n
#define num_args_25 func_n
#define num_args_26 func_n
#define num_args_27 func_n
#define num_args_28 func_n
#define num_args_29 func_n
#define num_args_30 func_n
#define num_args_31 func_n
#define num_args_32 func_n

// use macro to avoid void* cast of changing function signature
#define new_Function(name, arg_num, function)  \
  (name = _new_Function(arg_num),                        \
   name->funion.num_args_##arg_num = function,      \
   name)

void* Function_call(Function* self, ...) {
  void* result = NULL;
  va_list args;
  va_start(args, self->number_args);

  switch (self->number_args) {
  case 0:
    self->funion.func_0();
    break;
  case 1:
    result = self->funion.func_1(va_arg(args, void*));
    break;
  case 2:
    result = self->funion.func_2(va_arg(args, void*), va_arg(args, void*));
    break;
  case 3:
    result = self->funion.func_3(va_arg(args, void*), va_arg(args, void*), va_arg(args, void*));
    break;
  case 4:
    result = self->funion.func_4(va_arg(args, void*), va_arg(args, void*),
                                 va_arg(args, void*), va_arg(args, void*));
    break;
  case 5:
    result = self->funion.func_5(va_arg(args, void*), va_arg(args, void*),
                                 va_arg(args, void*), va_arg(args, void*), va_arg(args, void*));
    break;
  default:
    result = self->funion.func_n(args);
    break;
  }

  va_end(args);
  return result;
}

Function* _new_Function(unsigned int number_args) {
  // will never happen due to compiler error for the case
  // of using the new_Function macro
  if (number_args < 0) {
    return NULL;
  }

  Function* output = malloc(sizeof(Function));
  output->number_args = number_args;
  output->call = Function_call;
  return output;
}

void* test(void* blah, void* bleh) {
  return 0;
}

int main() {
  Function* t = new_Function(t, 2, test);
}
