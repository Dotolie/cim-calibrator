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
//  File         : minner.cpp
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <termios.h>

#include "stdafx.h"
#include "consol.h"
#include "taskMgr.h"
#include "INIReader.h"
#include "version.h"



#define DEBUG
#include "debug.h"


#define CAL_FILE_NAME		"/home/root/app/cal.ini"


extern CTaskMgr g_TaskMgr;



CConsol::CConsol(void *pTask) : Runnable(__func__)
{
	m_pIotBox = (CIotbox *)pTask;

	ReadCal();
	
	DBG_I_N("create\r\n" );
}

CConsol::~CConsol() 
{
	SetRunBit(false);
	
	DBG_I_N("destroy Id=0x%p\r\n", GetId() );
}

void CConsol::Stop()
{
	SetRunBit(false);
	
//	DBG_I_N("m_bRun=false\r\n");
}

void CConsol::Run()
{
	int nRet = 0;
	int nRet1 = 0;

	char c,c2;
	
	SetRunBit(true);
	
	DBG_I_N("run : start \r\n");
	
	while(IsRun()) {
		print_Menu();

		c=getKey();
		switch(c) {
			case '0':
				nRet = system("depmod");
				usleep(100000);
				nRet1 = system("modprobe fpga_spi");
				printf("\r\n드라이버를 설치 완료 했습니다.%d %d", nRet, nRet1);
				usleep(1000000);
				fflush(stdout);
				nRet = system("systemctl --now enable sdaq");
				g_TaskMgr.SendMessage(TASK_ID_IOTBOX, MSG_EVENT, 1, 0);
				printf("\r\n데몬을 시작 했습니다.");
				fflush(stdout);
				usleep(1500000);
				break;
			case '1':
				SaveCal();
				nRet = system("sync");
				nRet = system("systemctl restart sdaq");
				g_TaskMgr.SendMessage(TASK_ID_IOTBOX, MSG_EVENT, 1, 0);
				printf("\r\n 보정파일 초기화 했습니다.");
				fflush(stdout);				
				usleep(1500000);
				break;
			case '2':
				m_pIotBox->m_nDisplay = 1;
				c=getKey();
				m_pIotBox->m_nDisplay = 0;
				printf("\r\n오프셋:%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n      :%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d\r\n", 
					m_pIotBox->m_nOffset[0],m_pIotBox->m_nOffset[1],m_pIotBox->m_nOffset[2],m_pIotBox->m_nOffset[3],
					m_pIotBox->m_nOffset[4],m_pIotBox->m_nOffset[5],m_pIotBox->m_nOffset[6],m_pIotBox->m_nOffset[7],
					m_pIotBox->m_nOffset[8],m_pIotBox->m_nOffset[9],m_pIotBox->m_nOffset[10],m_pIotBox->m_nOffset[11],
					m_pIotBox->m_nOffset[12],m_pIotBox->m_nOffset[13],m_pIotBox->m_nOffset[14],m_pIotBox->m_nOffset[15],
					m_pIotBox->m_nOffset[16],m_pIotBox->m_nOffset[17],m_pIotBox->m_nOffset[18],m_pIotBox->m_nOffset[19]
					);
				printf("\r\n오프셋을 파일에 저장할까요? [y/N] ");
				fflush(stdout);
				c = getKey();
				if( c=='y' || c=='Y') {
					SaveOffsetCal();
					nRet = system("sync");
					nRet = system("systemctl restart sdaq");
//					usleep(100000);
					g_TaskMgr.SendMessage(TASK_ID_IOTBOX, MSG_EVENT, 1, 0);
					}
				break;
			case '3':
				printf("\r\n배율 0.000564 로 파일에 저장할까요? [y/N] ");
				fflush(stdout);
				for(int i=0;i<20;i++) {
					m_pIotBox->m_fScaleFix[i]=0.000564f;
					}
				c2 = getKey();
				if( c2=='y' || c2=='Y' ) {
					SaveScaleCal();
					nRet = system("sync");
					nRet = system("systemctl restart sdaq");
//					usleep(100000);
					g_TaskMgr.SendMessage(TASK_ID_IOTBOX, MSG_EVENT, 1, 0);
					}
				break;
			case '4':
				m_pIotBox->m_nDisplay = 2;
				c=getKey();
				m_pIotBox->m_nDisplay = 0;
				printf("\r\nscale:%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f\r\n     :%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f,%10.6f\r\n", 
					m_pIotBox->m_fScaleFix[0],m_pIotBox->m_fScaleFix[1],m_pIotBox->m_fScaleFix[2],m_pIotBox->m_fScaleFix[3],
					m_pIotBox->m_fScaleFix[4],m_pIotBox->m_fScaleFix[5],m_pIotBox->m_fScaleFix[6],m_pIotBox->m_fScaleFix[7],
					m_pIotBox->m_fScaleFix[8],m_pIotBox->m_fScaleFix[9],m_pIotBox->m_fScaleFix[10],m_pIotBox->m_fScaleFix[11],
					m_pIotBox->m_fScaleFix[12],m_pIotBox->m_fScaleFix[13],m_pIotBox->m_fScaleFix[14],m_pIotBox->m_fScaleFix[15],
					m_pIotBox->m_fScaleFix[16],m_pIotBox->m_fScaleFix[17],m_pIotBox->m_fScaleFix[18],m_pIotBox->m_fScaleFix[19]
					);
				printf("\r\n배율을 파일에 저장할까요? [y/N] ");
				fflush(stdout);
				c2 = getKey();
				if( c2=='y' || c2=='Y' ) {
					SaveScaleCal();
					nRet = system("sync");
					nRet = system("systemctl restart sdaq");
//					usleep(100000);
					g_TaskMgr.SendMessage(TASK_ID_IOTBOX, MSG_EVENT, 1, 0);
					}
				break;
			case '5':
				m_pIotBox->m_nDisplay = 2;
				c=getKey();
				m_pIotBox->m_nDisplay = 0;
				printf("\r\n측정값 검사가 완료되었습니다!");
				fflush(stdout);
				usleep(1500000);
				break;
			case 'x':
			case 'X':
				m_pIotBox->m_nDisplay = 0;
				g_TaskMgr.SendMessage(TASK_ID_MAX, MSG_QUIT, 0, 0);
				usleep(1000000);
				break;
			}
		usleep(1000);
		}
	
	DBG_I_N("end of loop \r\n");

}


