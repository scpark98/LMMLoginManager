#pragma once
#include "afxsock.h"

//scpark 20230925
//기존에는 nErrorCode만 받아서 간단한 액션만 처리했으나
//삼성생명에서는 Agent등록여부 등에 따른 처리가 필요하여
//기능 수정함.
typedef struct _udpmsg {
	int command;
	char company[32];
	char personnel[32];
	char phone[16];
	char time[32];
	char email[64];
	long temp;
	int temp2;
	TCHAR param1[MAX_PATH];
	TCHAR param2[MAX_PATH];
} UDPMSG;


class CUdpSocket :
	public CAsyncSocket
{
public:
	CUdpSocket(void);
	~CUdpSocket(void);

	BOOL Create();

	void OnReceive(int nErrorCode);
};

