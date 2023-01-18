
/*
 * Programmer: Carlos Molina-Jimenez
 * Date: 01 Jan 2023, Department of Computer Science, Univ of Cambridge
 * 
 * Program: invalidcap.c  
 * Source:  invalidcap was inspired by code provided by Jeremy Singer
 *          https://www.capabilitiesforcoders.com/faq/tagcheck.html
 * 
 * capvalidity uses: 
 *  _Bool cheri_tag_get(void *c) function that verifies if a capability
 *        has its validity tag set.
 *  void *cheri_tag_clear(void *c) return a new capability without the
 *        tag bit set.
 * See pg 27 of https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf
 *
 * Compilation and execution tested on 
 *  
 * morello-camb-2:~/progs/C $ cc --version
 * clang version 13.0.0
 * Target: aarch64-unknown-freebsd14.0
 * Thread model: posix
 * InstalledDir: /usr/local64/llvm-morello/bin
 *
 * Compilation: % cc -g -O2 -Wall -o setcappermissions setcappermissions.c 
 * Execution:   % ./setcappermissions
 *m

#include <cheriintrin.h> // interfaces to manupulate cap properties
#include <stdio.h>     
#include <stdlib.h>

int main() {

 size_t capper;

 void *cap = malloc(sizeof(int)); // cap has the tag set


 if (cheri_tag_get(cap) == 1) {
   printf("capability cap is valid\n");
 }

 capper= cheri_perms_get(cap);
 if (capper == CHERI_PERM_EXECUTE){
   printf("capability cap has load permission\n");
 }


 void *newcap= cheri_tag_clear(cap); // newcap has the tag cleared 
 
 if (cheri_tag_get(newcap) == 0) {
   printf("capability newcap is invalid\n");
 }

 if (cheri_tag_get(cap) == 1) {
   printf("capability cap is valid\n");
 }

 return 0;
}
 
