#pragma once
namespace Utils
{
	void DbgPrintfA(LPCSTR lpFmt, ...);
	void DbgPrintfW(LPCWSTR lpFmt, ...);
	void GetSpecialFolderDir(UINT uCode, std::wstring&);
	std::string Format(const char *fmt, ...);
	std::wstring Format(const wchar_t *fmt, ...);
	BOOL DirectoryExists(LPCSTR Directory);
	BOOL DirectoryExists(LPCWSTR Directory);

	void CreateClassID(std::wstring &);
	BOOL IsWow64();

	BOOL ShellOpenFile(LPCTSTR);
	BOOL ShellOpenUrl(LPCTSTR);

	void RandomStr(UINT uLength, TString& chRnd);
	int	RandomInt(UINT nMin, UINT nMax);
};