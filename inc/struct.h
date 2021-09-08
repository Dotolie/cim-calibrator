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
//  File         : struct.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __STRUCT_H__
#define __STRUCT_H__


#include <time.h>


#define MAX_CHANNEL			20
#define MAX_COEFFICIENTS	64
#define MAX_SAMPLES			65536


typedef struct {
	int d_nVersion;
	
	int d_nSampleRate;
	int d_nIntervalTime;
	int d_nCutFreq;
	int d_nTaps;
	int d_nDebug;
	
	bool d_bEnableCh0;
	bool d_bEnableCh1;
	bool d_bEnableCh2;
	bool d_bEnableCh3;	

	bool d_bUseFilter;
	

} __attribute__((packed)) DSP_t, *DSP_PTR_t;

typedef struct {
	int d_nOffset[MAX_CHANNEL];
	float d_fScale[MAX_CHANNEL];
} __attribute__((packed)) CAL_t, *CAL_PTR_t;


#endif /* __STRUCT_H__ */

