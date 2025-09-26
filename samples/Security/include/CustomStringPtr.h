#ifndef CUSTOM_STRING_PTR_H
#define CUSTOM_STRING_PTR_H


#define TERMINATE_CHAR _T('\0')


//uncopyable
class CUncopyable
{
protected:
	CUncopyable();
	~CUncopyable();

private:
	CUncopyable(const CUncopyable&);
	CUncopyable& operator=(const CUncopyable&);
};



//FS_WideString manage class
class WideString: public CUncopyable
{
public:
	WideString();
	WideString(wchar_t ch);
	WideString(FS_LPCWSTR lpcInit, FS_StrSize size = -1);
	~WideString();

	//implicitly cast to FS_WideString
	operator FS_WideString() const
	{
		return m_rawWideString;
	}

	//explicitly cast to FS_WideString
	FS_WideString Get() const
	{
		return m_rawWideString;
	}

	FS_WideString* GetAddr()
	{
		return &m_rawWideString;
	}

private:
	//raw data
	FS_WideString m_rawWideString;
};


/////////////////////////////////////////////////////////////
//FS_WideString manage class
class ByteString: public CUncopyable
{
public:
	ByteString();
	ByteString(FS_CHAR ch);
	ByteString(FS_LPCSTR ptr,  FS_StrSize len = -1);
	ByteString(FS_LPCBYTE ptr,  FS_StrSize len);
	~ByteString();

	//implicitly cast to FS_ByteString
	operator FS_ByteString() const
	{
		return m_rawByteString;
	}

	//explicitly cast to FS_ByteString
	FS_ByteString Get() const
	{
		return m_rawByteString;
	}

	FS_ByteString* GetAddr()
	{
		return &m_rawByteString;
	}

private:
	//raw data
	FS_ByteString m_rawByteString;
};

#endif 