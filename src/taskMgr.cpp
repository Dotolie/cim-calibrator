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
//  File           : TaskMgr.cpp
//  Description :
//  Author      : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

#include "thread.h"
#include "taskMgr.h"
#include "timerMgr.h"

#define DEBUG
#include "debug.h"



using namespace std;






CTaskMgr::CTaskMgr()
{
	SetName("CTaskMgr");
	
	DBG_I_N("create id=0x%p\r\n", this);
}

CTaskMgr::~CTaskMgr()
{
	DBG_I_N("destroy\r\n");
}

void CTaskMgr::Run()
{

	int nRet = 0;
	CObject *hObj;
	MSG msg;
	map<int, CObject*>::iterator it;
	
	while( nRet == 0 && PeekMessage( msg, 0, 0, 0) ) {
		if( msg.message == MSG_QUIT ) {
			nRet = -1;
			continue;
			}
		
		hObj = FindTask(msg.key);
		if( hObj != NULL ) {
			nRet = hObj->MsgProc(msg.message, msg.wparam, msg.lparam);
			}
		else {
			DBG_I_N("hObj=NULL, msg=0x%x\r\n", msg.message);
			}
		}

	for( it =m_mObjs.begin(); it !=m_mObjs.end(); ++it) {
		int key = it->first;
		hObj = it->second;
		hObj->MsgProc(MSG_QUIT, 0, 0);
//		DBG_I_N("find Tasks, key=%d, pObj=0x%p\r\n", key, hObj);
		}

	DBG_I_R("TaskMgr::QUIT Reason=%d\r\n", nRet);

}

int CTaskMgr::PeekMessage(MSG& msg, int a, int b, int c)
{
	while(1) {
		if( !m_MsgQ.IsEmpty() ) {
			m_MsgQ.pop( msg);

//			DBG_I_N("gMsgQ msg=0x%x \r\n", msg.message );
			
//			if( msg.message == MSG_END )
//				return 0;

			break;
			}
		else {
			usleep(500);
//			DBG_I_N("gMsgQ empty \r\n");
			}
		}

	return 1;
}

int CTaskMgr::SendMessage(int key, int message, long wparam, long lparam)
{
	MSG msg;

	msg.message = message;
	msg.wparam = wparam;
	msg.lparam = lparam;
	msg.key = key;
	
//	DBG_I_N("msg=0x%x, w=0x%x, l=0x%x \r\n", message, wparam, lparam);		
	
	if( !m_MsgQ.IsFull() ) {
		m_MsgQ.push(msg);
		return 1;
		}
	
	return 0;
}

int CTaskMgr::AddTask(const int key, CObject *pObj)
{
	int nRet = 0;

	m_mObjs.insert(pair<int,CObject*>(key, pObj));

	SendMessage(key, MSG_CREATE, 1, 0);
//	DBG_I_N("add Task: key=%d, pobj=0x%x\r\n", key, pObj);

	return nRet;
}

int CTaskMgr::DelTask(const int key)
{
	int nRet = 0;

	nRet = m_mObjs.erase(key);

//	DBG_I_N("erase Task: key=%d, nRet=%d\r\n", key, nRet);

	return nRet;
}

CObject* CTaskMgr::FindTask(const int key)
{
	map<int, CObject*>::iterator it;
	CObject *pObj = NULL;

	it = m_mObjs.find(key);
	if( it != m_mObjs.end()) {
		pObj = it->second;
//		DBG_I_N("find Tasks, key=%d, pObj=0x%x\r\n", key, pObj);
		}

	return pObj;
}

int CTaskMgr::ClearTasks(void)
{
	int nRet = 0;
	
	m_mObjs.clear();
	
	DBG_I_N("clear Tasks\r\n");

	return nRet;
}

