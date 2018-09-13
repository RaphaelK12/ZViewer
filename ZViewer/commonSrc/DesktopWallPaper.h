﻿#pragma once

/// 윈도우의 바탕화면으로의 지정등을 쉽게 도와주는 클래스
class CDesktopWallPaper final
{
public:
	enum eDesktopWallPaperStyle
	{
		eDesktopWallPaperStyle_CENTER,
		eDesktopWallPaperStyle_TILE,
		eDesktopWallPaperStyle_STRETCH
	};

	/// 실제로 바탕화면을 바꾼다.
	static void SetDesktopWallPaper(const tstring & strBitmapFileName, eDesktopWallPaperStyle style);

	/// 바탕화면의 배경화면을 지운다.
	static void ClearDesktopWallPaper();

private:
	/// 바탕화면으로 지정될 파일이름. bmp 여야 한다.
	tstring m_strFileName;
	eDesktopWallPaperStyle m_eStyle;
};
