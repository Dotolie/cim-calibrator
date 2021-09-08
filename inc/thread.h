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
//  Module      :
//  File           : thread.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __THREAD_H__
#define __THREAD_H__


#include <pthread.h>
#include <string.h>
#include "runnable.h"


#define	CORE_NUM_1	1
#define	CORE_NUM_2	2
#define	CORE_NUM_3	4
#define	CORE_NUM_4	8
#define	CORE_NUM_5	16
#define	CORE_NUM_6	32



class Thread : public Runnable
{
public:
	Runnable* _runnable;
	unsigned long m_coreNum;

private:
	pthread_t _thread;
	
public:
	Thread();

	Thread(Runnable* pRunnable);
	~Thread();
	
	void Start();
	void Wait();
	virtual void Run();
	virtual void Stop();
	void SetCoreNum(unsigned long core);

private:	
	static void* Main(void* pInst);
	
};

#endif  // __THREAD_H__

