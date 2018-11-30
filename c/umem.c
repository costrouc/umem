#include <assert.h>
#include <string.h>
#include "umem.h"

/*
  umemVirtual virtual methods.
*/
static uintptr_t umem_alloc_(umemVirtual  * const me, size_t nbytes) {
  assert(0); /* purely-virtual function should never be called */
  return 0;
}

static void umem_free_(umemVirtual  * const me, uintptr_t adr) {
  assert(0); /* purely-virtual function should never be called */
}

static void umem_set_(umemVirtual * const me, uintptr_t adr, int c, size_t nbytes) {
  assert(0); /* purely-virtual function should never be called */
}

static void umem_copy_to_(umemVirtual * const me, uintptr_t src_adr,
			  umemVirtual * const she, uintptr_t dest_adr,
			  size_t nbytes) {
  assert(0); /* purely-virtual function should never be called */
}

static void umem_copy_from_(umemVirtual  * const me, uintptr_t src_adr,
			    umemVirtual  * const she, uintptr_t dest_adr,
			    size_t nbytes) {
  assert(0); /* purely-virtual function should never be called */
}

/*
  umemVirtual constructor.
*/
void umemVirtual_ctor(umemVirtual * const me) {
  static struct umemVtbl const vtbl = {
    &umem_alloc_,
    &umem_free_,
    &umem_set_,
    &umem_copy_to_,
    &umem_copy_from_,
  };
  me->vptr = &vtbl;
  me->type = umemVirtualDevice;
  me->status.type = umemOK;
  // message is owned by umemVirtual instance. So, use only
  // umem_set_status or umem_clear_status to change it.
  me->status.message = NULL;
}

/*
  umemVirtual destructor.
*/
void umemVirtual_dtor(umemVirtual * const me) {
  if (me->status.message != NULL) {
    free(me->status.message);
    me->status.message = NULL;
  }
  me->status.type = umemOK;
}

/*
  Status handling utility functions.
*/
void umem_set_status(umemVirtual * const me,
		     umemStatusType type, const char * message) {
  if (message == NULL) {
    if (me->status.message != NULL)
      free(me->status.message);
    me->status.message = NULL;
  } else {
    if (me->status.message == NULL) {
      me->status.message = strdup(message);
    } else {
      // append message
      char buf[256];
      buf[0] = 0;
      if (me->status.type != type) {
	sprintf(buf, "\nstatus %s changed to %s",
		umem_get_status_name(me->status.type),
		umem_get_status_name(type));
      }
      me->status.message = realloc(me->status.message,
				   strlen(me->status.message) + strlen(buf) + strlen(message) + 2);
      strcat(me->status.message, buf);
      strcat(me->status.message, "\n");
      strcat(me->status.message, message);
    }
  }
  me->status.type = type;
}

void umem_clear_status(umemVirtual  * const me) {
  if (me->status.message != NULL) {
    free(me->status.message);
    me->status.message = NULL;
  }
  me->status.type = umemOK;
}

/*
  Utility functions
*/

const char* umem_get_device_name(umemDeviceType type) {
  static const char virtual_device_name[] = "Virtual";
  static const char host_device_name[] = "Host";
  static const char file_device_name[] = "FILE";
  static const char cuda_device_name[] = "CUDA";
  static const char mmap_device_name[] = "MMAP"; // NOT IMPLEMENTED
  switch(type) {
  case umemVirtualDevice: return virtual_device_name;
  case umemHostDevice: return host_device_name;
  case umemFileDevice: return file_device_name;
  case umemCudaDevice: return cuda_device_name;
  }
  return NULL;
}

const char* umem_get_status_name(umemStatusType type) {
  static const char ok_name[] = "OK";
  static const char memory_error_name[] = "MemoryError";
  static const char runtime_error_name[] = "RuntimeError";
  static const char io_error_name[] = "IOError";
  static const char notimpl_error_name[] = "NotImplementedError";
  static const char assert_error_name[] = "AssertError";
  static const char value_error_name[] = "ValueError";
  static const char type_error_name[] = "TypeError";
  switch (type) {
  case umemOK: return ok_name;
  case umemMemoryError: return memory_error_name;
  case umemRuntimeError: return runtime_error_name;
  case umemIOError: return io_error_name;
  case umemNotImplementedError: return notimpl_error_name;
  case umemAssertError: return assert_error_name;
  case umemValueError: return value_error_name;
  case umemTypeError: return type_error_name;
  }
  return NULL;
}
