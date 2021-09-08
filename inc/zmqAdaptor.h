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
//  File         : zmqAdaptor.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __ZMQADAPTOR_H__
#define __ZMQADAPTOR_H__

#include <string>
#include <vector>

#include "runnable.h"
#include "iotBox.h"

#define	MAX_FEATURES	64
#define BUF_SIZE		128



using namespace std;


class CZmqAdaptor: public Runnable
{
private:
	CIotbox *m_pIotBox;
	vector<string> split(string input, char delimiter);
		
public:
	CZmqAdaptor(void* pTask);
	virtual ~CZmqAdaptor();
	

	virtual void Run();
	virtual void Stop();

};

#endif  // __ZMQADAPTOR_H__
