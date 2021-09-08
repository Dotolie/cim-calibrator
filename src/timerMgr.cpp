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
//  File        : TimerMgr.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#include "msg.h"
#include "timerMgr.h"
#include "taskMgr.h"


#define DEBUG
#include "debug.h"



#define MY_TIMER_SIGNAL		SIGRTMIN
#define ONE_MSEC_TO_NSEC	1000000
#define ONE_SEC_TO_NSEC		1000000000

extern CTaskMgr g_TaskMgr;
extern CTimerMgr g_TimerMgr;




static void timer_handler(int sig, siginfo_t *si, void *context)
{
	int uid = si->si_value.sival_int;
	int taskId = 0;
	int lParam = 0;
	TimerInfo *pTimerInfo = NULL;
	
	pTimerInfo = g_TimerMgr.FindTimerInfo(uid);
	taskId = pTimerInfo->m_taskId;
	lParam = pTimerInfo->m_lParam;
	
//	DBG_I_N("taskId=0x%x uid=%d\r\n", taskId, uid );

	g_TaskMgr.SendMessage(taskId, MSG_TIMER, uid, lParam);
}

CTimerMgr::CTimerMgr() 
{
	DBG_I_N("create \r\n");

	setSignal();
}

CTimerMgr::~CTimerMgr() 
{
	int rtn;

	timer_t tTimerId;
	map<int, TimerInfo*>::iterator it;
	TimerInfo *pTimerInfo = NULL;

	for( it=m_mTimerInfo.begin(); it !=m_mTimerInfo.end(); ++it) {
	//	key = it->first;
		pTimerInfo = it->second;
		tTimerId = pTimerInfo->m_TmId;
		rtn = timer_delete(tTimerId);
		if (rtn != 0) {
			DBG_I_N("err delTimer \r\n");
			}

		DBG_I_N("delete TimerInfo, tTImerId=%p, pTimerInfo=%p\r\n", tTimerId, pTimerInfo );
		}

	DBG_I_N("destroy \r\n");
}

bool CTimerMgr::setSignal()
{
	int rtn;
	struct sigaction sa;

	sa.sa_flags 	= SA_SIGINFO;
	sa.sa_sigaction = timer_handler;

	sigemptyset(&sa.sa_mask);

	rtn = sigaction(MY_TIMER_SIGNAL, &sa, NULL);

	if (rtn == -1)
	{
		return false;
	}

	DBG_I_N("registered \r\n");

	return true;	
}

bool CTimerMgr::addTimer(int taskid, long intv, int uid, int lParam)
{
	int rtn;
	timer_t tTimerId;
	struct sigevent sigEvt;

	/* Create Timer */

	memset(&sigEvt, 0x00, sizeof(sigEvt));

	sigEvt.sigev_notify			 = SIGEV_SIGNAL;
	sigEvt.sigev_signo			 = MY_TIMER_SIGNAL;
	sigEvt.sigev_value.sival_int = uid;

	rtn = timer_create(CLOCK_REALTIME, &sigEvt, &tTimerId);
	if (rtn != 0) {
		DBG_I_N("fail timer_create \r\n");
		return false;
		}

	/* Set Timer Interval */

	long nano_intv;
	struct itimerspec its;

	nano_intv = intv;// * ONE_MSEC_TO_NSEC;

	// initial expiration
	its.it_value.tv_sec 	= nano_intv / 1000;
	its.it_value.tv_nsec 	= (nano_intv % 1000 ) * 1000000;
//	DBG_I_N("timer_settime sec=%lu, nsec=%lu\r\n",its.it_value.tv_sec,  its.it_value.tv_nsec);

	// timer interval
	its.it_interval.tv_sec	= its.it_value.tv_sec;
	its.it_interval.tv_nsec	= its.it_value.tv_nsec;
	
	rtn = timer_settime(tTimerId, 0, &its, NULL);

	if (rtn != 0) {
		DBG_I_N("fail timer_settime \r\n");
		return false;
		}

	/* Save Timer Inforamtion */
	TimerInfo *pTimerInfo = new TimerInfo;
	
	pTimerInfo->m_TmId 		= tTimerId;
	pTimerInfo->m_taskId	= taskid;
	pTimerInfo->m_lParam	= lParam;

	AddTimerInfo(uid, pTimerInfo);

	DBG_I_N("addTimer: uid=%d, timerId=%p, tv=%ld, tv_n=%ld\r\n", uid, tTimerId, its.it_value.tv_sec,  its.it_value.tv_nsec);
		

	return true;
}

bool CTimerMgr::setTimer(int taskId, long intv, int uid, int lParam)
{
	TimerInfo *pTimerInfo = FindTimerInfo(uid);

	if( pTimerInfo != NULL ) {
		int rtn;		
		long nano_intv;
		struct itimerspec its;
		
		nano_intv = intv;// * ONE_MSEC_TO_NSEC;
		
		// initial expiration
		its.it_value.tv_sec 	= nano_intv / 1000;
		its.it_value.tv_nsec	= (nano_intv % 1000 ) * 1000000;
		// timer interval
		its.it_interval.tv_sec	= its.it_value.tv_sec;
		its.it_interval.tv_nsec = its.it_value.tv_nsec;
		
		rtn = timer_settime(pTimerInfo->m_TmId, 0, &its, NULL);

		ASSERT(rtn == 0);
		if (rtn != 0) {
			DBG_I_N("fail timer_settime \r\n");
			return false;
			}
		
		/* Save Timer Inforamtion */
		pTimerInfo->m_taskId	= taskId;
		pTimerInfo->m_lParam	= lParam;
		
		DBG_I_N("setTimer: uid=%d, t=%ld, timerId=%p\r\n", uid, intv, pTimerInfo->m_TmId );

		return true;
		}
	else {
		return false;
		}
}

bool CTimerMgr::delTimer(int uid)
{
	int rtn;
	timer_t tTimerId;
	TimerInfo *pTimerInfo = FindTimerInfo(uid);

	if ( pTimerInfo != NULL ) {
		tTimerId = pTimerInfo->m_TmId;
		rtn = timer_delete(tTimerId);

		if (rtn != 0)
		{
			DBG_I_N("err delTimer \r\n");
			return false;
		}

		DelTimerInfo(uid);


		DBG_I_N("delTimer: uid=%d, tTimerId=%p\r\n",uid, tTimerId);
		return true;
		}
	else {
		return false;
		}
}

int CTimerMgr::AddTimerInfo(const int uid, TimerInfo* pTimerInfo)
{
	int nRet = 0;

	m_mTimerInfo.insert(pair<int,TimerInfo*>(uid, pTimerInfo));

//	DBG_I_N("add timerInfo: uid=%d\r\n", uid);

	return nRet;
}

int CTimerMgr::DelTimerInfo(const int uid)
{
	int nRet = 0;

	nRet = m_mTimerInfo.erase(uid);

//	DBG_I_N("del timerInfo: uid=%d, nRet=%d\r\n", uid, nRet);

	return nRet;
}

TimerInfo* CTimerMgr::FindTimerInfo(const int uid)
{
	map<int, TimerInfo*>::iterator it;
	TimerInfo *pTimerInfo = NULL;

	it = m_mTimerInfo.find(uid);
	if( it != m_mTimerInfo.end()) {
		pTimerInfo = it->second;
//		DBG_I_N("find timerInfo, uid=%d, pTimerInfo=0x%x\r\n", uid, pTimerInfo);
		}

	return pTimerInfo;
}

