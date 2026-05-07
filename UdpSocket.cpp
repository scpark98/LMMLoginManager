#include "StdAfx.h"
#include "LMMLoginManager.h"
#include "LMMLoginManagerDlg.h"
#include "MessageDlg.h"
#include "UdpSocket.h"
#include "AgentControl.h"
#include "Log.h"

#include "Common/Functions.h"

const int MAX_BUF = 256;
const int UDP_PORT = 20177;


CUdpSocket::CUdpSocket(void)
{
}


CUdpSocket::~CUdpSocket(void)
{
}


BOOL CUdpSocket::Create()
{
	int plusCnt = 0;
	while(TRUE)
	{
		if (CAsyncSocket::Create(UDP_PORT + plusCnt, SOCK_DGRAM))
		{
			break;
		}
		++plusCnt;
	}

	CString strUDPPort;
	strUDPPort.Format(_T("%d"), UDP_PORT, plusCnt);
	Config::SaveAgentUDPPort(strUDPPort);
	return TRUE;
}


void CUdpSocket::OnReceive(int nErrorCode)
{
	//20240723 scpark LMMAgent에서는 udpSocket.Send(LM_AGENT_LOGIN_OK);로 보내고 (cmd_id = 102)
	//아래 주석처리 한 기존 코드로는 Li_numRead가 계속 -1만 넘어온다.
	//SE와 동일하게 LMMAgent가 보낼 때 UDPMSG 구조체로 보내고
	//구조체로 받으니 Li_numRead, msg 모두 정상 동작함.
	//LMMAgent에서는 구조체가 아닌 udpSocket.Send(LM_AGENT_LOGIN_OK) 형식으로 보내는 코드들이
	//다수 존재하는데 이 때에도 구조체 형식으로 Receive해도 command에는 올바른 값이 수신된다.
	/*
	BYTE* Lb_Buffer = new BYTE[MAX_BUF];
	int Li_BufferSize = sizeof(BYTE) * MAX_BUF;
	::ZeroMemory(Lb_Buffer, Li_BufferSize);

	CString Ls_RemoteAddr;
	UINT Li_RemotePort;
	int Li_numRead = ReceiveFrom(Lb_Buffer, Li_BufferSize,
	Ls_RemoteAddr, Li_RemotePort);

	int cmd_id = *Lb_Buffer;
	*/

	CString str;

	UDPMSG msg;
	ZeroMemory(&msg, sizeof(msg));

	CString Ls_RemoteAddr;
	UINT Li_RemotePort;

	int Li_numRead = ReceiveFrom(&msg, sizeof(msg), Ls_RemoteAddr, Li_RemotePort);
	logWrite(_T("command = %d, param1 = %s, param2 = %s"), msg.command, msg.param1, msg.param2);
	logWrite(_T("Li_numRead = %d"), Li_numRead);


	switch (Li_numRead)
	{
		case NULL:
			Close();
			break;

		case SOCKET_ERROR:
			{
				int Li_ErrorCode = GetLastError();
				if (Li_ErrorCode != WSAEWOULDBLOCK)
				{
					if (Li_ErrorCode != WSAEMSGSIZE)
					{
						// OnReceiveError;
					}
					else
					{
						// Datagram was too large and was truncated
						// Buffer 에 추가로 담으면 됨
					}
				}
			}
			break;

		default:
			{
				//CString sStr((char*)Lb_Buffer);
			
				switch(msg.command)
				{
					case LM_AGENT_EXECUTE_OK:
						{
					
						}
						break;
					case LM_AGENT_LOGIN_OK:
						{
							Config::SaveManualLoginStatus(0);

							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->SetLoginState(LOGIN_OK);
							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->SelectChildDialog();
							//20240723 scpark 로그인이 완료되면 앱을 종료시키지 말고 완료된 모습을 그대로 표시하자.
							//20240729 scpark 로그인 완료 후 창을 남겨두지 않고 종료시키도록 변경
							//20240731 scpark 로그인 완료 후 메인창을 그냥 닫지말고 업데이트를 수행한 후 종료시키자.
							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->check_update_and_exit();
						}
						break;
					case LM_AGENT_SERVER_CON_FAIL:
						{
							if( ((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->GetLoginState() == LOGIN_BEFORE )
							{
								CMessageDlg dlgMessage;
								dlgMessage.SetMessage(_S(IDS_CONNECT_FAIL));
								dlgMessage.DoModal();
						
								Config::SaveManualLoginStatus(0);

								theApp.m_agent_control.StopAgent();
								((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->StopLoading();
							}
						}
						break;
					case LM_AGENT_ID_PASS_FAIL:
						{
							if( ((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->GetLoginState() == LOGIN_BEFORE )
							{
								CMessageDlg dlgMessage;
								dlgMessage.SetMessage(_S(IDS_INVALID_IDPW));
								dlgMessage.DoModal();
						
								Config::SaveManualLoginStatus(0);

								theApp.m_agent_control.StopAgent();
								((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->StopLoading();
							}
						}
						break;
					case LM_AGENT_LOGOUT:
						{
							Config::SaveManualLoginStatus(0);

							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->SetLoginState(LOGIN_BEFORE);
							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->SetUserLogout(TRUE);
							((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->SelectChildDialog();
						}
						break;
					case LM_AGENT_VOLUME_FULL:
						{
							if( ((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->GetLoginState() == LOGIN_BEFORE )
							{
								ShellExecute(NULL, _T("open"), _T("https://linkmemine.freshdesk.com/support/solutions/articles/151000206000--%EC%97%90%EC%9D%B4%EC%A0%84%ED%8A%B8%EC%9D%98-%EA%B3%84%EC%A0%95-%EB%93%B1%EB%A1%9D-%ED%95%9C%EB%8F%84%EA%B0%80-%EC%B4%88%EA%B3%BC%EB%90%98%EC%97%88%EC%8A%B5%EB%8B%88%EB%8B%A4-%EB%98%90%EB%8A%94-%EB%9D%BC%EC%9D%B4%EC%84%A0%EC%8A%A4-%EB%A7%8C%EB%A3%8C-%EC%95%8C%EB%A6%BC-%ED%8C%9D%EC%97%85-%EC%B2%98%EB%A6%AC-%EB%B0%A9%EB%B2%95"), 0, 0, SW_SHOWNORMAL);

								CMessageDlg dlgMessage;
								dlgMessage.SetMessage(_S(IDS_SERVER_VOLUME_FULL));
								dlgMessage.DoModal();
						
								Config::SaveManualLoginStatus(0);

								theApp.m_agent_control.StopAgent();
								((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->StopLoading();
							}
						}
						break;

					case LM_AGENT_LICENSE_EXPIRED:
						{
							if( ((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->GetLoginState() == LOGIN_BEFORE )
							{
								CMessageDlg dlgMessage;
								dlgMessage.SetMessage(_S(IDS_SERVER_LICENSE_EXPIRED));
								dlgMessage.DoModal();
						
								Config::SaveManualLoginStatus(0);

								theApp.m_agent_control.StopAgent();
								((CLMMLoginManagerDlg*)AfxGetApp()->m_pMainWnd)->StopLoading();
							}
						}
						break;
				}
			}
	}

	CAsyncSocket::OnReceive(nErrorCode);
}