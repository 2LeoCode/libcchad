#include <unistd.h>

void cc_kek(void) {
  write(STDOUT_FILENO, "kek\n", 4);
}