char CConsol::print_Menu(void)
{
	int nRet = 0;
	
	nRet = system("clear");
	printf("===============================\n");
	printf("         선택 메뉴   v%s\n", VERSION);
	printf("===============================\n");
	printf("   0. 드라이버 설치 및 데몬시작\n");
	printf("   1. 보정파일 초기화\n");
	printf("   2. 옾셋 탐색 (0V기준)\n");
	printf("   3. 배율 설정 (0.000564)\n");
	printf("   4. 배율 탐색 (5V기준)\n");
	printf("   5. 측정값 검사\n");	
	printf("\n");
	printf("   x. 나가기\n");
	printf("===============================\n");
	printf("   선택하세요? ");
	fflush(stdout);

	return (char)nRet;
}


char CConsol::getKey()
{
	 int res;
	 struct termios oldtio,newtio;

	 char input = 0;
	
	tcgetattr(0,&oldtio); /* 현재 설정을 oldtio에 저장 */
	newtio = oldtio;
	/* set input mode (non-canonical, no echo,...) */
	newtio.c_lflag = ~(ECHO|ICANON);
	
	newtio.c_cc[VTIME]	  = 10;	 /* 문자 사이의 timer를 disable */
	newtio.c_cc[VMIN]	  = 0;	 /* 최소 5 문자 받을 때까진 blocking */
	
	tcflush(0, TCIFLUSH);
	tcsetattr(0,TCSANOW,&newtio);
	
	
	while (1) {		/* loop for input */
		res = read(0,&input,1);	/* 최소 5 문자를 받으면 리턴 */
//		printf(":%d:%x\n", res, input);

		if (res == 1 && input != 0) 
			break;
		}
	tcsetattr(0,TCSANOW,&oldtio);

	printf("%c\r\n\r\n", input);
	
	return input;
}
void CConsol::ReadCal()
{
	INIReader *pReader = new INIReader(CAL_FILE_NAME);
	if( pReader->ParseError() < 0 ) {
		DBG_E_R("cal.ini does't exist\r\n");
		}

	m_sCal.d_nOffset[0]	= pReader->GetInteger("ch0", "offset", 0);
	m_sCal.d_fScale[0] 	= pReader->GetInteger("ch0", "scale", 1);

	m_sCal.d_nOffset[1]	= pReader->GetInteger("ch1", "offset", 0);
	m_sCal.d_fScale[1] 	= pReader->GetInteger("ch1", "scale", 1);

	m_sCal.d_nOffset[2]	= pReader->GetInteger("ch2", "offset", 0);
	m_sCal.d_fScale[2] 	= pReader->GetInteger("ch2", "scale", 1);

	m_sCal.d_nOffset[3]	= pReader->GetInteger("ch3", "offset", 0);
	m_sCal.d_fScale[3] 	= pReader->GetInteger("ch3", "scale", 1);

	m_sCal.d_nOffset[4]	= pReader->GetInteger("ch4", "offset", 0);
	m_sCal.d_fScale[4] 	= pReader->GetInteger("ch4", "scale", 1);

	m_sCal.d_nOffset[5]	= pReader->GetInteger("ch5", "offset", 0);
	m_sCal.d_fScale[5] 	= pReader->GetInteger("ch5", "scale", 1);

	m_sCal.d_nOffset[6]	= pReader->GetInteger("ch6", "offset", 0);
	m_sCal.d_fScale[6] 	= pReader->GetInteger("ch6", "scale", 1);

	m_sCal.d_nOffset[7]	= pReader->GetInteger("ch7", "offset", 0);
	m_sCal.d_fScale[7] 	= pReader->GetInteger("ch7", "scale", 1);

	m_sCal.d_nOffset[8]	= pReader->GetInteger("ch8", "offset", 0);
	m_sCal.d_fScale[8] 	= pReader->GetInteger("ch8", "scale", 1);

	m_sCal.d_nOffset[9]	= pReader->GetInteger("ch9", "offset", 0);
	m_sCal.d_fScale[9] 	= pReader->GetInteger("ch9", "scale", 1);

	m_sCal.d_nOffset[10]= pReader->GetInteger("ch10", "offset", 0);
	m_sCal.d_fScale[10] = pReader->GetInteger("ch10", "scale", 1);

	m_sCal.d_nOffset[11]= pReader->GetInteger("ch11", "offset", 0);
	m_sCal.d_fScale[11] = pReader->GetInteger("ch11", "scale", 1);

	m_sCal.d_nOffset[12]= pReader->GetInteger("ch12", "offset", 0);
	m_sCal.d_fScale[12] = pReader->GetInteger("ch12", "scale", 1);

	m_sCal.d_nOffset[13]= pReader->GetInteger("ch13", "offset", 0);
	m_sCal.d_fScale[13] = pReader->GetInteger("ch13", "scale", 1);

	m_sCal.d_nOffset[14]= pReader->GetInteger("ch14", "offset", 0);
	m_sCal.d_fScale[14] = pReader->GetInteger("ch14", "scale", 1);

	m_sCal.d_nOffset[15]= pReader->GetInteger("ch15", "offset", 0);
	m_sCal.d_fScale[15] = pReader->GetInteger("ch15", "scale", 1);

	m_sCal.d_nOffset[16]= pReader->GetInteger("ch16", "offset", 0);
	m_sCal.d_fScale[16] = pReader->GetInteger("ch16", "scale", 1);

	m_sCal.d_nOffset[17]= pReader->GetInteger("ch17", "offset", 0);
	m_sCal.d_fScale[17] = pReader->GetInteger("ch17", "scale", 1);

	m_sCal.d_nOffset[18]= pReader->GetInteger("ch18", "offset", 0);
	m_sCal.d_fScale[18] = pReader->GetInteger("ch18", "scale", 1);

	m_sCal.d_nOffset[19]= pReader->GetInteger("ch19", "offset", 0);
	m_sCal.d_fScale[19] = pReader->GetInteger("ch19", "scale", 1);

	for(int i=0;i<MAX_CHANNEL;i++) {
		m_pIotBox->m_fScaleFix[i] = m_sCal.d_fScale[i];
		DBG("ch=%d, offset=%d, scale=%f\r\n", i, m_sCal.d_nOffset[i], m_sCal.d_fScale[i]);
		}
}

