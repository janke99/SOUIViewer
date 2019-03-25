#include "stdafx.h"
#include "Utils.h"
#include <ShellAPI.h>
#include <Shlobj.h>
#include <time.h>
#pragma comment(lib,"Shell32.lib")

void Utils::DbgPrintfA(LPCSTR lpFmt, ...)
{
	char szBuf[1024];
	memset(szBuf, 0, 1024);
	va_list argList;
	va_start(argList, lpFmt);
	wvsprintfA(szBuf, lpFmt, argList);
	va_end(argList);
	OutputDebugStringA(szBuf);
}

void Utils::DbgPrintfW(LPCWSTR lpFmt, ...)
{
	WCHAR szBuf[1024];
	memset(szBuf, 0, 1024);
	va_list argList;
	va_start(argList, lpFmt);
	wvsprintfW(szBuf, lpFmt, argList);
	va_end(argList);
	OutputDebugStringW(szBuf);
}

void Utils::GetSpecialFolderDir(UINT uCode, std::wstring& chFolder)
{
	LPITEMIDLIST pidl;
	LPMALLOC pShellMalloc;
	TCHAR m_lpszDefaultDir[MAX_PATH] = { 0 }, szDocument[MAX_PATH] = { 0 };

	if (SUCCEEDED(SHGetMalloc(&pShellMalloc)))
	{
		if (SUCCEEDED(SHGetSpecialFolderLocation(NULL, uCode, &pidl)))
		{
			if (SHGetPathFromIDList(pidl, m_lpszDefaultDir))
			{
				::GetShortPathName(szDocument, m_lpszDefaultDir, _MAX_PATH);
				chFolder = std::wstring(m_lpszDefaultDir, m_lpszDefaultDir + wcslen(m_lpszDefaultDir));
			}

			pShellMalloc->Free(pidl);
		}

		pShellMalloc->Release();
	}
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

BOOL Utils::DirectoryExists(LPCWSTR Directory)
{
	DWORD Code = ::GetFileAttributes(Directory);
	return (Code != INVALID_FILE_ATTRIBUTES) && ((Code & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

BOOL Utils::DirectoryExists(LPCSTR Directory)
{
	DWORD Code = ::GetFileAttributesA(Directory);
	return (Code != INVALID_FILE_ATTRIBUTES) && ((Code & FILE_ATTRIBUTE_DIRECTORY) != 0);
}

void Utils::CreateClassID(std::wstring &str)
{
	GUID guid;
	CoCreateGuid(&guid);
	OLECHAR* bstrGuid;
	StringFromCLSID(guid, &bstrGuid);
	str = bstrGuid;
	::CoTaskMemFree(bstrGuid);
}

BOOL Utils::IsWow64()
{
	BOOL bResult = FALSE;
	HMODULE ker32 = ::GetModuleHandle(L"kernel32.dll");
	if (!ker32)
	{
		ker32 = ::LoadLibrary(L"kernel32.dll");
	}

	if (ker32)
	{
		typedef BOOL(WINAPI *TIsWow64Process)(__in  HANDLE hProcess, __out PBOOL Wow64Process);
		typedef VOID(WINAPI *TGetNativeSystemInfo)(__out LPSYSTEM_INFO lpSystemInfo);
		//GetNativeSystemInfo函数从Windows XP开始才有，而IsWow64Process函数从Windows XP SP2以及Windows Server 2003 SP1开始才有
		TGetNativeSystemInfo pfn_GetNativeSystemInfo = (TGetNativeSystemInfo)::GetProcAddress(ker32, "GetNativeSystemInfo");
		if (pfn_GetNativeSystemInfo)
		{
			SYSTEM_INFO sinfo = { 0 };
			pfn_GetNativeSystemInfo(&sinfo);
			bResult = (sinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_AMD64) || (sinfo.wProcessorArchitecture == PROCESSOR_ARCHITECTURE_IA64);
		}
		else
		{
			TIsWow64Process pfn_IsWow64Process = (TIsWow64Process)::GetProcAddress(ker32, "IsWow64Process");
			if (pfn_IsWow64Process)
			{
				pfn_IsWow64Process(::GetCurrentProcess(), &bResult);
			}
		}
	}

	return bResult;
}

BOOL Utils::ShellOpenFile(LPCTSTR pszName)
{
	int nResult = (int)ShellExecute(NULL, _T("open"), pszName, NULL, NULL, SW_SHOW);
	return nResult > 32;
}

BOOL Utils::ShellOpenUrl(LPCTSTR pszUrl)
{
	int nResult = (int)ShellExecuteW(NULL, _T("open"), pszUrl, NULL, NULL, SW_SHOW);
	return nResult > 32;
}

void Utils::RandomStr(UINT uLength, TString& chRnd)
{
	srand((unsigned)time(NULL));
	BYTE uTable[] = { 48,49,50,51,52,53,54,55,56,57,65,66,67,68,69,70,
		71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,
		97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
		113,114,115,116,117,118,119,120,121,122 };

	int nMax = _countof(uTable);

	std::string result;
	for (UINT i = 0; i < uLength; i++)
	{
		int nPos = (rand() % (nMax));
		result.push_back(uTable[nPos]);
	}

	chRnd = TString(result.begin(), result.end());
}

int Utils::RandomInt(UINT nMin, UINT nMax)
{
	srand((unsigned)time(NULL));
	return ((rand() % (nMax - nMin + 1)) + nMin);				// [a,b)
}
