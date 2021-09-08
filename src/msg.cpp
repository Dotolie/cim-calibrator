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
//  File           : msg.cpp
//  Description :
//  Author       : ywkim@mvtech.or.kr
//  Export       :
//  History      :
//
//------------------------------------------------------------------------------


#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "msg.h"

#define DEBUG
#include "debug.h"



CMessageQ::CMessageQ()
{
	DBG_I_N("create\r\n" );

	SetName("CMessageQ");
	
	pthread_mutex_init( &m_Mutex, NULL );	
}
CMessageQ::~CMessageQ()
{
	DBG_I_N("destroy\r\n" );
	
	pthread_mutex_unlock( &m_Mutex );
	pthread_mutex_destroy( &m_Mutex );
}

int CMessageQ::push(MSG msg)
{
	int nRet = -1;

	if( m_count < MAX_Q_SIZE ) {
		pthread_mutex_lock( &m_Mutex );

		m_buffer[m_in] = msg;
		m_in = (m_in+1)%MAX_Q_SIZE;
		m_count++;
		
		pthread_mutex_unlock( &m_Mutex );
		nRet = 0;		
//		DBG_I_N("msg=0x%x, count=%d\r\n", msg.message, m_count);
		}
	
	return nRet;
}
int CMessageQ::pop(MSG& msg)
{
	int nRet = -1;
	MSG temp;
	

	if( m_count > 0 ) {	
		pthread_mutex_lock( &m_Mutex );

		temp = m_buffer[m_out];	
		m_out = (m_out+1)%MAX_Q_SIZE;
		m_count--;
		
		pthread_mutex_unlock( &m_Mutex );
		msg = temp;
		nRet = 0; 
//		DBG_I_N("msg=0x%x, rest=%d\r\n", temp.message, m_count);
		}
	return nRet;
}
int CMessageQ::get(MSG& data)
{
	MSG temp;
	if( m_count > 0 ) {	
		temp = m_buffer[m_out];
		data = temp;
		return 0; 
		}

	return -1;
}
bool CMessageQ::IsEmpty()
{
	if(abs(m_in == m_out)){
		return true;
		}
	else {
		return false;
		}
}
bool CMessageQ::IsFull()
{
	if(m_out==((m_in+1)%MAX_Q_SIZE)) {
		DBG_I_N("m_in=%d, m_out=%d\r\n", m_in, m_out);
		return true;
		}
	else{
		return false;
		}
} 
int CMessageQ::GetSize()
{
	return m_count;
}




