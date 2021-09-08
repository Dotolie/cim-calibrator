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
//  File         : utils.c
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#include <sys/time.h>
#include <time.h>
#include <string>

#include "utils.h"




using namespace std;


double getTimeMS()
{
	struct timespec tspec;
	clock_gettime(CLOCK_REALTIME, &tspec);

	double time_in_mill =
	 (tspec.tv_sec) * 1000 + (tspec.tv_nsec) / 1000000 ; // convert tv_sec & tv_nsec to millisecond


	return time_in_mill;
}

const string GetDateTime()
{
    time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
    struct tm  tstruct;
	char 	pBuf[80];
    tstruct = *localtime(&now);
    strftime(pBuf, sizeof(pBuf), "%Y%m%d-%H%M%S", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return pBuf;
}

const string GetDateTime2()
{
	struct tm *lt;
	struct timeval tv;
	char 	pBuf[80];

	gettimeofday(&tv, NULL);

	if((lt = localtime(&tv.tv_sec)) == NULL) {
		return "";
		}
	
	sprintf(pBuf, "%04d-%02d-%02d %02d:%02d:%02d.%06ld", 
		lt->tm_year + 1900, lt->tm_mon + 1, lt->tm_mday, lt->tm_hour, lt->tm_min, lt->tm_sec, tv.tv_usec);

	return pBuf;
}

const string GetDateTime3()
{
    time_t     now = time(0); //현재 시간을 time_t 타입으로 저장
    struct tm  tstruct;
	char 	pBuf[80];
    tstruct = *localtime(&now);
    strftime(pBuf, sizeof(pBuf), "%Y.%m.%d %H:%M:%S", &tstruct); // YYYY-MM-DD.HH:mm:ss 형태의 스트링

	return pBuf;
}

struct timeval GetTimeVal()
{
	struct timeval tv;

	gettimeofday(&tv, NULL);

	return tv;
}