void CConsol::SaveOffsetCal()
{
	ofstream outFile;
	stringstream ss;

	m_sCal.d_nOffset[0] = m_pIotBox->m_nOffset[0];
	m_sCal.d_nOffset[1] = m_pIotBox->m_nOffset[1];
	m_sCal.d_nOffset[2] = m_pIotBox->m_nOffset[2];
	m_sCal.d_nOffset[3] = m_pIotBox->m_nOffset[3];
	m_sCal.d_nOffset[4] = m_pIotBox->m_nOffset[4];
	m_sCal.d_nOffset[5] = m_pIotBox->m_nOffset[5];
	m_sCal.d_nOffset[6] = m_pIotBox->m_nOffset[6];
	m_sCal.d_nOffset[7] = m_pIotBox->m_nOffset[7];
	m_sCal.d_nOffset[8] = m_pIotBox->m_nOffset[8];
	m_sCal.d_nOffset[9] = m_pIotBox->m_nOffset[9];
	m_sCal.d_nOffset[10] = m_pIotBox->m_nOffset[10];
	m_sCal.d_nOffset[11] = m_pIotBox->m_nOffset[11];
	m_sCal.d_nOffset[12] = m_pIotBox->m_nOffset[12];
	m_sCal.d_nOffset[13] = m_pIotBox->m_nOffset[13];
	m_sCal.d_nOffset[14] = m_pIotBox->m_nOffset[14];
	m_sCal.d_nOffset[15] = m_pIotBox->m_nOffset[15];
	m_sCal.d_nOffset[16] = m_pIotBox->m_nOffset[16];
	m_sCal.d_nOffset[17] = m_pIotBox->m_nOffset[17];
	m_sCal.d_nOffset[18] = m_pIotBox->m_nOffset[18];
	m_sCal.d_nOffset[19] = m_pIotBox->m_nOffset[19];

	ss << ";" << std::endl;
	ss << "; Generated cal.ini file by calibrator" << std::endl;
	ss << ";" << std::endl;
	ss << "; offset0 : -2048 ~ 2047" << std::endl;
	ss << "; scale   : -100.0 ~ 100.0"  << std::endl;
	ss << ";" << std::endl << std::endl;
	

	ss << "[ch0]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[0] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[0]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch1]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[1] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[1]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch2]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[2] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[2]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch3]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[3] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[3]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch4]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[4] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[4]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch5]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[5] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[5]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch6]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[6] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[6]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch7]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[7] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[7]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch8]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[8] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[8]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch9]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[9] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[9]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch10]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[10] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[10]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch11]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[11] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[11]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch12]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[12] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[12]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch13]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[13] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[13]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch14]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[14] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[14]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch15]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[15] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[15]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch16]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[16] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[16]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch17]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[17] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[17]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch18]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[18] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[18]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch19]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[19] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[19]  << std::endl;
	ss << "" << std::endl;

	outFile.open(CAL_FILE_NAME);
	outFile << ss.rdbuf();
	outFile.close();

	printf("\r\n오프셋을 저장 했습니다.\r\n");
	usleep(1000000);

}

