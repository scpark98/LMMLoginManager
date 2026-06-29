
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
