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
//  File         : utils.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#ifndef __UTILS_H__
#define __UTILS_H__



#ifdef __cplusplus
extern "C" {
#endif


double getTimeMS();

const std::string GetDateTime();
const std::string GetDateTime2();
const std::string GetDateTime3();

struct timeval GetTimeVal();


#ifdef __cplusplus
}
#endif


#endif /* __UTILS_H__ */

