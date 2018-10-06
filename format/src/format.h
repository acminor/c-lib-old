/* Python style format adapted to C */
#include <stdbool.h>

struct AlignSpecifiers {
  char fill;
  char align;
};

struct FormatSpecifiers {
  struct AlignSpecifiers align;
  char sign;
  bool hash_fmt;
  // bool zero_pad;
  unsigned int min_width;
  unsigned int precision;
  char* type;
};
