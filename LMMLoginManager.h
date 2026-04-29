
// LMMLoginManager.h: PROJECT_NAME 애플리케이션에 대한 주 헤더 파일입니다.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'pch.h'를 포함합니다."
#endif

#include "resource.h"		// 주 기호입니다.

#include "Common/ini/EZIni/EZIni.h"
#include "Common/log/SCLog/SCLog.h"

// CLMMLoginManagerApp:
// 이 클래스의 구현에 대해서는 LMMLoginManager.cpp을(를) 참조하세요.
//

class CLMMLoginManagerApp : public CWinApp
{
public:
	CLMMLoginManagerApp();

	//이 경로는 AutoPatcher(통합 버전)의 m_reg_path와 동일해야 한다.
#ifdef LINKMEMINE_10
	CString			m_reg_path = _T("SOFTWARE\\LinkMeMine.service\\LMMAgent");
#else//LINKMEMINE_30
	CString			m_reg_path = _T("SOFTWARE\\LinkMeMineSE\\LMMAgent");
#endif

	//config.ini 파일을 읽고 쓰지만 쓸 때 맨 마지막 위치에 기록되는 아쉬운 점이 있어서 읽는 용도로만 우선 사용한다.
	EZIni			m_ini;
	CString			m_server_ip;
	int				m_server_port;
	CString			m_config_path;

// 재정의입니다.
public:
	virtual BOOL InitInstance();

// 구현입니다.

	DECLARE_MESSAGE_MAP()
};

extern CLMMLoginManagerApp theApp;
extern CSCLog gLog;
