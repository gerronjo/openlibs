// md5Test
// A simple test of the MD5 library.


#include <stdio.h>
#include "md5.h"

void MD5Test( const void *data, unsigned int len, void *dest )
{
	MD5_CTX ctx;

	MD5Init( &ctx );
	MD5Update( &ctx, (unsigned char *)data, len );
	MD5Final(( unsigned char * )dest, &ctx );
}

int main(void)
{
  //MD5Test
  return 0;
}



