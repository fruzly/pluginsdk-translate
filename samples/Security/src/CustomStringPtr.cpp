#include "StdAfx.h"
#include "../Include/CustomStringPtr.h"


CUncopyable::CUncopyable()
{

}

CUncopyable::~CUncopyable()
{

}

WideString::WideString()
{
	m_rawWideString = FSWideStringNew();
}

WideString::WideString(wchar_t ch )
{
	m_rawWideString = FSWideStringNew2(ch);
}

WideString::WideString( FS_LPCWSTR lpcInit, FS_StrSize size /*= -1*/ )
{
	m_rawWideString = FSWideStringNew3(lpcInit, size);
}

WideString::~WideString()
{
	if (NULL != m_rawWideString)
		FSWideStringDestroy(m_rawWideString);
}

ByteString::ByteString()
{
	m_rawByteString = FSByteStringNew();
}

ByteString::ByteString( FS_CHAR ch )
{
	m_rawByteString = FSByteStringNew2(ch);
}

ByteString::ByteString( FS_LPCSTR ptr, FS_StrSize len )
{
	m_rawByteString = FSByteStringNew3(ptr, len);
}

ByteString::ByteString( FS_LPCBYTE ptr, FS_StrSize len )
{
	m_rawByteString = FSByteStringNew4(ptr, len);
}

ByteString::~ByteString()
{
	FSByteStringDestroy(m_rawByteString);
}