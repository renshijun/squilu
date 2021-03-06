/*	see copyright notice in squirrel.h */
#ifndef _SQSTD_BLOBIMPL_H_
#define _SQSTD_BLOBIMPL_H_

#include "sqstdio.h"
#define BLOB_BUFSIZE 8192

struct SQBlob : public SQStream
{
	SQBlob() {init(0, 8192);}
	SQBlob(SQInteger size, SQInteger allocated=0){init(size, allocated);}
	virtual ~SQBlob();
	SQInteger Write(const void *buffer, SQInteger size);
	SQInteger WriteZstr(const char *zStr);
	SQInteger WriteChar(const char c);
	SQInteger Read(void *buffer,SQInteger size);
	SQInteger Gets(char *buffer,SQInteger size);
	bool Resize(SQInteger n);
	bool Reserve(SQInteger n);
	bool GrowBufOf(SQInteger n);
	bool CanAdvance(SQInteger n) {
		if(_ptr+n>_size)return false;
		return true;
	}
	SQInteger Seek(SQInteger offset, SQInteger origin);
	bool IsValid() {
		return _size == 0 || _buf?true:false;
	}
	bool EOS() {
		return _ptr == _size;
	}
	SQInteger Flush() { return 0; }
	SQInteger Tell() { return _ptr; }
	SQInteger Len() { return _size; }
	SQUserPointer GetBuf(){ return _buf; }
	bool SetLen(SQInteger len);
	static SQUserPointer SQBlob_TAG;
private:
	void init(SQInteger size, SQInteger allocated);
	unsigned char *_buf;
	SQInteger _size;
	SQInteger _allocated;
	SQInteger _ptr;
	bool _owns;
};

#endif //_SQSTD_BLOBIMPL_H_
