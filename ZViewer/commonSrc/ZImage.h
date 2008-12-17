/********************************************************************
*
* Created by zelon(Kim, Jinwook Korea)
*
*   2005. 5. 7
*	ZImage.h
*
*                                       http://www.wimy.com
*********************************************************************/

#pragma once

#include <cassert>
#include <list>


#include "../lib/freeimage/FreeImagePlus.h"
#include "CommonFunc.h"
#include "CommonDefine.h"

/// FreeImagePlus 의 클래스인 fipWinImage 를 래핑하여 함수 등을 쓰기 쉽게 하는 클래스
class ZImage
{
private:
	ZImage & operator=(const ZImage & );	///< 대입 연산자 금지
	ZImage(const ZImage & );				///< 복사 생성자 금지

public:
	ZImage();
	~ZImage();

	static bool StartupLibrary(){ return true; }
	static bool CleanupLibrary(){ return true; }
	static const tstring GetLibraryVersion()
	{
#ifdef _UNICODE
		return tstring(getWStringFromString(std::string(FreeImage_GetVersion())));
#else
		return tstring(FreeImage_GetVersion());
#endif
	}

	void Clear()
	{
		m_image.clear();
	}

	WORD GetWidth() const { return m_image.getWidth(); }
	WORD GetHeight() const { return m_image.getHeight(); }

	WORD GetOriginalWidth() const { return m_originalWidth; }
	WORD GetOriginalHeight() const { return m_originalHeight; }

	WORD GetBPP() const { return m_image.getBitsPerPixel(); }
	BYTE * GetData() { return m_image.accessPixels(); }
	BITMAPINFO * GetBitmapInfo() { return m_image.getInfo(); }
	bool IsValid() const { return (m_image.isValid() == TRUE); }

	bool CopyFromBitmap(HBITMAP hBitmap) { return (TRUE == m_image.copyFromBitmap(hBitmap)); }

	bool LoadFromMemory(fipMemoryIO & memBuffer, const tstring & strFilename)
	{
		if ( TRUE == m_image.loadFromMemory(memBuffer) )
		{
			_ProcAfterLoad(strFilename);
			return true;
		}
		return false;
	}

	bool LoadFromFile(const tstring & strFilename)
	{
		//DebugPrintf("LoadFromFile : %s", strFilename.c_str());

#ifndef _DEBUG
		try
		{
#endif
			if ( m_image.loadU(strFilename.c_str()) == TRUE )
			{
				_ProcAfterLoad(strFilename);
				return true;
			}
#ifndef _DEBUG
		}
		catch ( ... )
		{
			assert(false);
			return false;
		}
#endif
		return false;
	}

	void _ProcAfterLoad(const tstring & strFilename)
	{
		m_originalWidth = m_image.getWidth();
		m_originalHeight = m_image.getHeight();
		m_originalSize = m_image.getImageSize();
		m_strFilename = strFilename;
	}

	bool SaveToFile(const tstring & strFilename, int iFlag) const
	{
#ifdef _UNICODE
		return ( TRUE == m_image.saveU(strFilename.c_str(), iFlag));
#else
		return ( TRUE == m_image.save(strFilename.c_str(), iFlag));
#endif
	}

	void Draw(HDC hDC, RECT& rcDest) const { m_image.draw(hDC, rcDest); }

	void Rotate(double dAngle) { m_image.rotate(dAngle); }

	bool Resize(WORD newWidth, WORD newHeight)
	{
		if ( newWidth <=  0 || newHeight <= 0 )
		{
			assert(!"Resize to below 0 or 0");
			return false;
		}
		return ( m_image.rescale(newWidth, newHeight, FILTER_BOX) == TRUE );
	}

	bool ConvertTo32Bit() { return ( TRUE == m_image.convertTo32Bits() ); }

	long GetImageSize() const
	{
		return m_image.getImageSize();
	}

	long GetOriginalImageSize() const
	{
		return m_originalSize;
	}

	bool isTransparent() const
	{
		return (m_image.isTransparent() == TRUE);
	}

	const fipWinImage & getFipImage()
	{
		return m_image;
	}

	void GetExifList(std::list < TagData > & exifList);

	void AutoRotate();

	bool CopyToClipboard(HWND hWndNewOwner) { return (TRUE == m_image.copyToClipboard(hWndNewOwner)); }
	bool PasteFromClipboard() { return (TRUE == m_image.pasteFromClipboard()); }
private:
	mutable fipWinImage m_image;

	/// Image's original width - before resizing
	WORD m_originalWidth;

	/// Image's original height - before resizing
	WORD m_originalHeight;

	/// Image's original size - before resizing
	long m_originalSize;

	/// current loaded filename
	tstring m_strFilename;
};
