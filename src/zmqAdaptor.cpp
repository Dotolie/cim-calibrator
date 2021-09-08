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
//  File         : zmqAdaptor.cpp
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <cstring>
#include <sstream>
#include <time.h>
#include <netinet/in.h>
#include <arpa/inet.h>


#include "stdafx.h"
#include "struct.h"
#include "zmqAdaptor.h"
#include "iotBox.h"
#include "utils.h"

#define DEBUG
#include "debug.h"

#define	JSON_DELIMITER			",[]: {}\n\r\"	"
#define	PATH_TEMP0				"/sys/class/thermal/thermal_zone0/temp"

#define MAXLINE 				1024



extern CIotbox g_IotBox;



CZmqAdaptor::CZmqAdaptor(void* pTask) : Runnable(__func__)
{
	m_pIotBox = (CIotbox *)pTask;
	
	DBG_I_N("create\r\n" );

}

CZmqAdaptor::~CZmqAdaptor() 
{
	SetRunBit(false);


	
	DBG_I_N("destroy\r\n" );
}

void CZmqAdaptor::Stop()
{
	SetRunBit(false);
	
//	DBG_I_N("m_bRun=false\r\n");
}

void CZmqAdaptor::Run()
{
	int nRet = 0;
	int socketfd;
	struct sockaddr_in sockaddr;
	int socklen;
	fd_set readfds, oldfds;
	int maxfd;
	int fd_num;
	int readn, writen;
	char buf[MAXLINE];
	float fData[MAX_CHANNEL];

	if( (socketfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		DBG_E_R("can't open socket\r\n");
		return;
  		}
	sockaddr.sin_family = AF_INET;
	sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	sockaddr.sin_port = htons(9001);
	
	socklen = sizeof(sockaddr);

	if(connect(socketfd, (struct sockaddr *)&sockaddr, socklen) == -1)
	{		
		DBG_E_R("connect error\r\n");
		return;
	}
	
	// readfd set 0 
	FD_ZERO(&readfds);
	// socketfd에 해당하는 readfds의 비트를 1로 한다.
	FD_SET(socketfd, &readfds);
	// 0번째 인자를 1로 set한다
//	FD_SET(0, &readfds);
	
	maxfd = socketfd;
	oldfds = readfds;

	 
	SetRunBit(true);
	
	DBG_I_N("run : start \r\n");
	while(IsRun()) {
		char szTemp1[16];
		float fTemp1;
		FILE *fp = fopen(PATH_TEMP0, "r");
		fgets(szTemp1, sizeof(szTemp1), fp);
		fTemp1 = atof(szTemp1)/1000.0f;
		fclose(fp); 

		readfds = oldfds;
		fd_num = select(maxfd + 1, &readfds, NULL, NULL, NULL);

		if(FD_ISSET(socketfd, &readfds)) {	// 초기 연결인지 아닌지 검사를 한다.
			memset(buf, 0x00, MAXLINE);
			readn = read(socketfd, buf, MAXLINE);
			if(readn > 0)	{
				string szBuf = buf;
				int nPos = szBuf.find("{");
				int nLen =szBuf.length();
			
				if( nPos > 0 ) {
					string szValues= szBuf.substr(nPos+1, nLen-nPos-2);
					vector<string> vResults = split(szValues, '^');
					for(int i=0;i<vResults.size();i++) {
						vector<string> vValaues = split(vResults[i], '=');
						fData[i] = stof( vValaues[1] );
						}
					if ( m_pIotBox->m_nDisplay == 1 ) {
						printf("AVG ----0------1------2------3------4------5------6------7------8------9----- %.0f[C]\r\nCh=0 :", fTemp1);
						for(int i=0;i<10;i++) {
							m_pIotBox->m_nOffset[i] = (int)fData[i];
							printf("%6.0f,", fData[i]);
							}
						printf("\r\nCh=10:");
						for(int i=10;i<20;i++) {
							m_pIotBox->m_nOffset[i] = (int)fData[i];
							printf("%6.0f,", fData[i]);
							}
						printf("\r\n");
						
						}
					if ( m_pIotBox->m_nDisplay == 2 ) {
						printf("AVG --0----------1----------2----------3----------4----------5----------6----------7----------8----------9---------- %.0f[C]\r\nCh=0 :", fTemp1);
						for(int i=0;i<10;i++) {
							m_pIotBox->m_fScaleFix[i] = 5.0f/fData[i];
							printf("%10.3f,", fData[i]);
							}
						printf("\r\nCh=10:");
						for(int i=10;i<20;i++) {
							m_pIotBox->m_fScaleFix[i] = 5.0f/fData[i];
							printf("%10.3f,", fData[i]);
							}
						printf("\r\n");
						
						}
					}
				}
			}

		}
	close(socketfd);

	
	DBG_I_N("end of loop \r\n");

}

vector<string> CZmqAdaptor::split(string input, char delimiter)
{
    vector<string> answer;
    stringstream ss(input);
    string temp;

    while (getline(ss, temp, delimiter)) {
        answer.push_back(temp);
	    }
 
    return answer;
}

