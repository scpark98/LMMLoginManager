#pragma once

class ServiceSetting
{
public:
	ServiceSetting(void);
	~ServiceSetting(void);

	static BOOL LoadServiceSetting();
	static CString GetProgramType();  // 서비스, 솔루션 구분
	static CString GetSetting();

	static CString strManagerToken;

	static BOOL isService;
	static BOOL isSolution;
	static BOOL isServerInput;
	static BOOL isServiceP2P;
	static BOOL isVisibleSignUp;
	static BOOL isVisibleLicense;
	static BOOL isKMSCrypt;
	static BOOL isCheckLicense;
	static BOOL isRemoteThumbnail;
	static BOOL isSyncGroup;
	static BOOL isThumbnailViewer;
	static BOOL isAlertCheck;
	static BOOL isAutoRecord;
	static BOOL isRegionJA;
};