void CConsol::SaveScaleCal()
{
	ofstream outFile;
	stringstream ss;

	m_sCal.d_fScale[0] = m_pIotBox->m_fScaleFix[0];
	m_sCal.d_fScale[1] = m_pIotBox->m_fScaleFix[1];
	m_sCal.d_fScale[2] = m_pIotBox->m_fScaleFix[2];
	m_sCal.d_fScale[3] = m_pIotBox->m_fScaleFix[3];	
	m_sCal.d_fScale[4] = m_pIotBox->m_fScaleFix[4];
	m_sCal.d_fScale[5] = m_pIotBox->m_fScaleFix[5];
	m_sCal.d_fScale[6] = m_pIotBox->m_fScaleFix[6];
	m_sCal.d_fScale[7] = m_pIotBox->m_fScaleFix[7];	
	m_sCal.d_fScale[8] = m_pIotBox->m_fScaleFix[8];
	m_sCal.d_fScale[9] = m_pIotBox->m_fScaleFix[9];
	m_sCal.d_fScale[10] = m_pIotBox->m_fScaleFix[10];
	m_sCal.d_fScale[11] = m_pIotBox->m_fScaleFix[11];	
	m_sCal.d_fScale[12] = m_pIotBox->m_fScaleFix[12];
	m_sCal.d_fScale[13] = m_pIotBox->m_fScaleFix[13];
	m_sCal.d_fScale[14] = m_pIotBox->m_fScaleFix[14];
	m_sCal.d_fScale[15] = m_pIotBox->m_fScaleFix[15];	
	m_sCal.d_fScale[16] = m_pIotBox->m_fScaleFix[16];	
	m_sCal.d_fScale[17] = m_pIotBox->m_fScaleFix[17];
	m_sCal.d_fScale[18] = m_pIotBox->m_fScaleFix[18];
	m_sCal.d_fScale[19] = m_pIotBox->m_fScaleFix[19];

	ss << ";" << std::endl;
	ss << "; Generated cal.ini file by calibrator" << std::endl;
	ss << ";" << std::endl;
	ss << "; offset0 : -2048 ~ 2047" << std::endl;
	ss << "; scale   : -100.0 ~ 100.0"  << std::endl;
	ss << ";" << std::endl << std::endl;
	

	ss << "[ch0]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[0] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[0]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch1]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[1] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[1]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch2]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[2] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[2]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch3]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[3] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[3]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch4]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[4] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[4]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch5]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[5] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[5]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch6]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[6] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[6]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch7]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[7] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[7]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch8]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[8] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[8]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch9]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[9] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[9]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch10]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[10] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[10]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch11]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[11] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[11]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch12]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[12] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[12]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch13]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[13] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[13]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch14]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[14] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[14]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch15]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[15] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[15]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch16]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[16] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[16]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch17]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[17] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[17]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch18]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[18] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[18]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch19]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[19] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[19]  << std::endl;
	ss << "" << std::endl;

	outFile.open(CAL_FILE_NAME);
	outFile << ss.rdbuf();
	outFile.close();

	
	printf("\r\n배율을 저장 했습니다.\r\n");
	usleep(1000000);

}



