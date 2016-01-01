
// md5Test
// A simple test of the MD5 library.


#include <stdio.h>
#include "md5.h"


int main(void)
{
  printf("MD5 Key of the phrase 'md5Test' = <%s>\n", MD5String("md5Test"));

  printf("MD5 Key of the file 'md5.cpp' = <%s>\n", MD5File("md5.cpp"));

  return 0;
}