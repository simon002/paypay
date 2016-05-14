#pragma once

#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <zlib.h>

class CMyGzip
{
public:
	CMyGzip(void);
	virtual ~CMyGzip(void);

	int zcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);

	int gzcompress(Bytef *data, uLong ndata, Bytef *zdata, uLong *nzdata);

	int zdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);
	int httpgzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);

	int gzdecompress(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);

	int httpgzdecompress_ex(Byte *zdata, uLong nzdata, Byte *data, uLong *ndata);

	char* ungzipHttp(char* source,int len); //http://blog.sina.com.cn/s/blog_65db99840100kwh9.html ÏÌÓãµÄÀý×Ó
};
