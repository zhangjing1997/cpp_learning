#include <stdio.h>
#include <stdlib.h>

unsigned power(unsigned x, unsigned y) {
  if (y == 0) {
    return 1;
  }
  else if ((x >= 0) & (y > 0)) {
    return x * power(x, y - 1);
  }
  else if ((x < 0) & (y > 0)) {
    if (y % 2 == 0) {
      return power(x, -y);
    }
    else {
      return -power(x, y);
    }
  }
  return EXIT_SUCCESS;
}
