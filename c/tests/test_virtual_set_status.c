#include "umem_testing.h"

int main() {
  umemVirtual virt;
  umemVirtual_ctor(&virt);
  umem_set_status(&virt, umemNotImplementedError, "notimpl");
  assert(umem_get_status(&virt) == umemNotImplementedError);
  assert_str_eq(umem_get_message(&virt), "notimpl");
  assert(!umem_is_ok(&virt));
  umem_clear_status(&virt);
  assert(umem_is_ok(&virt));
  assert_str_eq(umem_get_message(&virt), "");
  umemVirtual_dtor(&virt);
  RETURN_STATUS;
}