void CConsol::SaveCal()
{
	ofstream outFile;
	stringstream ss;

	m_sCal.d_fScale[0] = 1;
	m_sCal.d_fScale[1] = 1;
	m_sCal.d_fScale[2] = 1;
	m_sCal.d_fScale[3] = 1;	
	m_sCal.d_fScale[4] = 1;
	m_sCal.d_fScale[5] = 1;
	m_sCal.d_fScale[6] = 1;
	m_sCal.d_fScale[7] = 1;
	m_sCal.d_fScale[8] = 1;
	m_sCal.d_fScale[9] = 1;
	m_sCal.d_fScale[10] = 1;
	m_sCal.d_fScale[11] = 1;
	m_sCal.d_fScale[12] = 1;
	m_sCal.d_fScale[13] = 1;
	m_sCal.d_fScale[14] = 1;
	m_sCal.d_fScale[15] = 1;
	m_sCal.d_fScale[16] = 1;
	m_sCal.d_fScale[17] = 1;
	m_sCal.d_fScale[18] = 1;
	m_sCal.d_fScale[19] = 1;


	m_sCal.d_nOffset[0] = 0;
	m_sCal.d_nOffset[1] = 0;
	m_sCal.d_nOffset[2] = 0;
	m_sCal.d_nOffset[3] = 0;
	m_sCal.d_nOffset[4] = 0;
	m_sCal.d_nOffset[5] = 0;
	m_sCal.d_nOffset[6] = 0;
	m_sCal.d_nOffset[7] = 0;
	m_sCal.d_nOffset[8] = 0;
	m_sCal.d_nOffset[9] = 0;
	m_sCal.d_nOffset[10] = 0;
	m_sCal.d_nOffset[11] = 0;
	m_sCal.d_nOffset[12] = 0;
	m_sCal.d_nOffset[13] = 0;
	m_sCal.d_nOffset[14] = 0;
	m_sCal.d_nOffset[15] = 0;
	m_sCal.d_nOffset[16] = 0;
	m_sCal.d_nOffset[17] = 0;
	m_sCal.d_nOffset[18] = 0;
	m_sCal.d_nOffset[19] = 0;


	ss << ";" << std::endl;
	ss << "; Generated cal.ini file by calibrator" << std::endl;
	ss << ";" << std::endl;
	ss << "; offset0 : -2048 ~ 2047" << std::endl;
	ss << "; scale   :  -100.0 ~ 100.0"  << std::endl;
	ss << ";" << std::endl << std::endl;
	

	ss << "[ch0]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[0] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[0]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch1]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[1] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[1]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch2]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[2] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[2]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch3]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[3] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[3]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch4]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[4] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[4]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch5]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[5] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[5]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch6]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[6] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[6]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch7]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[7] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[7]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch8]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[8] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[8]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch9]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[9] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[9]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch10]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[10] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[10]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch11]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[11] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[11]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch12]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[12] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[12]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch13]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[13] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[13]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch14]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[14] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[14]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch15]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[15] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[15]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch16]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[16] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[16]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch17]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[17] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[17]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch18]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[18] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[18]  << std::endl;
	ss << "" << std::endl;

	ss << "[ch19]" << std::endl;
	ss << "offset = " << m_sCal.d_nOffset[19] << std::endl;
	ss << "scale = "   << m_sCal.d_fScale[19]  << std::endl;
	ss << "" << std::endl;

	outFile.open(CAL_FILE_NAME);
	outFile << ss.rdbuf();
	outFile.close();

}



