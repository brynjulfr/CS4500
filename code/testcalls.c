//#include "testcalls.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
   syscall(302); //mem self in 64 bit
   syscall(303,00400000); //mem_page in 64 bit
   syscall(304); //mem zones in 64 bit
   return 0;
}
