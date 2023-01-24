/*
 * Programmer: Kian Cross.
 * Date: 22 Jan 2023, Department of Computer Science, Univ of Cambridge
 * 
 * Program: sealed.c
 *          Demonstrates how CHERI capabilities can seal a memory address.
 *
 * Compilation and execution tested on 
 *
 * kc642@morello-camb-3:~ $ cc --version
 * clang version 13.0.0
 * Target: aarch64-unknown-freebsd14.0
 * Thread model: posix
 * InstalledDir: /usr/local64/llvm-morello/bin
 * kc642@morello-camb-3:~ $
 *
 * Compilation: % cc -g -O0 -Wall -o sealed sealed.c
 * Execution:   % ./sealed
 */

#include <cheri/cheric.h>
#include <sys/sysctl.h>
#include <stdio.h>

/*
 * Gets a capability with permissions to seal.
 */
int get_sealing_cap(void **cap_ptr) {
  void *cap = *cap_ptr;
  size_t cap_size = sizeof(cap);

  if (sysctlbyname("security.cheri.sealcap", &cap, &cap_size, NULL, 0) < 0) {
    return -1;
  }

  *cap_ptr = cap;
  return 0;
}

int main() {
  void *sealing_cap = NULL;
  if (get_sealing_cap(&sealing_cap) < 0) {
    fprintf(stderr, "Error: failed to get sealing capability");
    return 1;
  }
  
  int num = 13;
  int *ptr = &num;

  printf("Pointer value: %d\n", *ptr);

  // sealed_ptr should now be inaccessible.
  void *sealed_ptr = cheri_seal(ptr, sealing_cap);

  // This will cause a security exception.
#if 0
  printf("Pointer value after seal: %d\n", *((int *) sealed_ptr));
#endif
 
  // unsealed_ptr should now be accessible.
  void *unsealed_ptr = cheri_unseal(sealed_ptr, sealing_cap);

  printf("Pointer value after unseal: %d\n", *((int *) unsealed_ptr));

  return 0;
}
