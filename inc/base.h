//------------------------------------------------------------------------------
//
//  Copyright (C) 2008 MVTech Co., Ltd. All Rights Reserved
//  MVTech Co.,Ltd. Proprietary & Confidential
//
//  Reproduction in whole or in part is prohibited without the written consent 
//  of the copyright owner. MVTech reserves the right to make changes 
//  without notice at any time. MVTech makes no warranty, expressed, 
//  implied or statutory, including but not limited to any implied warranty of 
//  merchantability or fitness for any particular purpose, or that the use will
//  not infringe any third party patent,  copyright or trademark. 
//  MVTech must not be liable for any loss or damage arising from its use.
//
//  Module       :
//  File         : base.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#ifndef __BASE_H__
#define __BASE_H__

#include <string.h>

class CBase
{     
private:
	char m_szName[128];
	CBase *m_pId;
	
public:
	CBase() { m_pId = this; };
	CBase(const char* pName) { m_pId = this; SetName(pName);};
	virtual ~CBase() {};

	
	void SetName(const char *szName) { strcpy(m_szName, szName); };
	char* GetName() { return m_szName; };
	void* GetId() { return m_pId; };
}; 


#endif /* __BASE_H__ */

