#include <criterion/criterion.h>
#include <criterion/redirect.h>

#include <utils.h>

Test(util_kek, base) {
  cr_redirect_stdout();

  cc_kek();

  cr_expect_stdout_eq_str("kek\n");
}
