/********************************************************************
*
* Created by zelon(Kim, Jinwook Korea)
* 
*   2005. 5. 7
*	MoveToDlg.cpp
*
*                                       http://www.wimy.com
*********************************************************************/

#include "stdafx.h"
#include "SelectToFolderDlg.h"

#include "resource.h"
#include "ZResourceManager.h"
#include "ZMain.h"

CSelectToFolderDlg * pThis = NULL;

CSelectToFolderDlg::CSelectToFolderDlg()
{
	pThis = this;
}

CSelectToFolderDlg::~CSelectToFolderDlg()
{
}

bool CSelectToFolderDlg::DoModal()
{
	if ( IDOK == DialogBox(ZResourceManager::GetInstance().GetHInstance(), MAKEINTRESOURCE(IDD_MOVE_DIALOG), ZMain::GetInstance().GetHWND(), MoveToDlgPrc) )
	{
		return true;
	}
	return false;
}

void CSelectToFolderDlg::OnBrowserButton() const
{

}

int CALLBACK CSelectToFolderDlg::MoveToDlgPrc(HWND hWnd,UINT iMessage,WPARAM wParam,LPARAM /*lParam*/)
{
	static tstring strLastFolder = TEXT("");

	switch(iMessage)
	{
	case WM_INITDIALOG:
		{
			pThis->m_hWnd = hWnd;

			if ( !strLastFolder.empty() )
			{
				pThis->SetFolder(strLastFolder);
			}
		}
		return TRUE;

	case WM_COMMAND:
		{
			switch ( wParam )
			{
			case IDOK:
				{
					TCHAR szTemp[MAX_PATH];
					GetDlgItemText(hWnd, IDC_EDIT_MOVE_TO_FOLDER, szTemp, sizeof(szTemp) / sizeof(TCHAR) );

					pThis->m_strMoveToFolder = szTemp;
					strLastFolder = szTemp;

					::EndDialog(hWnd, IDOK);
				}

				break;

			case IDCANCEL:
				::EndDialog(hWnd, IDCANCEL);
				break;

			case IDC_BUTTON_SELECT_FOLDER:
				{
					TCHAR szFolder[MAX_PATH] = { 0 };

					if ( SelectFolder(hWnd, szFolder) )
					{
						pThis->SetFolder(szFolder);
					}
				}
				break;
			}
		}
		break;

	case WM_DESTROY:
		::EndDialog(hWnd, 0);
		return 0;
	}
	return FALSE;
}

void CSelectToFolderDlg::SetFolder(const tstring & strFolder)
{
	m_strMoveToFolder = strFolder;

	SetDlgItemText(m_hWnd, IDC_EDIT_MOVE_TO_FOLDER, strFolder.c_str());
}