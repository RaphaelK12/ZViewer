/********************************************************************
*
* Created by zelon(Kim, Jinwook Korea)
* 
*   2007. 7. 20
*	StringToValue.h
*
*                                       http://www.wimy.com
*********************************************************************/
#pragma once

#include "OptionFile.h"

class StringToValue
{
	enum eValueType
	{
		eValueType_INT,
		eValueType_BOOL,
	};

public:
	StringToValue(tstring & val, int * iValue)
	{
		_Init(val, iValue, eValueType_INT);
	}

	StringToValue(tstring & val, bool * bValue)
	{
		_Init(val, bValue, eValueType_BOOL);
	}


	void InsertValueToMap(iniMap & mapData);
	void InsertMapToValue(iniMap & mapData);
	const tstring & getString() const { return m_str; }

protected:

	void _Init(tstring & str, void * pData, eValueType valueType)
	{
		m_str = str;
		m_valueType = valueType;
		m_pData = pData;
	}

	void * m_pData;
	tstring m_str;
	eValueType m_valueType;
};

