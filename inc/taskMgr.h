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
//  File           : taskMgr.h
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __TASKMGR_H__
#define __TASKMGR_H__


#include <map>
#include "stdafx.h"
#include "object.h"
#include "thread.h"
#include "base.h"
#include "msg.h"
#include "error.h"

using namespace std;

class CTaskMgr : public CBase
{     
protected:
	MSG		m_msg;
	CMessageQ m_MsgQ;
	map<int, CObject *> m_mObjs;
	

public:	
	CTaskMgr();
	~CTaskMgr();

	void Run();
	int PeekMessage(MSG& msg, int a, int b, int c);
	int SendMessage(int key, int message, long wparam, long lparam);
	
	int AddTask(const int, CObject *);
	int DelTask(const int);
	CObject* FindTask(const int);
	int ClearTasks();	

}; 


#endif /* __TASKMGR_H__ */

