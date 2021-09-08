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
//  File         : consol.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __CONSOL_H__
#define __CONSOL_H__


#include "runnable.h"
#include "struct.h"
#include "iotBox.h"


using namespace std;

class CConsol: public Runnable
{	
private:
	CIotbox *m_pIotBox;
	CAL_t	m_sCal;
	
public:
	CConsol(void* pTask);
	virtual ~CConsol();
	

	virtual void Run();
	virtual void Stop();
	
private:
	char print_Menu();
	char getKey();
	void ReadCal();
	void SaveOffsetCal();
	void SaveScaleCal();
	void SaveCal();
};

#endif  // __CONSOL_H__