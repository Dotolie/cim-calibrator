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
//  File         : thread.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __RUNNABLE_H__
#define __RUNNABLE_H__

#include <pthread.h>
#include "base.h"

class Runnable : public CBase
{
protected:
	bool m_bRun;
	
public:
	Runnable():m_bRun(false)     {};
	Runnable(const char* pName): CBase(pName), m_bRun(false) {};
	virtual ~Runnable() {};
	

	virtual void Run() = 0;
	virtual void Stop() = 0;

	void SetRunBit(bool bit) { m_bRun = bit; };
	bool IsRun() { return m_bRun; };
	
};


#endif  // __RUNNABLE_H__

