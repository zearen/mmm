// memzip.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "deflate.h"

int _tmain(int argc, _TCHAR* argv[])
{
	FILE* out;
	
	Byte*		outbuf = (Byte*)ALLOC(OUTBUFLEN);
	int			outlen = 0;

	Byte*		inbuf = (Byte*)ALLOC(INBUFLEN);
	int			len;
	int			a;

	FILE* in = fopen("aaa.txt","rb");
	len = fread(inbuf,sizeof(Byte),INBUFLEN,in);
	fclose(in);

	//for(a=0;a<1000;a++)
		outlen = compress(inbuf,len, outbuf);

	out = fopen("bbb.gz","wb");
	len = fwrite(outbuf,sizeof(Byte),outlen,out);
	fclose(out);
 	
	TRYFREE(outbuf);
	TRYFREE(inbuf);
	return 0;
}

