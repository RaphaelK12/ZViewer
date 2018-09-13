﻿#include "stdafx.h"
#include "resource.h"
#include "ZFileExtDlg.h"
#include "ZMain.h"
#include "ZResourceManager.h"

#include "ZFileExtReg.h"

#include "../commonSrc/CommonFunc.h"
#include "../commonSrc/ExtInfoManager.h"

ZFileExtDlg * pThis = nullptr;

ZFileExtDlg::ZFileExtDlg()
{
	pThis = this;
}

ZFileExtDlg & ZFileExtDlg::GetInstance()
{
	static ZFileExtDlg singleton;
	return singleton;
}

void ZFileExtDlg::ShowDlg()
{
	INT_PTR iRet = DialogBox(ZResourceManager::GetInstance().GetHInstance(), MAKEINTRESOURCE(IDD_FILE_EXT_DIALOG), ZMain::GetInstance().GetHWND(), (DLGPROC)FileExtDlgProc);

	if ( iRet == IDOK )
	{
		SaveExtEnv();
	}
}

int CALLBACK ZFileExtDlg::FileExtDlgProc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM /*lParam*/)
{
	switch(iMessage)
	{
	case WM_INITDIALOG:
		{
			//pThis->LoadExtEnv(hWnd);
		}
		return TRUE;

	case WM_COMMAND:
		{
			switch ( wParam )
			{
			case IDOK:
				EndDialog(hWnd, IDOK);
				break;

			case IDCANCEL:
				EndDialog(hWnd, IDCANCEL);
				break;
			}
		}
		break;

	case WM_DESTROY:
		EndDialog(hWnd, 0);
		return 0;
	}
	return FALSE;
}

/*
void ZFileExtDlg::LoadExtEnv(HWND hwnd)
{
	/// Registry 에서 설정되어 있는 값들을 받아온다.
	/// TODO: registry 에서 읽어온다.

	/// 읽어온 대로 checkbox 를 세팅한다.
	SendMessage(GetDlgItem(hwnd, (IDC_CHECK_BMP)), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hwnd, (IDC_CHECK_GIF)), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hwnd, (IDC_CHECK_PNG)), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hwnd, (IDC_CHECK_PSD)), BM_SETCHECK, BST_CHECKED, 0);
	SendMessage(GetDlgItem(hwnd, (IDC_CHECK_JPG)), BM_SETCHECK, BST_CHECKED, 0);
	//SendMessage(GetDlgItem(hwnd, MAKEINTRESOURCE(IDC_CHECK_BMP)), BM_SETCHECK, BST_CHECKED, 0);
}
*/

void ZFileExtDlg::SaveExtEnv()
{
	const auto& ext_settings = ExtInfoManager::GetInstance().ext_settings();

	tstring strProgramFolder(GetProgramFolder());

	tstring strIconDll = strProgramFolder;
	strIconDll += TEXT("ZViewerIcons.dll");

	for (auto it = begin(ext_settings); it != end(ext_settings); ++it)
	{
		const ExtSetting& ext_setting = *it;

		if (SetExtWithProgram(TEXT("ZViewer"), ext_setting.m_strExt,
							TEXT(""),	/// 프로그램 Full Path. 비워두면 현재 프로그램이다.
							strIconDll.c_str(),	/// 아이콘 프로그램
							ext_setting.m_numIconIndex	/// 아이콘 index
							) == false) 
		{
			assert(false);
		}
	}

	/// explorer 의 아이콘을 update 시킨다.
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
}

bool ZFileExtDlg::SetExtWithProgram(const tstring & strProgramName, const tstring & strExt, tstring strFullProgramPath, const tstring & strIcon, int iIconIndex)
{
	ZFileExtReg fileExtReg;


	if ( strFullProgramPath.empty() )
	{
		// get full file path to program executable file
		TCHAR szProgPath[MAX_PATH] = { 0 };
		::GetModuleFileName(NULL, szProgPath, sizeof(szProgPath) / sizeof(TCHAR));

		strFullProgramPath = szProgPath;
	}

	tstring strTempText;

	fileExtReg.m_strExtension = strExt.c_str();

	// 프로그램에게 인자를 넘겨줄 때의 full path 를 만든다.
	strTempText  = strFullProgramPath;
	strTempText += TEXT(" \"%1\"");
	fileExtReg.m_strShellOpenCommand = strTempText.c_str();
	fileExtReg.m_strDocumentShellOpenCommand = strTempText.c_str();

	// 레지스트리에 등록할 때의 프로그램의 이름과 확장자를 정한다.
	tstring strClassName = strProgramName;
	strClassName += TEXT(".");
	strClassName += strExt;
	fileExtReg.m_strDocumentClassName = strClassName.c_str();

	// 확장자에 맞는 기본 아이콘을 지정한다.
	if ( strIcon.size() <= 0 )
	{
		// 아이콘 프로그램을 지정하지 않으면 원래 프로그램의 첫번째 아이콘을 쓴다.
		strTempText  = strFullProgramPath;
		strTempText += TEXT(",0");
	}
	else
	{
		// 아이콘 프로그램을 지정했으면 icon 번호를 쓴다.
		if ( iIconIndex < 0 )
		{
			assert(iIconIndex >= 0 );
			iIconIndex = 0;
		}

		strTempText = strIcon;
		strTempText += TEXT(",");

		TCHAR szTemp[COMMON_BUFFER_SIZE] = { 0 };
		SPrintf(szTemp, COMMON_BUFFER_SIZE, TEXT("%d"), iIconIndex);
		strTempText += szTemp;
	}
	fileExtReg.m_strDocumentDefaultIcon = strTempText.c_str();

	// 설정된 값으로 레지스트리를 등록한다.
	fileExtReg.SetRegistries();

	return true;
}
