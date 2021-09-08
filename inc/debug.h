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
//  File         : debug.h
//  Description  :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------

#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <assert.h>

#ifdef DEBUG0
#define DBG(fmt,...) 		printf(fmt, ##__VA_ARGS__);

#define DBG_I_N(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_W(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_R(fmt,...) 	printf("\033[7;31mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_E_N(fmt,...) 	printf("E:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_E_R(fmt,...) 	printf("\033[7;31mE:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_G(fmt,...)	printf("\033[7;32mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_Y(fmt,...)	printf("\033[7;33mW:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_P(fmt,...)	printf("\033[7;35mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_C(fmt,...)	printf("\033[7;36mI:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);

#define ASSERT(x)		assert(x);
#else
#define DBG(fmt,...) 

#define DBG_I_N(fmt,...)
#define DBG_I_W(fmt,...) 	printf("I:[%s:%s]" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_R(fmt...)
#define DBG_E_N(fmt...)
#define DBG_E_R(fmt,...) 	printf("\033[7;31mE:[%s:%s]\033[0m" fmt, __FILE__,__func__, ##__VA_ARGS__);
#define DBG_I_G(fmt,...)
#define DBG_I_Y(fmt,...)
#define DBG_I_P(fmt,...)
#define DBG_I_C(fmt,...)

#define ASSERT(x)
#endif



#endif /* __DEBUG_H__ */
