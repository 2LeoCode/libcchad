#include <unistd.h>

void kek(void) {
  write(STDOUT_FILENO, "kek\n", 4);
}
