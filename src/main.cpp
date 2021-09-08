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
//  File         : main.cpp
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <math.h>


#include "version.h"
#include "msg.h"
#include "taskMgr.h"
#include "timerMgr.h"
#include "iotBox.h"

#define DEBUG
#include "debug.h"


using namespace std;



CTaskMgr g_TaskMgr;
CTimerMgr g_TimerMgr;
CIotbox g_IotBox;




static void signal_handler( int32_t sig )
{
	DBG_I_N("Aborted by signal %s (%d)..\n", (char*)strsignal(sig), sig);

	switch( sig )
	{
		case SIGINT :
			g_TaskMgr.SendMessage(TASK_ID_MAX, MSG_QUIT, 0, 0);
			break;
		case SIGTERM :
			g_TaskMgr.SendMessage(TASK_ID_MAX, MSG_QUIT, 0, 0);
			break;
		case SIGABRT :
			g_TaskMgr.SendMessage(TASK_ID_MAX, MSG_QUIT, 0, 0);
			break;
		default :
			break;
	}

//	exit(EXIT_FAILURE);
}


static void register_signal( void )
{
	signal( SIGINT,  signal_handler );
	signal( SIGTERM, signal_handler );
	signal( SIGABRT, signal_handler );
}


int main ( int argc, char** argv )
{
	DBG("=====> Calibrator Start v%s <=====\r\n",  VERSION);

//	register_signal();

	g_TaskMgr.Run();

	DBG("=====> Calibrator end <=====\r\n");
	
	return 0;
}


