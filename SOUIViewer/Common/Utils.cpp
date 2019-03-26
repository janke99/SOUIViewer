#include "stdafx.h"
#include "Utils.h"

void Utils::DbgPrintf(LPCSTR lpFmt, ...)
{
	char szBuf[1024];
	memset(szBuf, 0, 1024);
	va_list argList;
	va_start(argList, lpFmt);
	wvsprintfA(szBuf, lpFmt, argList);
	va_end(argList);
	OutputDebugStringA(szBuf);
}

void Utils::DbgPrintf(LPCWSTR lpFmt, ...)
{
	WCHAR szBuf[1024];
	memset(szBuf, 0, 1024);
	va_list argList;
	va_start(argList, lpFmt);
	wvsprintfW(szBuf, lpFmt, argList);
	va_end(argList);
	OutputDebugStringW(szBuf);
}

std::string Utils::Format(const char *fmt, ...)
{
	std::string strResult = "";
	if (NULL != fmt)
	{
		va_list marker = NULL;
		va_start(marker, fmt);
		size_t nLength = _vscprintf(fmt, marker) + 1;
		std::vector<char> vBuffer(nLength, '\0');
		int nWritten = _vsnprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}

		va_end(marker);
	}

	return strResult;
}

BOOL Utils::DirectoryExists(LPCTSTR lpDirectory)
{
	DWORD Code = ::GetFileAttributes(lpDirectory);
	return (Code != INVALID_FILE_ATTRIBUTES) && ((Code & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

std::wstring Utils::Format(const wchar_t *fmt, ...)
{
	std::wstring strResult = L"";
	if (NULL != fmt)
	{
		va_list marker = NULL;
		va_start(marker, fmt);
		size_t nLength = _vscwprintf(fmt, marker) + 1;
		std::vector<wchar_t> vBuffer(nLength, L'\0');
		int nWritten = _vsnwprintf_s(&vBuffer[0], vBuffer.size(), nLength, fmt, marker);
		if (nWritten > 0)
		{
			strResult = &vBuffer[0];
		}

		va_end(marker);
	}

	return strResult;
}
