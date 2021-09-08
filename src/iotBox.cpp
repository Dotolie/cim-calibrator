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
//  File         : iotBox.cpp
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/time.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <linux/if_ether.h>
#include <linux/sockios.h>
#include <linux/ethtool.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <ifaddrs.h>
//#include <iwlib.h>


#include "object.h"
#include "taskMgr.h"
#include "timerMgr.h"
#include "iotBox.h"
#include "version.h"
#include "INIReader.h"
#include "utils.h"
#include "zmqAdaptor.h"
#include "consol.h"


#define DEBUG
#include "debug.h"




#define	TIMER_IOTBOX_ID_0 		(TASK_ID_IOTBOX + 1)


extern CTimerMgr g_TimerMgr;
extern CTaskMgr g_TaskMgr;
extern CIotbox 	g_IotBox;


CIotbox::CIotbox() : CBase(__func__)
{
	DBG_I_N("create id=0x%p\r\n", this);
	
	g_TaskMgr.AddTask(TASK_ID_IOTBOX, this);

	m_nDisplay = 0;
}

CIotbox::~CIotbox() 
{
	g_TaskMgr.DelTask(TASK_ID_IOTBOX);

	DBG_I_N("destroy \r\n");
}



int CIotbox::MsgProc(int message, long wparam, long lparam)
{
	int nRet = 0;
	
	switch(message) {
	case MSG_CREATE:
		nRet = On_MSG_CREATE( wparam, lparam);
		break;
	case MSG_EVENT:
		nRet = On_MSG_EVENT( wparam, lparam);
		break;	
	case MSG_QUIT:
		nRet = On_MSG_QUIT( wparam, lparam);
		break;
	default:
		break;
		}

	return nRet;
}

int CIotbox::On_MSG_CREATE( long wparam, long lparam )
{
	char strTemp[32] = {0,};
	char strTemp2[32] = {0,};

	DBG_I_Y("w=%d, l=%d\r\n", wparam, lparam);
#if 0
	if( getMac(strTemp, strTemp2) > -1 ) {
		strcpy(m_Device.d_szMac, strTemp);
		strcpy(m_Device.d_szMac2, strTemp2);
		}
	else {
		strcpy(m_Device.d_szMac, "no mac");
		strcpy(m_Device.d_szMac2, "no mac");		
		}

	m_nLink = getLink();


	getSsid("wlan0");
	scanIp4wifi("wlan0");
#endif

	m_pThread = new Thread( new CZmqAdaptor(this));
//	m_pThread->SetCoreNum(CORE_NUM_2);
	m_pThread->Start();

	m_pThreadConsol = new Thread( new CConsol(this));
//	m_pThreadConsol->SetCoreNum(CORE_NUM_1);
	m_pThreadConsol->Start();
	
	return 0;
}


int CIotbox::On_MSG_QUIT( long wparam, long lparam )
{

	if (m_pThread != NULL) {
		m_pThread->Stop();
		delete m_pThread;
		m_pThread = NULL;
		}
	if (m_pThreadConsol != NULL) {
		m_pThreadConsol->Stop();
		delete m_pThreadConsol;
		m_pThreadConsol = NULL;
		}


	DBG_I_P("w=%d, l=%d\r\n", wparam, lparam);
	
	return 0;
}

int CIotbox::On_MSG_EVENT( long wparam, long lparam )
{
	
	DBG_I_G("w=%d, l=%d\n", wparam, lparam);

	switch(wparam) {
		case 1:
			m_pThread->Stop();
			usleep(1000000);
			m_pThread->Start();
			break;
		case 2:
			m_pThread->Start();
			break;
		default:
			break;
		}
	
	return 0;
}



