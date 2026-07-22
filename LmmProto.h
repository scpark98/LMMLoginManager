#pragma once

// LM_AGENT_* UDP 커맨드는 Common 공유 헤더로 이관됨 (Agent 두 프로젝트와
// SSOT 유지 목적). 이 파일에는 LoginManager 로컬 상수만 남긴다.
#include "Common/Koino/LMM/LmmProto.h"

// UI 컬러 (LoginManager 로컬)
const COLORREF COLOR_MAIN      = RGB(241, 244, 250);
const COLORREF COLOR_MAIN_TOP  = RGB(91, 142, 251);
const COLORREF COLOR_EDIT_BACK = RGB(238, 238, 238);
const COLORREF COLOR_EDIT_TEXT = RGB(14, 154, 227);
const COLORREF COLOR_EDIT_LINE = RGB(198, 201, 207);

// LoginManager 내부 상태
const int LOGIN_BEFORE = 0;
const int LOGIN_OK     = 1;