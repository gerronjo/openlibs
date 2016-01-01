//
//USkin API Header File
//CopyRight by NEEMedia
//All Rights Reserved.
//
//
#ifndef _USKIN_H_
#define _USKIN_H_

#ifdef USKIN_EXPORTS
	#define USKIN_API __declspec(dllexport)
#else
	#define USKIN_API __declspec(dllimport)
#endif


USKIN_API BOOL  __stdcall USkinInit(LPCTSTR lpszUserName,LPCTSTR lpszRegCode,LPCTSTR lpszFileName);

//Exit USkin
USKIN_API BOOL  __stdcall USkinExit();

//Like Init USkin ,but to change skin
USKIN_API BOOL  __stdcall USkinLoadSkin(LPCTSTR lpszFileName);

USKIN_API void	__stdcall USkinAboutSkin();
//Get Window Menu
USKIN_API HMENU __stdcall USkinGetMenu(HWND hWnd);
//Apply Skin Effect In Hue Saturation Way
//fHue 0.0f-360.f
//fSaturation 0.0f - 1.0f
USKIN_API BOOL __stdcall USkinApplyColorTheme(float fHue,float fSaturation);
//Restore to windows default theme
USKIN_API BOOL  __stdcall USkinRemoveSkin();
//If use RemoveUSkin To Restore to windows default theme
//Use this function will restore to uskin theme
USKIN_API BOOL  __stdcall USkinRestoreSkin();

//Notes: following function only valide in professional version,not valide in free version
USKIN_API BOOL __stdcall USkinApplyColorThemeByRGB(COLORREF clrTheme);

USKIN_API BOOL __stdcall USkinLoadSkinFromBuffer(byte* lpBuffer,UINT nBufferSize);
USKIN_API BOOL __stdcall USkinLoadSkinFromResource(HMODULE hInstance,LPCTSTR lpszResourceName,LPCTSTR lpszResourceType);
USKIN_API BOOL __stdcall USkinSetMenuItemImage(HWND hWnd,UINT nMenuID,HIMAGELIST hImageList,UINT nImageIndex);
USKIN_API BOOL __stdcall USkinSetMenuItemImageEx(HWND hWnd,UINT nMenuID,HIMAGELIST hImageList,UINT nImageIndex,
						HIMAGELIST hImageListHot,UINT nImageIndexHot,
						HIMAGELIST hImageListDisabled,UINT nImageIndexDisabled);
typedef HANDLE HUSKIN;
USKIN_API HUSKIN __stdcall USkinOpenSkinData(LPCTSTR lpszSkinObjectName);
USKIN_API BOOL __stdcall USkinCloseSkinData(HUSKIN hUSkin);
USKIN_API BOOL __stdcall USkinGetBool(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,BOOL* lpValue);
USKIN_API BOOL __stdcall USkinGetInt(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,int* lpValue);
USKIN_API BOOL __stdcall USkinGetDWORD(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,DWORD* lpValue);
USKIN_API BOOL __stdcall USkinGetDouble(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,double* lpValue);
USKIN_API BOOL __stdcall USkinGetString(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,LPTSTR lpString,UINT nMaxSize);
USKIN_API BOOL __stdcall USkinGetEnumString(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,LPTSTR  lpString,UINT nMaxSize);
USKIN_API BOOL __stdcall USkinGetEnumValue(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,DWORD* lpValue);
USKIN_API BOOL __stdcall USkinGetFont(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,LOGFONT* lpLogFont);
USKIN_API BOOL __stdcall USkinGetColor(HUSKIN hUSkin,LPCTSTR lpszSkinPropName,COLORREF* lpColor);
USKIN_API BOOL __stdcall USkinDrawSkinImageSection(HUSKIN hUSkin,LPCTSTR lpszSkinPaintOptionResName,LPCTSTR lpszSkinImageSectionResName,HDC hDC,LPRECT lpDrawRect);
USKIN_API BOOL __stdcall USkinDrawText(HUSKIN hUSkin,LPCTSTR lpszSkinTextStyleResName,HDC hDC,LPRECT lpTextRect,LPCTSTR lpszText);
USKIN_API BOOL __stdcall USkinSetWindowSkin(HWND hWnd,LPCTSTR lpszSkinObjectName);
USKIN_API BOOL __stdcall USkinEnableWindowSkin(HWND hWnd,BOOL bEnabled);
USKIN_API BOOL __stdcall USkinApplyThread(DWORD dwThreadID);
USKIN_API COLORREF __stdcall USkinGetSysColor(int nIndex);
USKIN_API HBRUSH __stdcall USkinGetSysColorBrush(int nIndex);
USKIN_API COLORREF __stdcall USkinGetWinColor(int nIndex);
USKIN_API HBRUSH __stdcall USkinGetWinColorBrush(int nIndex);

#ifndef WM_USKINCOMMAND
	#define WM_USKINCOMMAND	WM_USER+1001
#endif

#ifndef WM_USKINCHANGED
	#define WM_USKINCHANGED WM_USER+1005
#endif

#endif