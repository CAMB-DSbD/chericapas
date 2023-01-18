/*
 * Programmer: Carlos Molina-Jimenez
 * Date: 01 Jan 2023, Department of Computer Science, Univ of Cambridge
 * 
 * Program: cappermexception.c  
 *          Demonstrates how cheri capabilities can protect againts
 *          ilegal operations.
 * See pg 27 of https://www.cl.cam.ac.uk/techreports/UCAM-CL-TR-947.pdf
 *
 * Source: original version taken from
 * https://i.blackhat.com/USA21/Wednesday-Handouts/us-21-Security-Analysis-Of-Cheri-Isa.pdf
 * where a detailed memory analysis, based on debugger gdb, is presented.
 *
 * Compilation and execution tested on 
 *  
 * morello-camb-2:~/progs/C $ cc --version
 * clang version 13.0.0
 * Target: aarch64-unknown-freebsd14.0
 * Thread model: posix
 * InstalledDir: /usr/local64/llvm-morello/bin
 *
 * Compilation: % cc -g -O2 -Wall -o cappermexception cappermexception.c 
 * Execution:   % ./cappermexception
 */


#include <cheriintrin.h> // interfaces to manupulate cap properties
#include <stdio.h>     
#include <stdlib.h>
                    // 0x100 =  1  x 16^2 + 0  x 16^1 + 0 x 16^0 = 256 
                    // 0xff  = 15 x  16^1 + 15 x 16^0 = 255 
                    // 0x43 is the C ascci char
#define SIZE 0x100                                                    

int main(void){
 char *buf= (char *)malloc(SIZE); // space for 256 chars, buf[0]...buf[255]
 int offset= 0;
 size_t capperms;

 if(!buf){
   perror("malloc allocation failed");
   return 1;
 }

 scanf("%d", &offset); // read a val to be used as index to buff

 printf("buf @ %#p\n", buf); // print cap metadata  
 capperms= cheri_perms_get(buf);
 printf("buf cap original permissions are 0x%lx\n\n",capperms);

 /*
  * Let's write char value in buf[i], for ex 0x43 (i.e the
  * 'C' char).
  * See that if i is between 0 and 255, 0x0 and
  * 0x77 in hexadecimal, respectively, the prog works OK;
  * but it raises a security exception if i >= 256 
  * because the OS takes it as an attempt to write
  * beyond the mem allocated to buf, i.e, 256 char
  */
 printf("write: *(%p + 0x%x)= 0x43\n", buf, offset);
 
 buf[offset]= 0x43; 

 printf("buf[%d]= %c \n", offset, buf[offset]);

 
 char *b= cheri_perms_and(buf,0xfff0ffff);
 capperms= cheri_perms_get(b);
 printf("b cap permissions are 0x%lx\n\n",capperms);

 printf("new buf perms @ %#p\n", buf); // print cap metadata  

 return 0;
}


