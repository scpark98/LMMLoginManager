@../../Common/claude.md

# LMMLoginManager 프로젝트 컨텍스트

## 빌드

- 솔루션은 **`.slnx`** (VS2026 형식). `.sln` 은 없다 — 없다고 단정하기 전에 확장자 변형을 확인할 것.
- 축: `_LINKMEMINE_10` / `_LINKMEMINE_30` 매크로로 1.0 / 3.0 SE 분기.
- 주 빌드 구성: **Release_Lmm30 | x86(Win32)**.
- 산출물: `C:\Program Files (x86)\LinkMeMineSE\Agent\LMMLgiMgr.exe` (빌드 후 자동 복사).

## 인코딩

- `.cpp` / `.h` / `resource.h` — **UTF-8 BOM**. Edit/Write 도구로 직접 편집 가능.
- `LMMLoginManager.rc` — **UTF-16LE BOM**. Edit/Write 금지. PowerShell `Get-Content -Encoding Unicode` → 수정 → `Set-Content -Encoding Unicode` 라운드트립으로만 편집한다.

## 다국어 리소스

- 스트링 테이블은 `LMMLoginManager.rc` 에 **일본어 / 한국어 / 영어** 3개 `LANGUAGE` 블록으로 존재한다. 신규 문자열은 **3개 블록 모두**에 추가해야 한다.
- 코드에서는 `_S(IDS_XXX)` (= `Common/Functions.h` 의 `load_string`) 로 읽는다. 한글 문자열 리터럴 하드코딩 금지.
- 2026-08-18 기준 프로젝트 `*.cpp` 내 하드코딩 한글 문자열은 0건 (주석과 `LMMLoginManager.h:8` 의 `#error` 제외).
- 용어 통일: 사용자에게 보이는 문구는 **"에이전트 이름"** 을 쓴다 ("장치명" 아님).
- `resource.h` 의 `_APS_NEXT_RESOURCE_VALUE` 가 실제 사용 번호보다 낮으면 VS 리소스 편집기가 중복 ID 를 배정한다. 신규 ID 추가 시 함께 올릴 것.

---

# 보류 중인 작업

## [보류] UI 언어 결정 기준 통일 (2026-08-18 논의, 다음으로 미룸)

**미루는 이유**: 변경 범위가 크고(앱 + Common), OS 별(XP / Vista+ / Win10+) · 언어별 실기 테스트가 많이 필요하다.

### 문제

앱 안에서 UI 언어 판정 기준이 **두 가지로 갈려 있다.**

| 경로 | 실제 기준 | 판정 |
|---|---|---|
| `CSCMessageBox` 버튼 캡션 (`Common/messagebox/CSCMessageBox/SCMessageBox.cpp:91`) | `GetUserDefaultUILanguage()` = 표시 언어 | 올바름 |
| `_S()` → `CString::LoadString` → 리소스 로더 | 스레드 로캘(= 지역·형식 설정) | 틀림 |

리소스 로더의 언어 선택 규칙은 **XP(스레드 로캘)와 Vista 이후(MUI 폴백 체인)가 서로 다르다.** 같은 설정의 머신에서도 OS 버전에 따라 결과가 갈린다.

**재현 사례**: 한국어 XP 에서 실행 시 푸시버튼은 한국어, 체크박스는 영어로 표시됨. (당시 버튼은 하드코딩이라 한국어로 보였고, `_S()` 를 타는 체크박스만 영어로 떨어졌다. 하드코딩은 이후 제거했으므로 지금은 두 컨트롤 모두 영어로 나올 것이다.)

### 결정된 원칙

**UI 언어(표시 언어, MUI)를 따른다. 지역/로캘은 날짜·숫자·통화 형식에만 쓴다.**

지역은 "이 사람이 어느 언어를 읽는가"의 대리 지표가 될 수 없다. 결정적 반례 — 한국 주재 미국인은 표시 언어가 영어인데 지역만 한국이며, 지역 기준으로 한국어를 보여주면 글자를 못 읽는다. 표시 언어는 사용자가 그 질문에 **직접 답해 둔 값**이다.

### 적용할 정책 (우선순위)

1. 앱 자체 설정 (`ini` 의 `[UI] LANGUAGE=ko|ja|en|auto`) — 명시 선택 시 무조건 우선
2. `auto` 이면 `GetUserPreferredUILanguages()` 목록을 순회해 지원 언어(ko/ja/en) 중 첫 매치 (XP 폴백: `GetUserDefaultUILanguage()`)
3. 매치 없으면 영어
4. 지역(LCID)은 언어 결정에 사용하지 않는다

### 구현 시 주의점

1. **리소스 로더에 강제해야 한다.** 언어를 결정만 하고 두면 `LoadString` 은 여전히 OS 판단을 따른다. Vista+ 는 `SetThreadUILanguage()` / `SetThreadPreferredUILanguages()`, XP 는 `SetThreadLocale()`. XP 를 지원하므로 Vista+ API 는 `GetProcAddress` 로 동적 로드해야 한다(정적 링크 시 XP 에서 프로세스 로드 실패). 호출은 `InitInstance` 초반, 다이얼로그 생성 전 1회.
2. **`CSCMessageBox` 는 이 강제를 따라오지 않는다.** `GetUserDefaultUILanguage()` 는 *사용자 수준* 값이라 `SetThreadUILanguage()` 로 바뀌지 않는다. Common 쪽에 전역 `set_ui_language()` 진입점을 만들어 `SCMessageBox.cpp:91` 이 그 값을 읽게 바꿔야 한다.
3. **`CSCMessageBox` 는 한/영 2개국어뿐이다.** `lang == 1042 ? 한국어 : 영어` 구조라 일본어 Windows 에서 버튼이 영어로 나온다. `.rc` 는 일본어까지 갖췄으므로 여기서 어긋난다.
4. **`ServiceSetting::isRegionJA` 는 dead flag.** 설정 파일의 `REGION_JA` 로 `TRUE` 가 되지만 읽는 곳이 없다 (`ServiceSetting.cpp:20,122` / `ServiceSetting.h:28` 이 전부). 일본어 배포판을 강제하려던 흔적으로 보이므로, 위 정책 도입 시 "배포판 고정 언어"(우선순위 0번)로 승격하거나 제거한다.

### 남는 한계

OS 가 그리는 것 — 공용 파일 대화상자, 시스템 `MessageBox`, MFC 기본 리소스(`afxres`) — 은 앱이 뭘 하든 OS 표시 언어를 따른다. 완전 일치는 불가능하며 자체 컨트롤로 그리는 부분만 통제할 수 있다.
