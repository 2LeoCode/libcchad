#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <cchad/utils.h>

Test(utils, kek) {
  cr_redirect_stdout();

  kek();

  cr_expect_stdout_eq_str("kek\n");
}
