﻿#pragma once

#include <vector>

#include "CommonDefine.h"

const tstring GetOnlyFileNameWithoutExt(const tstring & strFullFileName);

class CStringCompareIgnoreCase
{
public:
  bool operator()(const tstring & a, const tstring & b)
  {
    /*
    // 문자열 비교를 할 때 모두 소문자로 바꿔서 비교한다.
    char szTempA[FILENAME_MAX], szTempB[FILENAME_MAX];
    _snprintf(szTempA, sizeof(szTempA), a.c_str());
    _snprintf(szTempB, sizeof(szTempB), b.c_str());
    strlwr(szTempA);
    strlwr(szTempB);

    return (strcmp(szTempB, szTempA) > 0);
    */
    return (_tcscmp(b.c_str(), a.c_str()) > 0);
  }
};

class CStringCompareIgnoreCase_LengthFirst
{
public:
  bool operator()(const tstring & a, tstring & b)
  {
    // 길이를 우선.
    if ( a.size() < b.size() )
    {
      return true;
    }

    if ( a.size() > b.size() )
    {
      return false;
    }

    /*
    // 문자열 비교를 할 때 모두 소문자로 바꿔서 비교한다.
    char szTempA[FILENAME_MAX], szTempB[FILENAME_MAX];
    _snprintf(szTempA, sizeof(szTempA), a.c_str());
    _snprintf(szTempB, sizeof(szTempB), b.c_str());
    strlwr(szTempA);
    strlwr(szTempB);

    return (strcmp(szTempB, szTempA) > 0);
    */
    return (_tcscmp(b.c_str(), a.c_str()) > 0);

  }
};

void DebugPrintf( const TCHAR *fmt, ... );

bool IsPressedVirtualKey(int vk);

bool SetRegistryValue(HKEY hOpenKey, const tstring & strKey,LPCTSTR szValue, const tstring & strData);

/// 최대 크기를 넘지 않는 적당한 리사이즈 크기를 돌려준다.
RECT GetResizedRectForBigToSmall(const RECT & MaximumSize, const RECT & originalSize);

/// 최대 크기를 넘지 않는 적당한 리사이즈 크기를 돌려준다.
RECT GetResizedRectForSmallToBig(const RECT & MaximumSize, const RECT & originalSize);

/// int 를 tstring type 으로 돌려준다.
tstring toString(int i);

/// 폴더를 선택하는 다이얼로그를 띄운다.
bool SelectFolder(HWND hWnd, TCHAR * szFolder);

/// 드라이브와 폴더명과 파일명으로 이루어진 문자열을 주면, 드라이브와 폴더명까지만 반환한다.
tstring GetFolderNameFromFullFileName(const tstring & strFullFilename);

/// 드라이브와 폴더명과 파일명으로 된 문자열을 주면, 파일명만 준다.
tstring GetFileNameFromFullFileName(const tstring & strFullFilename);

/// string 을 wstring 으로 변환
std::wstring getWStringFromString(const std::string & str);

/// Make dump file name
tstring GetDumpFilename();

/// 현재 실행 파일이 있는 폴더를 얻는다.
tstring GetProgramFolder();

/// 컴파일러 버젼에 맞게 함수 정의
void SplitPath(const TCHAR * path, TCHAR * drive, size_t driveNumberOfElements, TCHAR * dir, size_t dirNumberOfElements,
        TCHAR * fname, size_t nameNumberOfElements, TCHAR * ext, size_t extNumberOfElements);

#ifdef _MSC_VER
#define SPrintf	StringCchPrintf
#else
#define SPrintf _snwprintf
#endif


#ifdef _DEBUG
#define TIMECHECK_START(szMsg)	std::chrono::system_clock::time_point dwStart___ = std::chrono::system_clock::now(); TCHAR szTimeCheckMsg___[] = TEXT(szMsg);
#define TIMECHECK_END()			long long dwDiff___ = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - dwStart___).count(); DebugPrintf(TEXT("--- %s - time(%d ms)"), szTimeCheckMsg___, dwDiff___);
#else
#define TIMECHECK_START(szMsg)	;
#define TIMECHECK_END()			;
#endif

UINT_PTR CenterOFNHookProc(HWND hdlg, UINT uiMsg, WPARAM /*wParam*/, LPARAM /*lParam*/);

void GetSortedFileList(const tstring & strFolderPathAndWildCard, const eFileSortOrder sort_order,
  std::vector<FileData>* filelist);

void FindFile(const tstring& path, const bool bFindRecursive, std::vector< FileData >* foundStorage);
void FindFolders(const TCHAR *path, std::vector<tstring> & foundStorage, bool bFindRecursive = false);

tstring ConvertFileSize(const int32_t filesize_bytes);
