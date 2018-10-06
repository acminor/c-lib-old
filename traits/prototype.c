/*
  Some initial prototyping of a trait system in c
*/
#include<stddef.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

struct KVIH {
  char* name;
  int offset;
} typedef KVIH;

struct ImplementationHandler {
  int (*get_implementation)(struct ImplementationHandler* self, char* trait);
  int (*set_implementation)(struct ImplementationHandler* self, char* trait, int offset);
  KVIH* offset_map; // should be hash map but for now array
  int offset_map_len;
} typedef ImplementationHandler;

int get_implementation(struct ImplementationHandler* self, char* trait) {
  for(int i = 0; i < self->offset_map_len; i++) {
    if (strcmp(self->offset_map[i].name, trait) == 0)
      return self->offset_map[i].offset;
  }

  return -1;
}

int set_implementation(struct ImplementationHandler* self, char* trait, int offset) {
  self->offset_map[self->offset_map_len].name = trait;
  self->offset_map[self->offset_map_len].offset = offset;
  self->offset_map_len++;

  return 0;
}

ImplementationHandler new_ImplementationHandler() {
  ImplementationHandler out;
  out.get_implementation=get_implementation;
  out.set_implementation=set_implementation;
  out.offset_map=malloc(sizeof(KVIH)*32);
  out.offset_map_len=0;

  return out;
}

struct TraitPrintable {
  void* self;
  int (*print)(struct TraitPrintable* self_trait, const char* message);
} typedef TraitPrintable;

struct Buffer {
  ImplementationHandler traitHandler;
  TraitPrintable printable;
  FILE* buffer;
} typedef Buffer;

int Buffer_printable_print(struct TraitPrintable* self_trait, const char* message) {
  Buffer* self = (Buffer*) self_trait->self;
  fputs(message, self->buffer);
  return 0;
}

Buffer* new_Buffer(FILE* buffer) {
  Buffer* out = malloc(sizeof(Buffer));
  out->traitHandler = new_ImplementationHandler();

  /* Attach Printable Trait -- probably can be turned into a function or macro */
  out->traitHandler.set_implementation(&out->traitHandler,
                                       "printable", offsetof(Buffer, printable));
  out->printable.self = out;
  out->printable.print = Buffer_printable_print;

  out->buffer = buffer;

  return out;
}

int main() {
  Buffer* test = new_Buffer(stdout);
  test->printable.print(&test->printable, "This is test from Buffer\n");

  void* test2 = test;
  ImplementationHandler testTraitHandler = *((ImplementationHandler*) test2);
  int offset = testTraitHandler.get_implementation(&testTraitHandler, "printable");
  TraitPrintable testPrintable = *((TraitPrintable*) (((char*) test2) + offset));
  testPrintable.print(&testPrintable, "Hello from TraitPrintable\n");

  test->buffer = stderr;
  testPrintable.print(&testPrintable, "Hello from TraitPrintable (changed to error buffer)\n");
}
