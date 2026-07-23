
// LMMLoginManagerDlg.h: 헤더 파일
//

#pragma once

#include "Common/colors.h"
#include "Common/CEdit/CSCStaticEdit/SCStaticEdit.h"
#include "Common/CButton/GdiButton/GdiButton.h"
#include "Common/thread/CSCThread/SCThread.h"

#include "UdpSocket.h"

// CLMMLoginManagerDlg 대화 상자
class CLMMLoginManagerDlg : public CDialogEx
{
// 생성입니다.
public:
	CLMMLoginManagerDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.

	bool				service_start();
	bool				service_stop(bool include_delete = false);

	int					get_login_state() { return m_login_state; }
	void				set_login_state(int login_state) { m_login_state = login_state; }
	void				select_child_dialog();

	void				request_put_device_env_info();

	//20240912 scpark 3.0 SE 전용 — 로그인 성공 후 에이전트 등록신청 상태를 조회하고
	//상태에 맞는 안내창을 띠운다. 개인향(account_type==0) 또는 결재프로세스 off 사용자는
	//즉시 창을 닫고 종료. 기업향 & 결재프로세스 on 사용자만 등록/승인/거절 안내가 표시된다.
	//호출 지점(UdpSocket LM_AGENT_LOGIN_OK 등)에서 #ifdef _LINKMEMINE_30 로 가드하므로
	//함수 자체는 무가드 상시 컴파일 (내부에서 3.0 SE 백엔드 API 만 호출).
	void				check_regi_status();
	//return 0: 신청중, 1/3/4: 미등록/거절/취소, 2: 승인됨, -1: 요청 실패
	DWORD				request_regi_status();

	//20260723 by claude. LM_AGENT_LOGIN_FAILED / LM_AGENT_ID_PASS_FAIL 수신 후 재입력 편의를 위해
	//ID edit 에 포커스 + 텍스트 전체선택. 사용자가 바로 다시 타이핑하면 이전 값을 덮어쓴다.
	//3.0 SE old CLoginDlg::set_id_input_focus 이식.
	void				set_id_input_focus();

	//config.ini를 다시 읽어 설정 관련 컨트롤 상태를 갱신한다.
	//LMMAgent가 config.ini를 직접 고쳐도(로그인 실패 시 AUTO_LOGIN 해제 등) UI는 시작 시 읽은 상태로
	//남아 실제 설정과 어긋나므로, LM_AGENT_CONFIG_CHANGED 수신 시 호출한다.
	void				reload_config_to_controls();

	//for test
	void				change_theme(bool next_theme = true);

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LMMLOGINMANAGER_DIALOG };
#endif

protected:
	enum TIMER_ID
	{
		timer_check_version = 1,
	};

	CSCColorTheme		m_theme = CSCColorTheme(this);

	CSCGdiplusBitmap	m_logo;

	CSCThread			m_thread;
	void				thread_get_version_and_login(CSCThread& th);

	CUdpSocket			m_udpSocket;		// UDP Server

	static constexpr UINT WM_APP_UI_INVOKE = WM_APP + 2;
	afx_msg LRESULT		on_ui_invoke(WPARAM wParam, LPARAM lParam);

private:
	void				invoke_ui(std::function<void()> func);

protected:
	void				init_dialog_style();
	void				init_controls();

	CString				m_current_version = _T("0.0.0.0");
	CString				m_latest_version = _T("0.0.0.0");

	int					m_login_state = LOGIN_BEFORE;

	bool				get_server();
	bool				get_current_version();
	bool				get_latest_version();
	bool				validate_login_input();

	//online(>0), offline(0), request error(<0)
	int					get_device_onoff_status();

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	CGdiButton m_button_config;
	CGdiButton m_button_minimize;
	CGdiButton m_button_close;
	CSCStaticEdit m_edit_id;
	CSCStaticEdit m_edit_pw;
	CGdiButton m_check_save_pw;
	CGdiButton m_check_auto_login;
	CGdiButton m_button_login;
	CGdiButton m_button_restart;
	afx_msg void OnBnClickedButtonConfig();
	afx_msg void OnBnClickedButtonMinimize();
	afx_msg void OnBnClickedButtonClose();
	afx_msg void OnBnClickedCheckSavePw();
	afx_msg void OnBnClickedCheckAutoLogin();
	afx_msg void OnBnClickedButtonLogin();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CSCStatic m_static_version;
	afx_msg void OnBnClickedButtonRestart();
	afx_msg void OnDestroy();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
