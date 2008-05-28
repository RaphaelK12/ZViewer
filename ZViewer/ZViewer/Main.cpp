﻿/********************************************************************
*
* Created by zelon(Kim, Jinwook Korea)
* 
*   2005. 5. 7
*	Main.cpp
*
*                                       http://www.wimy.com
*********************************************************************/

/**
	@mainpage ZViewer's Document

	ZViewer 는 GPL 에 의해 배포되는 이미지 뷰어 프로그램입니다.

	Created by zelon (Kim, Jinwook in Korea)

	Project Homepage : http://zviewer.wimy.com

	Creator Homepage : http://www.wimy.com


	주요 기능은 다음과 같습니다.

	- 다양한 확장자를 가진 이미지 파일을 볼 수 있습니다.

	- 탐색기에서 오른쪽 버튼을 통한 미리보기가 가능합니다.

	- 이미지 파일들을 캐시해두어 빠른 이미지 보기가 가능합니다.

	- 실행 크기가 작아 빨리 로딩됩니다.

	- 이미지 파일을 다른 형식으로 변환이 가능합니다.

	- 화면 크기에 맞게 확대/축소하여 이미지를 볼 수 있습니다.

*/

#include "stdafx.h"
#include "src/ZMain.h"
#include "src/ZFileExtDlg.h"
#include "src/ZResourceManager.h"
#include "src/ZCacheImage.h"
#include "src/MainWindow.h"

#include "../commonSrc/LogManager.h"
#include "../commonSrc/MessageManager.h"

#ifdef _DEBUG
#include "vld/vld.h"
#endif



/// Entry point
int APIENTRY _tWinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance ,LPTSTR lpszCmdParam,int nCmdShow)
{
	// 기본적인 언어팩은 프로젝트에 있는 영어이다.
	ZResourceManager::GetInstance().SetHandleInstance(hInstance);

	HINSTANCE hLang = NULL;

	// debug mode 에서는 항상 영어모드(언어팩 테스트를 위해서)
#ifndef _DEBUG
	if ( GetSystemDefaultLangID() == 0x0412 )
	{
		CMessageManager::getInstance().SetLanguage(eLanguage_KOREAN);

		hLang = LoadLibrary(TEXT("language/korean.dll"));

		if ( hLang )
		{
			ZResourceManager::GetInstance().SetHandleInstance(hLang);
		}
		else
		{
			assert(hLang != NULL);
		}
	}
#endif

	/// 파일 확장자를 연결하라는 거면
	if ( _tcscmp(lpszCmdParam, TEXT("/fileext")) == 0 )	
	{
		int iRet = MessageBox(HWND_DESKTOP, GetMessage(TEXT("REG_FILE_TYPE")), TEXT("ZViewer"), MB_YESNO);

		if ( iRet == IDYES )
		{
			ZFileExtDlg::GetInstance().SaveExtEnv();
		}
		return 0;
	}
	else if ( _tcscmp(lpszCmdParam, TEXT("/freezvieweragent")) == 0 )	// uninstall 할 때 ZViewerAgent 를 unload 한다.
	{
		CoFreeUnusedLibraries();
		return 0;
	}

	tstring strCmdString;

	// 쉘에서 보낼 때는 따옴표로 둘러싸서 준다. 그래서 따옴표를 제거한다.
	if ( _tcslen(lpszCmdParam) > 0 )
	{
		// 만약 따옴표를 포함하고 있으면(바탕화면에서 보냈을 때)
		size_t iLen = _tcslen(lpszCmdParam);
		for ( unsigned int i=0; i<iLen; ++i)
		{
			if ( lpszCmdParam[i] == '\"')
			{
				continue;
			}
			strCmdString.push_back(lpszCmdParam[i]);
		}
	}

	// ~1 형식으로 긴 파일명이 온다면, 원래 긴 패스를 얻는다.
	TCHAR szTemp[MAX_PATH] = { 0 };
	GetLongPathName(strCmdString.c_str(), szTemp, MAX_PATH);

	strCmdString = szTemp;

	tstring strInitArg = strCmdString;

	//MessageBox(HWND_DESKTOP, strInitArg.c_str(), "sf", MB_OK);

#ifdef _DEBUG
	if ( strInitArg.empty() )
	{
		strInitArg = TEXT("..\\SampleImages\\zviewer_jpg.jpg");
	}
	//strInitArg = "C:\\A.bmp";
#endif
	ZImage::StartupLibrary();

	ZMain::GetInstance().SetInitArg(strInitArg);
	ZMain::GetInstance().SetInstance(hInstance);

	CMainWindow aWindow;

	HWND hWnd = aWindow.Create(hInstance, HWND_DESKTOP, nCmdShow);

	/// Add Timer
	enum
	{
		eTimerValue = 9153
	};
	UINT_PTR timerPtr = SetTimer(hWnd, eTimerValue, 100, NULL);

	if ( timerPtr == 0 )
	{
		MessageBox(hWnd, GetMessage(TEXT("")), TEXT("ZViewer"), MB_OK);
		return 0;
	}

	WPARAM wParam = aWindow.MsgProc();

	/*
	/// KillTimer
	if ( timerPtr != 0 )
	{
		BOOL bRet = KillTimer(hWnd, eTimerValue);

		_ASSERTE(bRet);
	}
	*/

	//
	if ( hLang )
	{
		if ( FALSE == FreeLibrary(hLang) )
		{
			assert(!"Can't free language dll.");
		}
	}

	ZCacheImage::GetInstance().CleanUp();
	CLogManager::getInstance().CleanUp();
	ZImage::CleanupLibrary();

	return (int)wParam;
}
