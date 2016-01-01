#ifndef _COMMON_NET_
#define _COMMON_NET_

#pragma once
#include "common.h"
#include <LM.h>

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;
using namespace System::Runtime::InteropServices;
using namespace System::Security::Cryptography;
using namespace System::IO;
using namespace System::Diagnostics;
using namespace System::Text;
using namespace Microsoft::Win32;
using namespace System::ServiceProcess;
using namespace System::Management;
using namespace System::Text::RegularExpressions ;
using namespace System::Net;
using namespace System::DirectoryServices;

namespace Deianeira {

	/// <summary>
	/// config 摘要
	/// </summary>
	public class common_net 
	{
	public:
		common_net(void)
		{
			//
			//TODO: 在此处添加构造函数代码
			//
		}

		//获得系统的版本
		static ULONG common_getOSVersion()
		{
			if (Environment::OSVersion->Platform == PlatformID::Win32NT)
			{
				if (Environment::OSVersion->Version->Major == 5)
				{
					if (Environment::OSVersion->Version->Minor == 0)
					{
						return OS_WINDOWS_2000;
					}
					else if(Environment::OSVersion->Version->Minor == 1)
					{
						return OS_WINDOWS_XP;
					}
					else if(Environment::OSVersion->Version->Minor == 2)
					{
						return OS_WINDOWS_2003;
					}
					else
					{
						return OS_UNKNOWN;
					}
				}
				else if (Environment::OSVersion->Version->Major == 6)
				{
					if (Environment::OSVersion->Version->Minor == 0)
					{
						return OS_WINDOWS_VISTA_2008;
					}
					else if(Environment::OSVersion->Version->Minor == 1)
					{
						return OS_WINDOWS_7_2008R2;
					}
					else
					{
						return OS_UNKNOWN;
					}
				}
				else
				{
					return OS_UNKNOWN;
				}
			}
			else
			{
				return OS_UNKNOWN;
			}
		}

		//shadow xp函数列表
		static array<String^>^ common_getXPShadowAPI()
		{
			return gcnew array<String^>{
				"NtGdiAbortDoc",
					"NtGdiAbortPath",
					"NtGdiAddFontResourceW", 
					"NtGdiAddRemoteFontToDC", 
					"NtGdiAddFontMemResourceEx", 
					"NtGdiRemoveMergeFont",
					"NtGdiAddRemoteMMInstanceToDC",  
					"NtGdiAlphaBlend",  
					"NtGdiAngleArc",  
					"NtGdiAnyLinkedFonts",  
					"NtGdiFontIsLinked", 
					"NtGdiArcInternal",  
					"NtGdiBeginPath",
					"NtGdiBitBlt",
					"NtGdiCancelDC",
					"NtGdiCheckBitmapBits", 
					"NtGdiCloseFigure", 
					"NtGdiClearBitmapAttributes", 
					"NtGdiClearBrushAttributes", 
					"NtGdiColorCorrectPalette", 
					"NtGdiCombineRgn",  
					"NtGdiCombineTransform",
					"NtGdiComputeXformCoefficients", 
					"NtGdiConsoleTextOut",
					"NtGdiConvertMetafileRect", 
					"NtGdiCreateBitmap", 
					"NtGdiCreateClientObj",
					"NtGdiCreateColorSpace",  
					"NtGdiCreateColorTransform",  
					"NtGdiCreateCompatibleBitmap",   
					"NtGdiCreateCompatibleDC",   
					"NtGdiCreateDIBBrush",   
					"NtGdiCreateDIBitmapInternal",   
					"NtGdiCreateDIBSection",   
					"NtGdiCreateEllipticRgn",   
					"NtGdiCreateHalftonePalette",   
					"NtGdiCreateHatchBrushInternal",   
					"NtGdiCreateMetafileDC",   
					"NtGdiCreatePaletteInternal",   
					"NtGdiCreatePatternBrushInternal",   
					"NtGdiCreatePen",  
					"NtGdiCreateRectRgn",  
					"NtGdiCreateRoundRectRgn",   
					"NtGdiCreateServerMetaFile",   
					"NtGdiCreateSolidBrush",   
					"NtGdiD3dContextCreate",   
					"NtGdiD3dContextDestroy",   
					"NtGdiD3dContextDestroyAll",   
					"NtGdiD3dValidateTextureStageState",  
					"NtGdiD3dDrawPrimitives2",  
					"NtGdiDdGetDriverState",   
					"NtGdiDdAddAttachedSurface",   
					"NtGdiDdAlphaBlt",  
					"NtGdiDdAttachSurface",  
					"NtGdiDdBeginMoCompFrame",  
					"NtGdiDdBlt",   
					"NtGdiDdCanCreateSurface",  
					"NtGdiDdCanCreateD3DBuffer",  
					"NtGdiDdColorControl",  
					"NtGdiDdCreateDirectDrawObject",   
					"NtGdiDdCreateSurface",   
					"NtGdiDdCreateD3DBuffer",  
					"NtGdiDdCreateMoComp",  
					"NtGdiDdCreateSurfaceObject",  
					"NtGdiDdDeleteDirectDrawObject",  
					"NtGdiDdDeleteSurfaceObject",   
					"NtGdiDdDestroyMoComp",   
					"NtGdiDdDestroySurface",  
					"NtGdiDdDestroyD3DBuffer",   
					"NtGdiDdEndMoCompFrame",  
					"NtGdiDdFlip",   
					"NtGdiDdFlipToGDISurface",  
					"NtGdiDdGetAvailDriverMemory",   
					"NtGdiDdGetBltStatus",   
					"NtGdiDdGetDC",   
					"NtGdiDdGetDriverInfo",   
					"NtGdiDdGetDxHandle",  
					"NtGdiDdGetFlipStatus",  
					"NtGdiDdGetInternalMoCompInfo",  
					"NtGdiDdGetMoCompBuffInfo",   
					"NtGdiDdGetMoCompGuids",  
					"NtGdiDdGetMoCompFormats",   
					"NtGdiDdGetScanLine",   
					"NtGdiDdLock",   
					"NtGdiDdLockD3D",   
					"NtGdiDdQueryDirectDrawObject",   
					"NtGdiDdQueryMoCompStatus",   
					"NtGdiDdReenableDirectDrawObject",   
					"NtGdiDdReleaseDC",   
					"NtGdiDdRenderMoComp",   
					"NtGdiDdResetVisrgn",   
					"NtGdiDdSetColorKey",   
					"NtGdiDdSetExclusiveMode",   
					"NtGdiDdSetGammaRamp",  
					"NtGdiDdCreateSurfaceEx",  
					"NtGdiDdSetOverlayPosition",   
					"NtGdiDdUnattachSurface",   
					"NtGdiDdUnlock",   
					"NtGdiDdUnlockD3D",   
					"NtGdiDdUpdateOverlay",   
					"NtGdiDdWaitForVerticalBlank",   
					"NtGdiDvpCanCreateVideoPort",   
					"NtGdiDvpColorControl", 
					"NtGdiDvpCreateVideoPort",  
					"NtGdiDvpDestroyVideoPort",  
					"NtGdiDvpFlipVideoPort",  
					"NtGdiDvpGetVideoPortBandwidth",  
					"NtGdiDvpGetVideoPortField",  
					"NtGdiDvpGetVideoPortFlipStatus",  
					"NtGdiDvpGetVideoPortInputFormats",  
					"NtGdiDvpGetVideoPortLine",  
					"NtGdiDvpGetVideoPortOutputFormats", 
					"NtGdiDvpGetVideoPortConnectInfo",
					"NtGdiDvpGetVideoSignalStatus", 
					"NtGdiDvpUpdateVideoPort",
					"NtGdiDvpWaitForVideoPortSync",
					"NtGdiDvpAcquireNotification",
					"NtGdiDvpReleaseNotification",
					"NtGdiDxgGenericThunk",
					"NtGdiDeleteClientObj",
					"NtGdiDeleteColorSpace",
					"NtGdiDeleteColorTransform",
					"NtGdiDeleteObjectApp",
					"NtGdiDescribePixelFormat",
					"NtGdiGetPerBandInfo",
					"NtGdiDoBanding",
					"NtGdiDoPalette",
					"NtGdiDrawEscape",
					"NtGdiEllipse", 
					"NtGdiEnableEudc",
					"NtGdiEndDoc",
					"NtGdiEndPage", 
					"NtGdiEndPath",
					"NtGdiEnumFontChunk",
					"NtGdiEnumFontClose", 
					"NtGdiEnumFontOpen",  
					"NtGdiEnumObjects",  
					"NtGdiEqualRgn",   
					"NtGdiEudcLoadUnloadLink",   
					"NtGdiExcludeClipRect",   
					"NtGdiExtCreatePen",  
					"NtGdiExtCreateRegion",   
					"NtGdiExtEscape",   
					"NtGdiExtFloodFill",   
					"NtGdiExtGetObjectW",  
					"NtGdiExtSelectClipRgn",  
					"NtGdiExtTextOutW",  
					"NtGdiFillPath",  
					"NtGdiFillRgn",   
					"NtGdiFlattenPath",   
					"NtGdiFlushUserBatch",  
					"NtGdiFlush",  
					"NtGdiForceUFIMapping",  
					"NtGdiFrameRgn",  
					"NtGdiFullscreenControl",  
					"NtGdiGetAndSetDCDword",  
					"NtGdiGetAppClipBox",  
					"NtGdiGetBitmapBits",  
					"NtGdiGetBitmapDimension",   
					"NtGdiGetBoundsRect",  
					"NtGdiGetCharABCWidthsW",  
					"NtGdiGetCharacterPlacementW",   
					"NtGdiGetCharSet",  
					"NtGdiGetCharWidthW",   
					"NtGdiGetCharWidthInfo",   
					"NtGdiGetColorAdjustment",   
					"NtGdiGetColorSpaceforBitmap",   
					"NtGdiGetDCDword",   
					"NtGdiGetDCforBitmap",   
					"NtGdiGetDCObject",   
					"NtGdiGetDCPoint",  
					"NtGdiGetDeviceCaps",   
					"NtGdiGetDeviceGammaRamp",  
					"NtGdiGetDeviceCapsAll",  
					"NtGdiGetDIBitsInternal",  
					"NtGdiGetETM",  
					"NtGdiGetEudcTimeStampEx",   
					"NtGdiGetFontData",   
					"NtGdiGetFontResourceInfoInternalW",   
					"NtGdiGetGlyphIndicesW",   
					"NtGdiGetGlyphIndicesWInternal",   
					"NtGdiGetGlyphOutline",   
					"NtGdiGetKerningPairs",   
					"NtGdiGetLinkedUFIs",   
					"NtGdiGetMiterLimit",   
					"NtGdiGetMonitorID",  
					"NtGdiGetNearestColor",   
					"NtGdiGetNearestPaletteIndex",   
					"NtGdiGetObjectBitmapHandle",   
					"NtGdiGetOutlineTextMetricsInternalW",  
					"NtGdiGetPath",  
					"NtGdiGetPixel",  
					"NtGdiGetRandomRgn",  
					"NtGdiGetRasterizerCaps",   
					"NtGdiGetRealizationInfo",   
					"NtGdiGetRegionData",   
					"NtGdiGetRgnBox",   
					"NtGdiGetServerMetaFileBits",  
					"NtGdiGetSpoolMessage",  
					"NtGdiGetStats",  
					"NtGdiGetStockObject",  
					"NtGdiGetStringBitmapW",  
					"NtGdiGetSystemPaletteUse",  
					"NtGdiGetTextCharsetInfo",  
					"NtGdiGetTextExtent", 
					"NtGdiGetTextExtentExW",  
					"NtGdiGetTextFaceW",  
					"NtGdiGetTextMetricsW",  
					"NtGdiGetTransform",   
					"NtGdiGetUFI",  
					"NtGdiGetEmbUFI",  
					"NtGdiGetUFIPathname",  
					"NtGdiGetEmbedFonts",  
					"NtGdiChangeGhostFont", 
					"NtGdiAddEmbFontToDC",  
					"NtGdiGetFontUnicodeRanges",  
					"NtGdiGetWidthTable",  
					"NtGdiGradientFill",
					"NtGdiHfontCreate", 
					"NtGdiIcmBrushInfo", 
					"NtGdiInit",  
					"NtGdiInitSpool",   
					"NtGdiIntersectClipRect",   
					"NtGdiInvertRgn",   
					"NtGdiLineTo",   
					"NtGdiMakeFontDir",   
					"NtGdiMakeInfoDC",   
					"NtGdiMaskBlt",   
					"NtGdiModifyWorldTransform",  
					"NtGdiMonoBitmap",   
					"NtGdiMoveTo",  
					"NtGdiOffsetClipRgn",   
					"NtGdiOffsetRgn",   
					"NtGdiOpenDCW",  
					"NtGdiPatBlt",  
					"NtGdiPolyPatBlt",  
					"NtGdiPathToRegion",  
					"NtGdiPlgBlt",  
					"NtGdiPolyDraw",  
					"NtGdiPolyPolyDraw",   
					"NtGdiPolyTextOutW",  
					"NtGdiPtInRegion",  
					"NtGdiPtVisible",  
					"NtGdiQueryFonts",   
					"NtGdiQueryFontAssocInfo",   
					"NtGdiRectangle",   
					"NtGdiRectInRegion",   
					"NtGdiRectVisible",   
					"NtGdiRemoveFontResourceW",  
					"NtGdiRemoveFontMemResourceEx",  
					"NtGdiResetDC",  
					"NtGdiResizePalette",  
					"NtGdiRestoreDC", 
					"NtGdiRoundRect",  
					"NtGdiSaveDC",  
					"NtGdiScaleViewportExtEx",  
					"NtGdiScaleWindowExtEx",  
					"NtGdiSelectBitmap",  
					"NtGdiSelectBrush",  
					"NtGdiSelectClipPath",  
					"NtGdiSelectFont",  
					"NtGdiSelectPen",  
					"NtGdiSetBitmapAttributes",  
					"NtGdiSetBitmapBits",  
					"NtGdiSetBitmapDimension",   
					"NtGdiSetBoundsRect",  
					"NtGdiSetBrushAttributes",  
					"NtGdiSetBrushOrg",   
					"NtGdiSetColorAdjustment", 
					"NtGdiSetColorSpace",  
					"NtGdiSetDeviceGammaRamp",  
					"NtGdiSetDIBitsToDeviceInternal", 
					"NtGdiSetFontEnumeration",   
					"NtGdiSetFontXform",   
					"NtGdiSetIcmMode",  
					"NtGdiSetLinkedUFIs",  
					"NtGdiSetMagicColors",  
					"NtGdiSetMetaRgn",  
					"NtGdiSetMiterLimit",  
					"NtGdiGetDeviceWidth", 
					"NtGdiMirrorWindowOrg", 
					"NtGdiSetLayout",  
					"NtGdiSetPixel",  
					"NtGdiSetPixelFormat",  
					"NtGdiSetRectRgn",  
					"NtGdiSetSystemPaletteUse",  
					"NtGdiSetTextJustification",  
					"NtGdiSetupPublicCFONT",   
					"NtGdiSetVirtualResolution",  
					"NtGdiSetSizeDevice",  
					"NtGdiStartDoc",  
					"NtGdiStartPage",  
					"NtGdiStretchBlt",  
					"NtGdiStretchDIBitsInternal", 
					"NtGdiStrokeAndFillPath", 
					"NtGdiStrokePath",  
					"NtGdiSwapBuffers",
					"NtGdiTransformPoints", 
					"NtGdiTransparentBlt", 
					"NtGdiUnloadPrinterDriver",  
					"NtGdiUnmapMemFont",  
					"NtGdiUnrealizeObject",  
					"NtGdiUpdateColors",  
					"NtGdiWidenPath",  
					"NtUserActivateKeyboardLayout", 
					"NtUserAlterWindowStyle",  
					"NtUserAssociateInputContext", 
					"NtUserAttachThreadInput", 
					"NtUserBeginPaint", 
					"NtUserBitBltSysBmp", 
					"NtUserBlockInput",  
					"NtUserBuildHimcList",   
					"NtUserBuildHwndList",  
					"NtUserBuildNameList",  
					"NtUserBuildPropList",  
					"NtUserCallHwnd",  
					"NtUserCallHwndLock", 
					"NtUserCallHwndOpt", 
					"NtUserCallHwndParam", 
					"NtUserCallHwndParamLock", 
					"NtUserCallMsgFilter",  
					"NtUserCallNextHookEx",  
					"NtUserCallNoParam",  
					"NtUserCallOneParam",  
					"NtUserCallTwoParam",  
					"NtUserChangeClipboardChain", 
					"NtUserChangeDisplaySettings",
					"NtUserCheckImeHotKey",  
					"NtUserCheckMenuItem",  
					"NtUserChildWindowFromPointEx",  
					"NtUserClipCursor",  
					"NtUserCloseClipboard",  
					"NtUserCloseDesktop",  
					"NtUserCloseWindowStation",  
					"NtUserConsoleControl",   
					"NtUserConvertMemHandle",   
					"NtUserCopyAcceleratorTable",   
					"NtUserCountClipboardFormats",   
					"NtUserCreateAcceleratorTable",   
					"NtUserCreateCaret",   
					"NtUserCreateDesktop",   
					"NtUserCreateInputContext",  
					"NtUserCreateLocalMemHandle",  
					"NtUserCreateWindowEx",  
					"NtUserCreateWindowStation",   
					"NtUserDdeGetQualityOfService",   
					"NtUserDdeInitialize",   
					"NtUserDdeSetQualityOfService",   
					"NtUserDeferWindowPos",   
					"NtUserDefSetText",   
					"NtUserDeleteMenu",   
					"NtUserDestroyAcceleratorTable",  
					"NtUserDestroyCursor",  
					"NtUserDestroyInputContext",  
					"NtUserDestroyMenu",  
					"NtUserDestroyWindow",  
					"NtUserDisableThreadIme",  
					"NtUserDispatchMessage",   
					"NtUserDragDetect",  
					"NtUserDragObject",  
					"NtUserDrawAnimatedRects",  
					"NtUserDrawCaption",   
					"NtUserDrawCaptionTemp",   
					"NtUserDrawIconEx",  
					"NtUserDrawMenuBarTemp",  
					"NtUserEmptyClipboard",   
					"NtUserEnableMenuItem",   
					"NtUserEnableScrollBar",   
					"NtUserEndDeferWindowPosEx",  
					"NtUserEndMenu",  
					"NtUserEndPaint",   
					"NtUserEnumDisplayDevices",   
					"NtUserEnumDisplayMonitors",   
					"NtUserEnumDisplaySettings",   
					"NtUserEvent",   
					"NtUserExcludeUpdateRgn",   
					"NtUserFillWindow",   
					"NtUserFindExistingCursorIcon",   
					"NtUserFindWindowEx",  
					"NtUserFlashWindowEx",   
					"NtUserGetAltTabInfo",  
					"NtUserGetAncestor",   
					"NtUserGetAppImeLevel",   
					"NtUserGetAsyncKeyState",   
					"NtUserGetAtomName",   
					"NtUserGetCaretBlinkTime",   
					"NtUserGetCaretPos",   
					"NtUserGetClassInfo",   
					"NtUserGetClassName",   
					"NtUserGetClipboardData",   
					"NtUserGetClipboardFormatName",   
					"NtUserGetClipboardOwner",   
					"NtUserGetClipboardSequenceNumber",   
					"NtUserGetClipboardViewer",   
					"NtUserGetClipCursor",   
					"NtUserGetComboBoxInfo",   
					"NtUserGetControlBrush",   
					"NtUserGetControlColor",   
					"NtUserGetCPD",   
					"NtUserGetCursorFrameInfo",   
					"NtUserGetCursorInfo",   
					"NtUserGetDC",   
					"NtUserGetDCEx",   
					"NtUserGetDoubleClickTime",   
					"NtUserGetForegroundWindow",   
					"NtUserGetGuiResources",   
					"NtUserGetGUIThreadInfo",   
					"NtUserGetIconInfo",   
					"NtUserGetIconSize",   
					"NtUserGetImeHotKey",   
					"NtUserGetImeInfoEx",  
					"NtUserGetInternalWindowPos",  
					"NtUserGetKeyboardLayoutList",   
					"NtUserGetKeyboardLayoutName",   
					"NtUserGetKeyboardState",   
					"NtUserGetKeyNameText",   
					"NtUserGetKeyState",   
					"NtUserGetListBoxInfo",   
					"NtUserGetMenuBarInfo",   
					"NtUserGetMenuIndex",   
					"NtUserGetMenuItemRect",  
					"NtUserGetMessage",  
					"NtUserGetMouseMovePointsEx",   
					"NtUserGetObjectInformation",   
					"NtUserGetOpenClipboardWindow",   
					"NtUserGetPriorityClipboardFormat",   
					"NtUserGetProcessWindowStation",   
					"NtUserGetRawInputBuffer",  
					"NtUserGetRawInputData",   
					"NtUserGetRawInputDeviceInfo",   
					"NtUserGetRawInputDeviceList",   
					"NtUserGetRegisteredRawInputDevices",   
					"NtUserGetScrollBarInfo",   
					"NtUserGetSystemMenu",   
					"NtUserGetThreadDesktop",   
					"NtUserGetThreadState",  
					"NtUserGetTitleBarInfo",   
					"NtUserGetUpdateRect",   
					"NtUserGetUpdateRgn",   
					"NtUserGetWindowDC",   
					"NtUserGetWindowPlacement",  
					"NtUserGetWOWClass",   
					"NtUserHardErrorControl",   
					"NtUserHideCaret",   
					"NtUserHiliteMenuItem",   
					"NtUserImpersonateDdeClientWindow",   
					"NtUserInitialize",   
					"NtUserInitializeClientPfnArrays",   
					"NtUserInitTask",  
					"NtUserInternalGetWindowText",  
					"NtUserInvalidateRect",  
					"NtUserInvalidateRgn",  
					"NtUserIsClipboardFormatAvailable",  
					"NtUserKillTimer",   
					"NtUserLoadKeyboardLayoutEx",   
					"NtUserLockWindowStation",   
					"NtUserLockWindowUpdate",   
					"NtUserLockWorkStation",   
					"NtUserMapVirtualKeyEx",   
					"NtUserMenuItemFromPoint",   
					"NtUserMessageCall",   
					"NtUserMinMaximize",   
					"NtUserMNDragLeave",   
					"NtUserMNDragOver",   
					"NtUserModifyUserStartupInfoFlags",  
					"NtUserMoveWindow",  
					"NtUserNotifyIMEStatus",  
					"NtUserNotifyProcessCreate",  
					"NtUserNotifyWinEvent",   
					"NtUserOpenClipboard",   
					"NtUserOpenDesktop",   
					"NtUserOpenInputDesktop",   
					"NtUserOpenWindowStation",   
					"NtUserPaintDesktop",   
					"NtUserPeekMessage",   
					"NtUserPostMessage",   
					"NtUserPostThreadMessage",   
					"NtUserPrintWindow",   
					"NtUserProcessConnect",   
					"NtUserQueryInformationThread",   
					"NtUserQueryInputContext",   
					"NtUserQuerySendMessage",   
					"NtUserQueryUserCounters",   
					"NtUserQueryWindow",   
					"NtUserRealChildWindowFromPoint",   
					"NtUserRealInternalGetMessage",   
					"NtUserRealWaitMessageEx",   
					"NtUserRedrawWindow",  
					"NtUserRegisterClassExWOW",   
					"NtUserRegisterUserApiHook",   
					"NtUserRegisterHotKey",   
					"NtUserRegisterRawInputDevices",   
					"NtUserRegisterTasklist",   
					"NtUserRegisterWindowMessage",   
					"NtUserRemoveMenu",   
					"NtUserRemoveProp",   
					"NtUserResolveDesktop",   
					"NtUserResolveDesktopForWOW",   
					"NtUserSBGetParms",  
					"NtUserScrollDC",  
					"NtUserScrollWindowEx",  
					"NtUserSelectPalette",  
					"NtUserSendInput",  
					"NtUserSetActiveWindow",   
					"NtUserSetAppImeLevel",   
					"NtUserSetCapture",   
					"NtUserSetClassLong",   
					"NtUserSetClassWord",   
					"NtUserSetClipboardData",  
					"NtUserSetClipboardViewer",  
					"NtUserSetConsoleReserveKeys",   
					"NtUserSetCursor",   
					"NtUserSetCursorContents",   
					"NtUserSetCursorIconData",   
					"NtUserSetDbgTag",   
					"NtUserSetFocus",   
					"NtUserSetImeHotKey",   
					"NtUserSetImeInfoEx",   
					"NtUserSetImeOwnerWindow",  
					"NtUserSetInformationProcess",  
					"NtUserSetInformationThread",  
					"NtUserSetInternalWindowPos",   
					"NtUserSetKeyboardState",   
					"NtUserSetLogonNotifyWindow",   
					"NtUserSetMenu",   
					"NtUserSetMenuContextHelpId",   
					"NtUserSetMenuDefaultItem",   
					"NtUserSetMenuFlagRtoL",   
					"NtUserSetObjectInformation",   
					"NtUserSetParent",   
					"NtUserSetProcessWindowStation",   
					"NtUserSetProp",   
					"NtUserSetRipFlags",   
					"NtUserSetScrollInfo",   
					"NtUserSetShellWindowEx",   
					"NtUserSetSysColors",   
					"NtUserSetSystemCursor",  
					"NtUserSetSystemMenu",  
					"NtUserSetSystemTimer",   
					"NtUserSetThreadDesktop",  
					"NtUserSetThreadLayoutHandles",   
					"NtUserSetThreadState",   
					"NtUserSetTimer",   
					"NtUserSetWindowFNID",   
					"NtUserSetWindowLong",  
					"NtUserSetWindowPlacement",  
					"NtUserSetWindowPos",  
					"NtUserSetWindowRgn",   
					"NtUserSetWindowsHookAW",   
					"NtUserSetWindowsHookEx",   
					"NtUserSetWindowStationUser",  
					"NtUserSetWindowWord",   
					"NtUserSetWinEventHook",   
					"NtUserShowCaret",   
					"NtUserShowScrollBar",   
					"NtUserShowWindow",   
					"NtUserShowWindowAsync",   
					"NtUserSoundSentry",   
					"NtUserSwitchDesktop",   
					"NtUserSystemParametersInfo",   
					"NtUserTestForInteractiveUser",  
					"NtUserThunkedMenuInfo",   
					"NtUserThunkedMenuItemInfo",   
					"NtUserToUnicodeEx",   
					"NtUserTrackMouseEvent",   
					"NtUserTrackPopupMenuEx",   
					"NtUserCalcMenuBar",   
					"NtUserPaintMenuBar",  
					"NtUserTranslateAccelerator",   
					"NtUserTranslateMessage",   
					"NtUserUnhookWindowsHookEx",   
					"NtUserUnhookWinEvent",   
					"NtUserUnloadKeyboardLayout",   
					"NtUserUnlockWindowStation",   
					"NtUserUnregisterClass",   
					"NtUserUnregisterUserApiHook",   
					"NtUserUnregisterHotKey",   
					"NtUserUpdateInputContext",   
					"NtUserUpdateInstance",  
					"NtUserUpdateLayeredWindow",  
					"NtUserGetLayeredWindowAttributes",   
					"NtUserSetLayeredWindowAttributes",   
					"NtUserUpdatePerUserSystemParameters",   
					"NtUserUserHandleGrantAccess",   
					"NtUserValidateHandleSecure",   
					"NtUserValidateRect",   
					"NtUserValidateTimerCallback",   
					"NtUserVkKeyScanEx",   
					"NtUserWaitForInputIdle",   
					"NtUserWaitForMsgAndEvent",   
					"NtUserWaitMessage",   
					"NtUserWin32PoolAllocationStats",   
					"NtUserWindowFromPoint",   
					"NtUserYieldTask",   
					"NtUserRemoteConnect",   
					"NtUserRemoteRedrawRectangle",   
					"NtUserRemoteRedrawScreen",   
					"NtUserRemoteStopScreenUpdates",   
					"NtUserCtxDisplayIOCtl",   
					"NtGdiEngAssociateSurface",  
					"NtGdiEngCreateBitmap",  
					"NtGdiEngCreateDeviceSurface",  
					"NtGdiEngCreateDeviceBitmap",   
					"NtGdiEngCreatePalette",  
					"NtGdiEngComputeGlyphSet",   
					"NtGdiEngCopyBits",   
					"NtGdiEngDeletePalette",   
					"NtGdiEngDeleteSurface",   
					"NtGdiEngEraseSurface",   
					"NtGdiEngUnlockSurface",   
					"NtGdiEngLockSurface",   
					"NtGdiEngBitBlt",   
					"NtGdiEngStretchBlt",   
					"NtGdiEngPlgBlt",   
					"NtGdiEngMarkBandingSurface",   
					"NtGdiEngStrokePath",   
					"NtGdiEngFillPath",  
					"NtGdiEngStrokeAndFillPath",  
					"NtGdiEngPaint",  
					"NtGdiEngLineTo",   
					"NtGdiEngAlphaBlend",   
					"NtGdiEngGradientFill",   
					"NtGdiEngTransparentBlt",   
					"NtGdiEngTextOut",   
					"NtGdiEngStretchBltROP",   
					"NtGdiXLATEOBJ_cGetPalette",   
					"NtGdiXLATEOBJ_iXlate",  
					"NtGdiXLATEOBJ_hGetColorTransform",  
					"NtGdiCLIPOBJ_bEnum",  
					"NtGdiCLIPOBJ_cEnumStart",  
					"NtGdiCLIPOBJ_ppoGetPath",
					"NtGdiEngDeletePath", 
					"NtGdiEngCreateClip",
					"NtGdiEngDeleteClip",
					"NtGdiBRUSHOBJ_ulGetBrushColor", 
					"NtGdiBRUSHOBJ_pvAllocRbrush", 
					"NtGdiBRUSHOBJ_pvGetRbrush", 
					"NtGdiBRUSHOBJ_hGetColorTransform", 
					"NtGdiXFORMOBJ_bApplyXform",
					"NtGdiXFORMOBJ_iGetXform",
					"NtGdiFONTOBJ_vGetInfo",
					"NtGdiFONTOBJ_pxoGetXform",
					"NtGdiFONTOBJ_cGetGlyphs",
					"NtGdiFONTOBJ_pifi",
					"NtGdiFONTOBJ_pfdg", 
					"NtGdiFONTOBJ_pQueryGlyphAttrs", 
					"NtGdiFONTOBJ_pvTrueTypeFontFile", 
					"NtGdiFONTOBJ_cGetAllGlyphHandles",
					"NtGdiSTROBJ_bEnum", 
					"NtGdiSTROBJ_bEnumPositionsOnly", 
					"NtGdiSTROBJ_bGetAdvanceWidths",  
					"NtGdiSTROBJ_vEnumStart",  
					"NtGdiSTROBJ_dwGetCodePage",  
					"NtGdiPATHOBJ_vGetBounds",  
					"NtGdiPATHOBJ_bEnum",  
					"NtGdiPATHOBJ_vEnumStart", 
					"NtGdiPATHOBJ_vEnumStartClipLines", 
					"NtGdiPATHOBJ_bEnumClipLines", 
					"NtGdiGetDhpdev", 
					"NtGdiEngCheckAbort", 
					"NtGdiHT_Get8BPPFormatPalette", 
					"NtGdiHT_Get8BPPMaskPalette", 
					"NtGdiUpdateTransform", 
					"NtGdiSetPUMPDOBJ", 
					"NtGdiBRUSHOBJ_DeleteRbrush", 
					"NtGdiUnmapMemFont", 
					"NtGdiDrawStream"
			};
		}

		//shadow WIN7函数列表
		static array<String^>^ common_getWIN7ShadowAPI()
		{
			return gcnew array<String^>{
				"NtGdiAbortDoc",
					"NtGdiAbortPath",
					"NtGdiAddFontResourceW",
					"NtGdiAddRemoteFontToDC",
					"NtGdiAddFontMemResourceEx",
					"NtGdiRemoveMergeFont",
					"NtGdiAddRemoteMMInstanceToDC",
					"NtGdiAlphaBlend",
					"NtGdiAngleArc",
					"NtGdiAnyLinkedFonts",
					"NtGdiFontIsLinked",
					"NtGdiArcInternal",
					"NtGdiBeginGdiRendering",
					"NtGdiBeginPath",
					"NtGdiBitBlt",
					"NtGdiCancelDC",
					"NtGdiCheckBitmapBits",
					"NtGdiCloseFigure",
					"NtGdiClearBitmapAttributes",
					"NtGdiClearBrushAttributes",
					"NtGdiColorCorrectPalette",
					"NtGdiCombineRgn",
					"NtGdiCombineTransform",
					"NtGdiComputeXformCoefficients",
					"NtGdiConfigureOPMProtectedOutput",
					"NtGdiConvertMetafileRect",
					"NtGdiCreateBitmap",
					"NtGdiCreateBitmapFromDxSurface",
					"NtGdiCreateClientObj",
					"NtGdiCreateColorSpace",
					"NtGdiCreateColorTransform",
					"NtGdiCreateCompatibleBitmap",
					"NtGdiCreateCompatibleDC",
					"NtGdiCreateDIBBrush",
					"NtGdiCreateDIBitmapInternal",
					"NtGdiCreateDIBSection",
					"NtGdiCreateEllipticRgn",
					"NtGdiCreateHalftonePalette",
					"NtGdiCreateHatchBrushInternal",
					"NtGdiCreateMetafileDC",
					"NtGdiCreateOPMProtectedOutputs",
					"NtGdiCreatePaletteInternal",
					"NtGdiCreatePatternBrushInternal",
					"NtGdiCreatePen",
					"NtGdiCreateRectRgn",
					"NtGdiCreateRoundRectRgn",
					"NtGdiCreateServerMetaFile",
					"NtGdiCreateSolidBrush",
					"NtGdiD3dContextCreate",
					"NtGdiD3dContextDestroy",
					"NtGdiD3dContextDestroyAll",
					"NtGdiD3dValidateTextureStageState",
					"NtGdiD3dDrawPrimitives2",
					"NtGdiDdGetDriverState",
					"NtGdiDdAddAttachedSurface",
					"NtGdiDdAlphaBlt",
					"NtGdiDdAttachSurface",
					"NtGdiDdBeginMoCompFrame",
					"NtGdiDdBlt",
					"NtGdiDdCanCreateSurface",
					"NtGdiDdCanCreateD3DBuffer",
					"NtGdiDdColorControl",
					"NtGdiDdCreateDirectDrawObject",
					"NtGdiDdCreateSurface",
					"NtGdiDdCreateD3DBuffer",
					"NtGdiDdCreateMoComp",
					"NtGdiDdCreateSurfaceObject",
					"NtGdiDdDeleteDirectDrawObject",
					"NtGdiDdDeleteSurfaceObject",
					"NtGdiDdDestroyMoComp",
					"NtGdiDdDestroySurface",
					"NtGdiDdDestroyD3DBuffer",
					"NtGdiDdEndMoCompFrame",
					"NtGdiDdFlip",
					"NtGdiDdFlipToGDISurface",
					"NtGdiDdGetAvailDriverMemory",
					"NtGdiDdGetBltStatus",
					"NtGdiDdGetDC",
					"NtGdiDdGetDriverInfo",
					"NtGdiDdGetDxHandle",
					"NtGdiDdGetFlipStatus",
					"NtGdiDdGetInternalMoCompInfo",
					"NtGdiDdGetMoCompBuffInfo",
					"NtGdiDdGetMoCompGuids",
					"NtGdiDdGetMoCompFormats",
					"NtGdiDdGetScanLine",
					"NtGdiDdLock",
					"NtGdiDdLockD3D",
					"NtGdiDdQueryDirectDrawObject",
					"NtGdiDdQueryMoCompStatus",
					"NtGdiDdReenableDirectDrawObject",
					"NtGdiDdReleaseDC",
					"NtGdiDdRenderMoComp",
					"NtGdiDdResetVisrgn",
					"NtGdiDdSetColorKey",
					"NtGdiDdSetExclusiveMode",
					"NtGdiDdSetGammaRamp",
					"NtGdiDdCreateSurfaceEx",
					"NtGdiDdSetOverlayPosition",
					"NtGdiDdUnattachSurface",
					"NtGdiDdUnlock",
					"NtGdiDdUnlockD3D",
					"NtGdiDdUpdateOverlay",
					"NtGdiDdWaitForVerticalBlank",
					"NtGdiDvpCanCreateVideoPort",
					"NtGdiDvpColorControl",
					"NtGdiDvpCreateVideoPort",
					"NtGdiDvpDestroyVideoPort",
					"NtGdiDvpFlipVideoPort",
					"NtGdiDvpGetVideoPortBandwidth",
					"NtGdiDvpGetVideoPortField",
					"NtGdiDvpGetVideoPortFlipStatus",
					"NtGdiDvpGetVideoPortInputFormats",
					"NtGdiDvpGetVideoPortLine",
					"NtGdiDvpGetVideoPortOutputFormats",
					"NtGdiDvpGetVideoPortConnectInfo",
					"NtGdiDvpGetVideoSignalStatus",
					"NtGdiDvpUpdateVideoPort",
					"NtGdiDvpWaitForVideoPortSync",
					"NtGdiDvpAcquireNotification",
					"NtGdiDvpReleaseNotification",
					"NtGdiDxgGenericThunk",
					"NtGdiDeleteClientObj",
					"NtGdiDeleteColorSpace",
					"NtGdiDeleteColorTransform",
					"NtGdiDeleteObjectApp",
					"NtGdiDescribePixelFormat",
					"NtGdiDestroyOPMProtectedOutput",
					"NtGdiGetPerBandInfo",
					"NtGdiDoBanding",
					"NtGdiDoPalette",
					"NtGdiDrawEscape",
					"NtGdiEllipse",
					"NtGdiEnableEudc",
					"NtGdiEndDoc",
					"NtGdiEndGdiRendering",
					"NtGdiEndPage",
					"NtGdiEndPath",
					"NtGdiEnumFonts",
					"NtGdiEnumObjects",
					"NtGdiEqualRgn",
					"NtGdiEudcLoadUnloadLink",
					"NtGdiExcludeClipRect",
					"NtGdiExtCreatePen",
					"NtGdiExtCreateRegion",
					"NtGdiExtEscape",
					"NtGdiExtFloodFill",
					"NtGdiExtGetObjectW",
					"NtGdiExtSelectClipRgn",
					"NtGdiExtTextOutW",
					"NtGdiFillPath",
					"NtGdiFillRgn",
					"NtGdiFlattenPath",
					"NtGdiFlush",
					"NtGdiForceUFIMapping",
					"NtGdiFrameRgn",
					"NtGdiFullscreenControl",
					"NtGdiGetAndSetDCDword",
					"NtGdiGetAppClipBox",
					"NtGdiGetBitmapBits",
					"NtGdiGetBitmapDimension",
					"NtGdiGetBoundsRect",
					"NtGdiGetCertificate",
					"NtGdiGetCertificateSize",
					"NtGdiGetCharABCWidthsW",
					"NtGdiGetCharacterPlacementW",
					"NtGdiGetCharSet",
					"NtGdiGetCharWidthW",
					"NtGdiGetCharWidthInfo",
					"NtGdiGetColorAdjustment",
					"NtGdiGetColorSpaceforBitmap",
					"NtGdiGetCOPPCompatibleOPMInformation",
					"NtGdiGetDCDword",
					"NtGdiGetDCforBitmap",
					"NtGdiGetDCObject",
					"NtGdiGetDCPoint",
					"NtGdiGetDeviceCaps",
					"NtGdiGetDeviceGammaRamp",
					"NtGdiGetDeviceCapsAll",
					"NtGdiGetDIBitsInternal",
					"NtGdiGetETM",
					"NtGdiGetEudcTimeStampEx",
					"NtGdiGetFontData",
					"NtGdiGetFontFileData",
					"NtGdiGetFontFileInfo",
					"NtGdiGetFontResourceInfoInternalW",
					"NtGdiGetGlyphIndicesW",
					"NtGdiGetGlyphIndicesWInternal",
					"NtGdiGetGlyphOutline",
					"NtGdiGetOPMInformation",
					"NtGdiGetKerningPairs",
					"NtGdiGetLinkedUFIs",
					"NtGdiGetMiterLimit",
					"NtGdiGetMonitorID",
					"NtGdiGetNearestColor",
					"NtGdiGetNearestPaletteIndex",
					"NtGdiGetObjectBitmapHandle",
					"NtGdiGetOPMRandomNumber",
					"NtGdiGetOutlineTextMetricsInternalW",
					"NtGdiGetPath",
					"NtGdiGetPixel",
					"NtGdiGetRandomRgn",
					"NtGdiGetRasterizerCaps",
					"NtGdiGetRealizationInfo",
					"NtGdiGetRegionData",
					"NtGdiGetRgnBox",
					"NtGdiGetServerMetaFileBits",
					"DxgStubContextCreate",
					"NtGdiGetStats",
					"NtGdiGetStockObject",
					"NtGdiGetStringBitmapW",
					"NtGdiGetSuggestedOPMProtectedOutputArraySize",
					"NtGdiGetSystemPaletteUse",
					"NtGdiGetTextCharsetInfo",
					"NtGdiGetTextExtent",
					"NtGdiGetTextExtentExW",
					"NtGdiGetTextFaceW",
					"NtGdiGetTextMetricsW",
					"NtGdiGetTransform",
					"NtGdiGetUFI",
					"NtGdiGetEmbUFI",
					"NtGdiGetUFIPathname",
					"NtGdiGetEmbedFonts",
					"NtGdiChangeGhostFont",
					"NtGdiAddEmbFontToDC",
					"NtGdiGetFontUnicodeRanges",
					"NtGdiGetWidthTable",
					"NtGdiGradientFill",
					"NtGdiHfontCreate",
					"NtGdiIcmBrushInfo",
					"IsIMMEnabledSystem",
					"NtGdiInitSpool",
					"NtGdiIntersectClipRect",
					"NtGdiInvertRgn",
					"NtGdiLineTo",
					"NtGdiMakeFontDir",
					"NtGdiMakeInfoDC",
					"NtGdiMaskBlt",
					"NtGdiModifyWorldTransform",
					"NtGdiMonoBitmap",
					"NtGdiMoveTo",
					"NtGdiOffsetClipRgn",
					"NtGdiOffsetRgn",
					"NtGdiOpenDCW",
					"NtGdiPatBlt",
					"NtGdiPolyPatBlt",
					"NtGdiPathToRegion",
					"NtGdiPlgBlt",
					"NtGdiPolyDraw",
					"NtGdiPolyPolyDraw",
					"NtGdiPolyTextOutW",
					"NtGdiPtInRegion",
					"NtGdiPtVisible",
					"NtGdiQueryFonts",
					"NtGdiQueryFontAssocInfo",
					"NtGdiRectangle",
					"NtGdiRectInRegion",
					"NtGdiRectVisible",
					"NtGdiRemoveFontResourceW",
					"NtGdiRemoveFontMemResourceEx",
					"NtGdiResetDC",
					"NtGdiResizePalette",
					"NtGdiRestoreDC",
					"NtGdiRoundRect",
					"NtGdiSaveDC",
					"NtGdiScaleViewportExtEx",
					"NtGdiScaleWindowExtEx",
					"GreSelectBitmap",
					"NtGdiSelectBrush",
					"NtGdiSelectClipPath",
					"NtGdiSelectFont",
					"NtGdiSelectPen",
					"NtGdiSetBitmapAttributes",
					"NtGdiSetBitmapBits",
					"NtGdiSetBitmapDimension",
					"NtGdiSetBoundsRect",
					"NtGdiSetBrushAttributes",
					"NtGdiSetBrushOrg",
					"NtGdiSetColorAdjustment",
					"NtGdiSetColorSpace",
					"NtGdiSetDeviceGammaRamp",
					"NtGdiSetDIBitsToDeviceInternal",
					"NtGdiSetFontEnumeration",
					"NtGdiSetFontXform",
					"NtGdiSetIcmMode",
					"NtGdiSetLinkedUFIs",
					"NtGdiSetMagicColors",
					"NtGdiSetMetaRgn",
					"NtGdiSetMiterLimit",
					"NtGdiGetDeviceWidth",
					"NtGdiMirrorWindowOrg",
					"NtGdiSetLayout",
					"NtGdiSetOPMSigningKeyAndSequenceNumbers",
					"NtGdiSetPixel",
					"NtGdiSetPixelFormat",
					"NtGdiSetRectRgn",
					"NtGdiSetSystemPaletteUse",
					"NtGdiSetTextJustification",
					"NtGdiSetVirtualResolution",
					"NtGdiSetSizeDevice",
					"NtGdiStartDoc",
					"NtGdiStartPage",
					"NtGdiStretchBlt",
					"NtGdiStretchDIBitsInternal",
					"NtGdiStrokeAndFillPath",
					"NtGdiStrokePath",
					"NtGdiSwapBuffers",
					"NtGdiTransformPoints",
					"NtGdiTransparentBlt",
					"DxgStubEnableDirectDrawRedirection",
					"NtGdiUnmapMemFont",
					"NtGdiUnrealizeObject",
					"NtGdiUpdateColors",
					"NtGdiWidenPath",
					"NtUserActivateKeyboardLayout",
					"NtUserAddClipboardFormatListener",
					"NtUserAlterWindowStyle",
					"NtUserAssociateInputContext",
					"NtUserAttachThreadInput",
					"NtUserBeginPaint",
					"NtUserBitBltSysBmp",
					"NtUserBlockInput",
					"NtUserBuildHimcList",
					"NtUserBuildHwndList",
					"NtUserBuildNameList",
					"NtUserBuildPropList",
					"NtUserCallHwnd",
					"NtUserCallHwndLock",
					"NtUserCallHwndOpt",
					"NtUserCallHwndParam",
					"NtUserCallHwndParamLock",
					"NtUserCallMsgFilter",
					"NtUserCallNextHookEx",
					"NtUserCallNoParam",
					"NtUserCallOneParam",
					"NtUserCallTwoParam",
					"NtUserChangeClipboardChain",
					"NtUserChangeDisplaySettings",
					"NtUserGetDisplayConfigBufferSizes",
					"NtUserSetDisplayConfig",
					"NtUserQueryDisplayConfig",
					"NtUserDisplayConfigGetDeviceInfo",
					"NtUserDisplayConfigSetDeviceInfo",
					"NtUserCheckAccessForIntegrityLevel",
					"NtUserCheckDesktopByThreadId",
					"NtUserCheckWindowThreadDesktop",
					"NtUserCheckMenuItem",
					"NtUserChildWindowFromPointEx",
					"NtUserClipCursor",
					"NtUserCloseClipboard",
					"NtUserCloseDesktop",
					"NtUserCloseWindowStation",
					"NtUserConsoleControl",
					"NtUserConvertMemHandle",
					"NtUserCopyAcceleratorTable",
					"NtUserCountClipboardFormats",
					"NtUserCreateAcceleratorTable",
					"NtUserCreateCaret",
					"NtUserCreateDesktopEx",
					"NtUserCreateInputContext",
					"NtUserCreateLocalMemHandle",
					"NtUserCreateWindowEx",
					"NtUserCreateWindowStation",
					"NtUserDdeInitialize",
					"NtUserDeferWindowPos",
					"NtUserDefSetText",
					"NtUserDeleteMenu",
					"NtUserDestroyAcceleratorTable",
					"NtUserDestroyCursor",
					"NtUserDestroyInputContext",
					"NtUserDestroyMenu",
					"NtUserDestroyWindow",
					"NtUserDisableThreadIme",
					"NtUserDispatchMessage",
					"NtUserDoSoundConnect",
					"NtUserDoSoundDisconnect",
					"NtUserDragDetect",
					"NtUserDragObject",
					"NtUserDrawAnimatedRects",
					"NtUserDrawCaption",
					"NtUserDrawCaptionTemp",
					"NtUserDrawIconEx",
					"NtUserDrawMenuBarTemp",
					"NtUserEmptyClipboard",
					"NtUserEnableMenuItem",
					"NtUserEnableScrollBar",
					"NtUserEndDeferWindowPosEx",
					"NtUserEndMenu",
					"NtUserEndPaint",
					"NtUserEnumDisplayDevices",
					"NtUserEnumDisplayMonitors",
					"NtUserEnumDisplaySettings",
					"NtUserEvent",
					"NtUserExcludeUpdateRgn",
					"NtUserFillWindow",
					"NtUserFindExistingCursorIcon",
					"NtUserFindWindowEx",
					"NtUserFlashWindowEx",
					"NtUserFrostCrashedWindow",
					"NtUserGetAltTabInfo",
					"NtUserGetAncestor",
					"NtUserGetAppImeLevel",
					"NtUserGetAsyncKeyState",
					"NtUserGetAtomName",
					"NtUserGetCaretBlinkTime",
					"NtUserGetCaretPos",
					"NtUserGetClassInfoEx",
					"NtUserGetClassName",
					"NtUserGetClipboardData",
					"NtUserGetClipboardFormatName",
					"NtUserGetClipboardOwner",
					"NtUserGetClipboardSequenceNumber",
					"NtUserGetClipboardViewer",
					"NtUserGetClipCursor",
					"NtUserGetComboBoxInfo",
					"NtUserGetControlBrush",
					"NtUserGetControlColor",
					"NtUserGetCPD",
					"NtUserGetCursorFrameInfo",
					"NtUserGetCursorInfo",
					"NtUserGetDC",
					"NtUserGetDCEx",
					"NtUserGetDoubleClickTime",
					"NtUserGetForegroundWindow",
					"NtUserGetGuiResources",
					"NtUserGetGUIThreadInfo",
					"NtUserGetIconInfo",
					"NtUserGetIconSize",
					"NtUserGetImeHotKey",
					"NtUserGetImeInfoEx",
					"NtUserGetInputLocaleInfo",
					"NtUserGetInternalWindowPos",
					"NtUserGetKeyboardLayoutList",
					"NtUserGetKeyboardLayoutName",
					"NtUserGetKeyboardState",
					"NtUserGetKeyNameText",
					"NtUserGetKeyState",
					"NtUserGetListBoxInfo",
					"NtUserGetMenuBarInfo",
					"NtUserGetMenuIndex",
					"NtUserGetMenuItemRect",
					"NtUserGetMessage",
					"NtUserGetMouseMovePointsEx",
					"NtUserGetObjectInformation",
					"NtUserGetOpenClipboardWindow",
					"NtUserGetPriorityClipboardFormat",
					"NtUserGetProcessWindowStation",
					"NtUserGetRawInputBuffer",
					"NtUserGetRawInputData",
					"NtUserGetRawInputDeviceInfo",
					"NtUserGetRawInputDeviceList",
					"NtUserGetRegisteredRawInputDevices",
					"NtUserGetScrollBarInfo",
					"NtUserGetSystemMenu",
					"NtUserGetThreadDesktop",
					"NtUserGetThreadState",
					"NtUserGetTitleBarInfo",
					"NtUserGetTopLevelWindow",
					"NtUserGetUpdatedClipboardFormats",
					"NtUserGetUpdateRect",
					"NtUserGetUpdateRgn",
					"NtUserGetWindowCompositionInfo",
					"NtUserGetWindowCompositionAttribute",
					"NtUserGetWindowDC",
					"NtUserGetWindowDisplayAffinity",
					"NtUserGetWindowPlacement",
					"NtUserGetWOWClass",
					"NtUserGhostWindowFromHungWindow",
					"NtUserHardErrorControl",
					"NtUserHideCaret",
					"NtUserHiliteMenuItem",
					"NtUserHungWindowFromGhostWindow",
					"NtUserImpersonateDdeClientWindow",
					"NtUserInitialize",
					"NtUserInitializeClientPfnArrays",
					"NtUserInitTask",
					"NtUserInternalGetWindowText",
					"NtUserInternalGetWindowIcon",
					"NtUserInvalidateRect",
					"NtUserInvalidateRgn",
					"NtUserIsClipboardFormatAvailable",
					"NtUserIsTopLevelWindow",
					"NtUserKillTimer",
					"NtUserLoadKeyboardLayoutEx",
					"NtUserLockWindowStation",
					"NtUserLockWindowUpdate",
					"NtUserLockWorkStation",
					"NtUserLogicalToPhysicalPoint",
					"NtUserMapVirtualKeyEx",
					"NtUserMenuItemFromPoint",
					"NtUserMessageCall",
					"NtUserMinMaximize",
					"NtUserMNDragLeave",
					"NtUserMNDragOver",
					"NtUserModifyUserStartupInfoFlags",
					"NtUserMoveWindow",
					"NtUserNotifyIMEStatus",
					"NtUserNotifyProcessCreate",
					"NtUserNotifyWinEvent",
					"NtUserOpenClipboard",
					"NtUserOpenDesktop",
					"NtUserOpenInputDesktop",
					"NtUserOpenThreadDesktop",
					"NtUserOpenWindowStation",
					"NtUserPaintDesktop",
					"NtUserPaintMonitor",
					"NtUserPeekMessage",
					"NtUserPhysicalToLogicalPoint",
					"NtUserPostMessage",
					"NtUserPostThreadMessage",
					"NtUserPrintWindow",
					"NtUserProcessConnect",
					"NtUserQueryInformationThread",
					"NtUserQueryInputContext",
					"NtUserQuerySendMessage",
					"NtUserQueryWindow",
					"NtUserRealChildWindowFromPoint",
					"NtUserRealInternalGetMessage",
					"NtUserRealWaitMessageEx",
					"NtUserRedrawWindow",
					"NtUserRegisterClassExWOW",
					"NtUserRegisterErrorReportingDialog",
					"NtUserRegisterUserApiHook",
					"NtUserRegisterHotKey",
					"NtUserRegisterRawInputDevices",
					"NtUserRegisterServicesProcess",
					"NtUserRegisterTasklist",
					"NtUserRegisterWindowMessage",
					"NtUserRemoveClipboardFormatListener",
					"NtUserRemoveMenu",
					"NtUserRemoveProp",
					"NtUserResolveDesktopForWOW",
					"NtUserSBGetParms",
					"NtUserScrollDC",
					"NtUserScrollWindowEx",
					"NtUserSelectPalette",
					"NtUserSendInput",
					"NtUserSetActiveWindow",
					"NtUserSetAppImeLevel",
					"NtUserSetCapture",
					"NtUserSetChildWindowNoActivate",
					"NtUserSetClassLong",
					"NtUserSetClassWord",
					"NtUserSetClipboardData",
					"NtUserSetClipboardViewer",
					"NtUserSetCursor",
					"NtUserSetCursorContents",
					"NtUserSetCursorIconData",
					"NtUserSetFocus",
					"NtUserSetImeHotKey",
					"NtUserSetImeInfoEx",
					"NtUserSetImeOwnerWindow",
					"NtUserSetInformationThread",
					"NtUserSetInternalWindowPos",
					"NtUserSetKeyboardState",
					"NtUserSetMenu",
					"NtUserSetMenuContextHelpId",
					"NtUserSetMenuDefaultItem",
					"NtUserSetMenuFlagRtoL",
					"NtUserSetObjectInformation",
					"NtUserSetParent",
					"NtUserSetProcessWindowStation",
					"NtUserGetProp",
					"NtUserSetProp",
					"NtUserSetScrollInfo",
					"NtUserSetShellWindowEx",
					"NtUserSetSysColors",
					"NtUserSetSystemCursor",
					"NtUserSetSystemMenu",
					"NtUserSetSystemTimer",
					"NtUserSetThreadDesktop",
					"NtUserSetThreadLayoutHandles",
					"NtUserSetThreadState",
					"NtUserSetTimer",
					"NtUserSetProcessDPIAware",
					"NtUserSetWindowCompositionAttribute",
					"NtUserSetWindowDisplayAffinity",
					"NtUserSetWindowFNID",
					"NtUserSetWindowLong",
					"NtUserSetWindowPlacement",
					"NtUserSetWindowPos",
					"NtUserSetWindowRgn",
					"NtUserGetWindowRgnEx",
					"NtUserSetWindowRgnEx",
					"NtUserSetWindowsHookAW",
					"NtUserSetWindowsHookEx",
					"NtUserSetWindowStationUser",
					"NtUserSetWindowWord",
					"NtUserSetWinEventHook",
					"NtUserShowCaret",
					"NtUserShowScrollBar",
					"NtUserShowWindow",
					"NtUserShowWindowAsync",
					"NtUserSoundSentry",
					"NtUserSwitchDesktop",
					"NtUserSystemParametersInfo",
					"NtUserTestForInteractiveUser",
					"NtUserThunkedMenuInfo",
					"NtUserThunkedMenuItemInfo",
					"NtUserToUnicodeEx",
					"NtUserTrackMouseEvent",
					"NtUserTrackPopupMenuEx",
					"NtUserCalculatePopupWindowPosition",
					"NtUserCalcMenuBar",
					"NtUserPaintMenuBar",
					"NtUserTranslateAccelerator",
					"NtUserTranslateMessage",
					"NtUserUnhookWindowsHookEx",
					"NtUserUnhookWinEvent",
					"NtUserUnloadKeyboardLayout",
					"NtUserUnlockWindowStation",
					"NtUserUnregisterClass",
					"NtUserUnregisterUserApiHook",
					"NtUserUnregisterHotKey",
					"NtUserUpdateInputContext",
					"NtUserUpdateInstance",
					"NtUserUpdateLayeredWindow",
					"NtUserGetLayeredWindowAttributes",
					"NtUserSetLayeredWindowAttributes",
					"NtUserUpdatePerUserSystemParameters",
					"NtUserUserHandleGrantAccess",
					"NtUserValidateHandleSecure",
					"NtUserValidateRect",
					"NtUserValidateTimerCallback",
					"NtUserVkKeyScanEx",
					"NtUserWaitForInputIdle",
					"NtUserWaitForMsgAndEvent",
					"NtUserWaitMessage",
					"NtUserWindowFromPhysicalPoint",
					"NtUserWindowFromPoint",
					"NtUserYieldTask",
					"NtUserRemoteConnect",
					"NtUserRemoteRedrawRectangle",
					"NtUserRemoteRedrawScreen",
					"NtUserRemoteStopScreenUpdates",
					"NtUserCtxDisplayIOCtl",
					"NtUserRegisterSessionPort",
					"NtUserUnregisterSessionPort",
					"NtUserUpdateWindowTransform",
					"NtUserDwmStartRedirection",
					"NtUserDwmStopRedirection",
					"NtUserGetWindowMinimizeRect",
					"NtUserSfmDxBindSwapChain",
					"NtUserSfmDxOpenSwapChain",
					"NtUserSfmDxReleaseSwapChain",
					"NtUserSfmDxSetSwapChainBindingStatus",
					"NtUserSfmDxQuerySwapChainBindingStatus",
					"NtUserSfmDxReportPendingBindingsToDwm",
					"NtUserSfmDxGetSwapChainStats",
					"NtUserSfmDxSetSwapChainStats",
					"NtUserSfmGetLogicalSurfaceBinding",
					"NtUserSfmDestroyLogicalSurfaceBinding",
					"NtUserModifyWindowTouchCapability",
					"NtUserIsTouchWindow",
					"NtUserSendTouchInput",
					"NtUserEndTouchOperation",
					"NtUserGetTouchInputInfo",
					"NtUserChangeWindowMessageFilterEx",
					"NtUserInjectGesture",
					"NtUserGetGestureInfo",
					"NtUserGetGestureExtArgs",
					"NtUserManageGestureHandlerWindow",
					"NtUserSetGestureConfig",
					"NtUserGetGestureConfig",
					"NtGdiEngAssociateSurface",
					"NtGdiEngCreateBitmap",
					"NtGdiEngCreateDeviceSurface",
					"NtGdiEngCreateDeviceBitmap",
					"NtGdiEngCreatePalette",
					"NtGdiEngComputeGlyphSet",
					"NtGdiEngCopyBits",
					"NtGdiEngDeletePalette",
					"NtGdiEngDeleteSurface",
					"NtGdiEngEraseSurface",
					"NtGdiEngUnlockSurface",
					"NtGdiEngLockSurface",
					"NtGdiEngBitBlt",
					"NtGdiEngStretchBlt",
					"NtGdiEngPlgBlt",
					"NtGdiEngMarkBandingSurface",
					"NtGdiEngStrokePath",
					"NtGdiEngFillPath",
					"NtGdiEngStrokeAndFillPath",
					"NtGdiEngPaint",
					"NtGdiEngLineTo",
					"NtGdiEngAlphaBlend",
					"NtGdiEngGradientFill",
					"NtGdiEngTransparentBlt",
					"NtGdiEngTextOut",
					"NtGdiEngStretchBltROP",
					"NtGdiXLATEOBJ_cGetPalette",
					"NtGdiXLATEOBJ_iXlate",
					"NtGdiXLATEOBJ_hGetColorTransform",
					"NtGdiCLIPOBJ_bEnum",
					"NtGdiCLIPOBJ_cEnumStart",
					"NtGdiCLIPOBJ_ppoGetPath",
					"NtGdiEngDeletePath",
					"NtGdiEngCreateClip",
					"NtGdiEngDeleteClip",
					"NtGdiBRUSHOBJ_ulGetBrushColor",
					"NtGdiBRUSHOBJ_pvAllocRbrush",
					"NtGdiBRUSHOBJ_pvGetRbrush",
					"NtGdiBRUSHOBJ_hGetColorTransform",
					"NtGdiXFORMOBJ_bApplyXform",
					"NtGdiXFORMOBJ_iGetXform",
					"NtGdiFONTOBJ_vGetInfo",
					"NtGdiFONTOBJ_pxoGetXform",
					"NtGdiFONTOBJ_cGetGlyphs",
					"NtGdiFONTOBJ_pifi",
					"NtGdiFONTOBJ_pfdg",
					"NtGdiFONTOBJ_pQueryGlyphAttrs",
					"NtGdiFONTOBJ_pvTrueTypeFontFile",
					"NtGdiFONTOBJ_cGetAllGlyphHandles",
					"NtGdiSTROBJ_bEnum",
					"NtGdiSTROBJ_bEnumPositionsOnly",
					"NtGdiSTROBJ_bGetAdvanceWidths",
					"NtGdiSTROBJ_vEnumStart",
					"NtGdiSTROBJ_dwGetCodePage",
					"NtGdiPATHOBJ_vGetBounds",
					"NtGdiPATHOBJ_bEnum",
					"NtGdiPATHOBJ_vEnumStart",
					"NtGdiPATHOBJ_vEnumStartClipLines",
					"NtGdiPATHOBJ_bEnumClipLines",
					"NtGdiGetDhpdev",
					"NtGdiEngCheckAbort",
					"NtGdiHT_Get8BPPFormatPalette",
					"NtGdiHT_Get8BPPMaskPalette",
					"NtGdiUpdateTransform",
					"NtGdiSetPUMPDOBJ",
					"NtGdiBRUSHOBJ_DeleteRbrush",
					"NtGdiUnmapMemFont",
					"NtGdiDrawStream",
					"NtGdiSfmGetNotificationTokens",
					"NtGdiHLSurfGetInformation",
					"NtGdiHLSurfSetInformation",
					"NtGdiDdDDICreateAllocation",
					"NtGdiDdDDIQueryResourceInfo",
					"NtGdiDdDDIOpenResource",
					"NtGdiDdDDIDestroyAllocation",
					"NtGdiDdDDISetAllocationPriority",
					"NtGdiDdDDIQueryAllocationResidency",
					"NtGdiDdDDICreateDevice",
					"NtGdiDdDDIDestroyDevice",
					"NtGdiDdDDICreateContext",
					"NtGdiDdDDIDestroyContext",
					"NtGdiDdDDICreateSynchronizationObject",
					"NtGdiDdDDIOpenSynchronizationObject",
					"NtGdiDdDDIDestroySynchronizationObject",
					"NtGdiDdDDIWaitForSynchronizationObject",
					"NtGdiDdDDISignalSynchronizationObject",
					"NtGdiDdDDIGetRuntimeData",
					"NtGdiDdDDIQueryAdapterInfo",
					"NtGdiDdDDILock",
					"NtGdiDdDDIUnlock",
					"NtGdiDdDDIGetDisplayModeList",
					"NtGdiDdDDISetDisplayMode",
					"NtGdiDdDDIGetMultisampleMethodList",
					"NtGdiDdDDIPresent",
					"NtGdiDdDDIRender",
					"NtGdiDdDDIOpenAdapterFromDeviceName",
					"NtGdiDdDDIOpenAdapterFromHdc",
					"NtGdiDdDDICloseAdapter",
					"NtGdiDdDDIGetSharedPrimaryHandle",
					"NtGdiDdDDIEscape",
					"NtGdiDdDDIQueryStatistics",
					"NtGdiDdDDISetVidPnSourceOwner",
					"NtGdiDdDDIGetPresentHistory",
					"NtGdiDdDDIGetPresentQueueEvent",
					"NtGdiDdDDICreateOverlay",
					"NtGdiDdDDIUpdateOverlay",
					"NtGdiDdDDIFlipOverlay",
					"NtGdiDdDDIDestroyOverlay",
					"NtGdiDdDDIWaitForVerticalBlankEvent",
					"NtGdiDdDDISetGammaRamp",
					"NtGdiDdDDIGetDeviceState",
					"NtGdiDdDDICreateDCFromMemory",
					"NtGdiDdDDIDestroyDCFromMemory",
					"NtGdiDdDDISetContextSchedulingPriority",
					"NtGdiDdDDIGetContextSchedulingPriority",
					"NtGdiDdDDISetProcessSchedulingPriorityClass",
					"NtGdiDdDDIGetProcessSchedulingPriorityClass",
					"NtGdiDdDDIReleaseProcessVidPnSourceOwners",
					"NtGdiDdDDIGetScanLine",
					"NtGdiDdDDISetQueuedLimit",
					"NtGdiDdDDIPollDisplayChildren",
					"NtGdiDdDDIInvalidateActiveVidPn",
					"NtGdiDdDDICheckOcclusion",
					"NtGdiDdDDIWaitForIdle",
					"NtGdiDdDDICheckMonitorPowerState",
					"NtGdiDdDDICheckExclusiveOwnership",
					"NtGdiDdDDISetDisplayPrivateDriverFormat",
					"NtGdiDdDDISharedPrimaryLockNotification",
					"NtGdiDdDDISharedPrimaryUnLockNotification",
					"NtGdiDdDDICreateKeyedMutex",
					"NtGdiDdDDIOpenKeyedMutex",
					"NtGdiDdDDIDestroyKeyedMutex",
					"NtGdiDdDDIAcquireKeyedMutex",
					"NtGdiDdDDIReleaseKeyedMutex",
					"NtGdiDdDDIConfigureSharedResource",
					"NtGdiDdDDIGetOverlayState",
					"NtGdiDdDDICheckVidPnExclusiveOwnership",
					"NtGdiDdDDICheckSharedResourceAccess",
					"DxgStubEnableDirectDrawRedirection",
					"DxgStubDeleteDirectDrawObject",
					"NtGdiGetNumberOfPhysicalMonitors",
					"NtGdiGetPhysicalMonitors",
					"NtGdiGetPhysicalMonitorDescription",
					"DestroyPhysicalMonitor",
					"NtGdiDDCCIGetVCPFeature",
					"NtGdiDDCCISetVCPFeature",
					"NtGdiDDCCISaveCurrentSettings",
					"NtGdiDDCCIGetCapabilitiesStringLength",
					"NtGdiDDCCIGetCapabilitiesString",
					"NtGdiDDCCIGetTimingReport",
					"NtGdiDdCreateFullscreenSprite",
					"NtGdiDdNotifyFullscreenSpriteUpdate",
					"NtGdiDdDestroyFullscreenSprite",
					"NtGdiDdQueryVisRgnUniqueness",
					"NtUserSetMirrorRendering",
					"NtUserShowSystemCursor",
					"NtUserMagControl",
					"NtUserMagSetContextInformation",
					"NtUserMagGetContextInformation",
					"NtUserHwndQueryRedirectionInfo",
					"NtUserHwndSetRedirectionInfo"
			};
		}

		//
		static LPSTR common_stringToHGlobalAnsi(String^ str_){
			return (LPSTR)(LPVOID)Marshal::StringToHGlobalAnsi(str_);
		}

		//
		static LPTSTR common_stringToHGlobalUni(String^ str_){
			return (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_);
		}

		//字节转字符串
		static System::String^ common_byteToString(array<byte>^ b_)
		{
			return Encoding::Unicode->GetString(b_);
		}

		//字节转字符串
		static System::String^ common_byteToString2(array<byte>^ b_)
		{
			String^ str_ = nullptr;
			for each(byte b in b_)
			{
				str_ = str_ + (b).ToString("X2") + " ";
			}
			return str_;
		}

		//url编码
		static System::String^ common_urlEncode(String^ str_)
		{
			try
			{
				//StringBuilder^ sb_ = gcnew StringBuilder();
				//array<byte^>^ bts_ = System::Text::Encoding::UTF8->GetBytes(str_);

				//for (int i = 0; i < bts_->Length; i++)
				//{
				//	sb_->Append("%" + Convert::ToString(bts_[i], 16));
				//}

				//return (sb->ToString());

				return System::Web::HttpUtility::UrlEncode(str_);
			}
			catch(Exception^ ex){ }
		}

		//url解码
		static System::String^ common_urlDecode(String^ str_)
		{
			try
			{
				return System::Web::HttpUtility::UrlDecode(str_);
			}
			catch(Exception^ ex){ }
		}

		//获得短路径
		//返回LPWSTR
		static LPWSTR common_getShortPath(String^ str_){
			WCHAR wsShortPath[MAX_PATH] = {0};
			GetShortPathName(common_net::common_stringToHGlobalUni(str_),wsShortPath,MAX_PATH);
			return wsShortPath;
		}

		//通配符和正则之间的转换
		static System::String^ common_getRegexEscapeString(String^ str_){
			return "^" + Regex::Escape(str_)->Replace( "\\*", ".*" )->Replace( "\\?", "." ) + "$";
		}

		//字节大小单位的换算
		static System::String^ common_getUnitConversionString(ULONG ul_){
					 double d = 0.0;

					 //如果大于G
					 if (ul_ > 1024*1024*1024)
					 {
						 d = ul_/(1024*1024*1024*1.0);
						 return (d).ToString("F2") +"G";
					 }
					 //如果大于MB
					 if(ul_ > 1024*1024)
					 {
						 d = ul_/(1024*1024*1.0);
						 return (d).ToString("F2") +"MB";
					 }
					 //如果大于KB
					 if(ul_ > 1024)
					 {
						 d = ul_/(1024*1.0);
						 return (d).ToString("F2") +"KB";
					 }

					 return ul_.ToString() +"Bytes";
				 }

		//从注册表中获得软件安装目录
		static System::String^ common_getSoftwareInstallLocation(String^ str_,String^ str2_){
			try
			{
				RegistryKey^ rk_ = Registry::LocalMachine->OpenSubKey("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall\\" + str_);
				str_ = rk_->GetValue(str2_)->ToString();
				rk_->Close();

				return str_;
			}
			catch(Exception^ ex){ return nullptr;}	
		}
	
		//获得目标项值
		static System::String^ common_getRegkeyValue(RegistryKey^ rk_,String^ str_,String^ str2_){
			try
			{
				RegistryKey^ rk2_ = rk_->OpenSubKey(str_);
				str_ = rk2_->GetValue(str2_)->ToString();
				rk2_->Close();

				return str_;
			}
			catch(Exception^ ex){ return nullptr;}	
		}

		//在线搜索
		static System::Void common_onlineFind(String^ str_){
			Process::Start("http://www.baidu.com/s?wd="+str_);
		}

		//在线分析
		static System::Void common_onlineAnalysis(){
			Process::Start("http://www.virscan.org/");
		}

		//读配置信息
		static System::String^ config_getPrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_filePath) {
			LPTSTR lpSecName = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_secName);
			LPTSTR lpKeyName = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_keyName);
			LPTSTR lpFilePath = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_filePath);
			TCHAR lpBuffer[1024];
			GetPrivateProfileString(lpSecName,
				lpKeyName,
				NULL,
				lpBuffer, 
				1024, 
				lpFilePath);
			//返回
			return gcnew String(lpBuffer);
		}

		//写配置信息
		static System::Void config_writePrivateProfileString(String^ str_secName,String^ str_keyName,String^ str_keyValue,String^ str_filePath) {
			LPTSTR lpSecName = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_secName);
			LPTSTR lpKeyName = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_keyName);
			LPTSTR lpKeyValue = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_keyValue);
			LPTSTR lpFilePath = (LPTSTR)(LPVOID)Marshal::StringToHGlobalUni(str_filePath);
			WritePrivateProfileString(lpSecName, 
				lpKeyName,
				lpKeyValue, 
				lpFilePath);
		}

		//根据进程ID获得进程名称
		static System::String^ common_getProcessName(int PID) {
			try
			{
				//
				Process^ p = Process::GetProcessById(PID);
				//
				if (PID==0)
				{
					//
					return "Idle";
				}
				//
				else if (PID==4)
				{
					//
					return "System";
				}
				//
				return p->MainModule->ModuleName;
			}
			catch(Exception^ ex){ return ""; }			
		}

		//根据进程ID获得进程映像文件路径
		static System::String^ common_getProcessPath(int PID) {
			try
			{
				//
				Process^ p = Process::GetProcessById(PID);
				//
				if (PID==0)
				{
					//
					return "Idle";
				}
				//
				else if (PID==4)
				{
					//
					return "System";
				}
				//
				return p->MainModule->FileName;
			}
			catch(Exception^ ex){ return ""; }	
		}

		//根据进程ID获得进程映像文件生产厂商
		static System::String^ common_getProcessCompanyName(int PID) {
			try
			{
				//
				Process^ p = Process::GetProcessById(PID);
				//
				if (p->MainModule->FileName->Contains("\\??\\"))
				{
					return common_getFileCompanyName(common_formatString(p->MainModule->FileName));
				}
				else
				{
					return p->MainModule->FileVersionInfo->CompanyName;
				}
			}
			catch(Exception^ ex){ return ""; }		
		}

		//获得进程的模块信息
		static System::Void common_getProcessModuleInfo(){
			PPROCESS_MODULE_INFO p;

			//初始化
			pProcessModuleInfoHead = pProcessModuleInfo = new PROCESS_MODULE_INFO;

			for each(Process^ p2 in Process::GetProcesses(Environment::MachineName)){
				try
				{
					ProcessModuleCollection^ pmc_ = p2->Modules;

					for each(ProcessModule^ pm_ in pmc_)
					{
						p = new PROCESS_MODULE_INFO;
						wcscpy_s(p->FileName,common_net::common_stringToHGlobalUni(pm_->ModuleName));
						wcscpy_s(p->FilePath,common_net::common_stringToHGlobalUni(pm_->FileName));
						p->ulBaseAddress = (ULONG)(HANDLE)pm_->BaseAddress;
						p->ulSize = pm_->ModuleMemorySize;
						p->next = NULL;

						pProcessModuleInfo->next = p;
						pProcessModuleInfo = p;	 
					}			
				}
				catch(Exception^ ex){ }
			}

			pProcessModuleInfo = pProcessModuleInfoHead->next;
		}

		//获得进程的模块信息
		static System::Void common_getProcessModuleInfo(ULONG ulPID){
			PPROCESS_MODULE_INFO p;

			//初始化
			pProcessModuleInfoHead = pProcessModuleInfo = new PROCESS_MODULE_INFO;

			Process^ p2 = Process::GetProcessById(ulPID);
			ProcessModuleCollection^ pmc_ = p2->Modules;

			for each(ProcessModule^ pm_ in pmc_)
			{
				p = new PROCESS_MODULE_INFO;
				wcscpy_s(p->FileName,common_net::common_stringToHGlobalUni(pm_->ModuleName));
				wcscpy_s(p->FilePath,common_net::common_stringToHGlobalUni(pm_->FileName));
				p->ulBaseAddress = (ULONG)(HANDLE)pm_->BaseAddress;
				p->ulSize = pm_->ModuleMemorySize;
				p->next = NULL;

				pProcessModuleInfo->next = p;
				pProcessModuleInfo = p;	 
			}				
			
			pProcessModuleInfo = pProcessModuleInfoHead->next;
		}

		//释放内存
		static System::Void common_freeProcessModuleInfo(){
			PPROCESS_MODULE_INFO p;
			while(pProcessModuleInfo)
			{
				p = pProcessModuleInfo->next;
				free(pProcessModuleInfo);
				pProcessModuleInfo = p;
			}
			if (pProcessModuleInfoHead)
			{
				free(pProcessModuleInfoHead);
			}
		}

		//获得线程所在模块       
		static PPROCESS_MODULE_INFO common_getProcessModuleName(ULONG ulBaseAddress)
		{
			PPROCESS_MODULE_INFO p;

			for (p = pProcessModuleInfo;p;p = p->next)
			{
				if (p->ulBaseAddress <= ulBaseAddress && ulBaseAddress <= p->ulBaseAddress + p->ulSize)
				{
					//return gcnew String(p->FileName);
					return p;
				}
			}	 

			return NULL;
		}

		//获得文件生产厂商
		static System::String^ common_getFileCompanyName(String^ str_) {
			try
			{
				//转换成小写字符串
				String^ str_file = str_->ToLower();
				//获得文件生产厂商
				FileVersionInfo^ fvi = FileVersionInfo::GetVersionInfo(str_file);
				//判断是否为空
				if (fvi->CompanyName==nullptr)
				{
					return "";
				}
				else
				{
					return fvi->CompanyName;
				}		
			}
			catch(Exception^ ex){ return "文件不存在！";}
		}

		//检测文件的出厂厂商名称，是否是微软的名称
		//str_:目标文件的出厂厂商名称
		//返回:true:为微软厂商名称，false:不是
		static bool common_checkFileCompanyName(String^ str_){
			if (str_->ToLower()->Contains("microsoft cor"))
			{
				return true;
			}

			return false;
		}

		//获得文件MD5
		static System::String^ common_getFileMD5(String^ str_) {
			//获得一个文件流继承stream
			FileStream^ fs = gcnew FileStream(str_,FileMode::Open);
			//创建MD5
			MD5^ md5 = MD5::Create();
			//计算流的MD5值
			array<byte>^ data = md5->ComputeHash(fs);
			//把字节数据转换成16进制
			StringBuilder^ sb = gcnew StringBuilder();
			for (int i = 0; i < data->Length; i++)
			{
				sb->Append(data[i].ToString("x2"));
			}
			//关掉流
			fs->Close();
			//返回
			return sb->ToString();
		}

		static System::String^ common_formatString_lv_sysmodule(String^ str_)
		{
			//首先把相关环境变量转换成本来值
			str_ = Environment::ExpandEnvironmentVariables(str_);
			//全部转换成小写
			str_ = str_->ToLower();

			//先去掉两个问号
			if (str_->Contains("\\??\\"))
			{
				//替换掉
				str_ = str_->Replace("\\??\\",""); 
			}

			//如果文件存在的话，就不作处理直接返回
			if (File::Exists(str_))
			{
				return str_;
			}

			//主要是sys文件
			if (File::Exists(Environment::SystemDirectory +"\\drivers\\"+ str_))
			{
				return Environment::SystemDirectory +"\\drivers\\"+ str_;
			}

			//主要是dll文件的路径的判断
			if (File::Exists(Environment::SystemDirectory +"\\"+ str_))
			{
				return Environment::SystemDirectory +"\\"+ str_;
			}

			if (str_->Contains("\\systemroot\\")) 
			{ 
				str_ = str_->Replace("\\systemroot\\system32",Environment::SystemDirectory); 
			} 

			//部分驱动返回的路径事宜system32开始，所以也要等效替换 
			if(str_->Contains("system32") && str_->IndexOf("system32")==0) 
			{ 
				str_ = str_->Replace("system32",Environment::SystemDirectory); 
			}
			
			return str_;			
		}

		static System::String^ common_formatString_lv_kernel(String^ str_)
		{
			//首先把相关环境变量转换成本来值
			str_ = Environment::ExpandEnvironmentVariables(str_);
			//全部转换成小写
			str_ = str_->ToLower();

			//先去掉两个问号
			if (str_->Contains("\\??\\"))
			{
				//替换掉
				str_ = str_->Replace("\\??\\",""); 
			}

			//如果文件存在的话，就不作处理直接返回
			if (File::Exists(str_))
			{
				return str_;
			}

			//如果文件存在的话，就不作处理直接返回
			if (File::Exists(Directory::GetDirectoryRoot(Environment::SystemDirectory) + "\\" + str_))
			{
				return Directory::GetDirectoryRoot(Environment::SystemDirectory) + "\\" + str_;
			}

			//主要是sys文件
			if (File::Exists(Environment::SystemDirectory +"\\drivers\\"+ str_))
			{
				return Environment::SystemDirectory +"\\drivers\\"+ str_;
			}
			
			return str_;
		}

		//格式化字符串
		static System::String^ common_formatString(String^ str_) 
		{ 
			//首先把相关环境变量转换成本来值
			str_ = Environment::ExpandEnvironmentVariables(str_);
			//全部转换成小写
			str_ = str_->ToLower();

			//先去掉两个问号
			if (str_->Contains("\\??\\"))
			{
				//替换掉
				str_ = str_->Replace("\\??\\",""); 
			}

			//如果文件存在的话，就不作处理直接返回
			if (File::Exists(str_))
			{
				return str_;
			}	

			if (str_->Contains("regsvr32"))
			{
				str_ = str_->Replace("regsvr32.exe","")->Replace("/u","")->Replace("/s","")->Replace("/n","");
				if (str_->Contains("/i:"))
				{
					str_ = str_->Split(gcnew array<String^>{"/i:"},StringSplitOptions::RemoveEmptyEntries)[1];
				}
				str_ = str_->Split(gcnew array<String^>{" "},StringSplitOptions::RemoveEmptyEntries)[1];
			}

			//主要是dll文件的路径的判断
			if (File::Exists(Environment::SystemDirectory +"\\"+ str_))
			{
				return Environment::SystemDirectory +"\\"+ str_;
			}

			//主要是sys文件
			if (File::Exists(Environment::SystemDirectory +"\\drivers\\"+ str_))
			{
				return Environment::SystemDirectory +"\\drivers\\"+ str_;
			}

			//Windows目录查找
			if (File::Exists(Environment::GetFolderPath(System::Environment::SpecialFolder::Windows) +"\\"+ str_))
			{
				return Environment::GetFolderPath(System::Environment::SpecialFolder::Windows) +"\\"+ str_;
			}

			if (str_->Contains("rundll32"))
			{
				if (str_->Contains(","))
				{
					str_ = str_->Split(gcnew array<String^>{","},StringSplitOptions::RemoveEmptyEntries)[0];
				}
				if (str_->IndexOf("rundll32")==0 && str_->Contains(".exe"))
				{
					str_ = str_->Replace("rundll32.exe","")->Replace(" ","");
				}		
				else
				{
					str_ = str_->Split(gcnew array<String^>{" "},StringSplitOptions::RemoveEmptyEntries)[1];
				}
			}

			//如果在这里还不是以.exe结尾的话，说明还带有参数
			if (str_->Contains(" ") && !str_->Contains("\"") && !str_->Contains("program files"))
			{
				str_ = str_->Split(' ')[0];
			}

			//过滤相关字符
			if (str_->Contains("\""))
			{
				str_ = str_->Split(gcnew array<String^>{"\" "},StringSplitOptions::RemoveEmptyEntries)[0];
				//替换掉
				str_ = str_->Replace("\"",""); 
			}

			if (str_->Contains(","))
			{
				str_ = str_->Split(gcnew array<String^>{","},StringSplitOptions::RemoveEmptyEntries)[0];
				//替换掉
				str_ = str_->Replace(",",""); 
			}

			if (str_->Contains("-"))
			{
				//在这里首先做好第一步处理
				str_ = str_->Split(gcnew array<String^>{"-"},StringSplitOptions::RemoveEmptyEntries)[0];
				//替换掉
				str_ = str_->Replace("-",""); 
			}

			if (str_->Contains("\\systemroot\\")) 
			{ 
				str_ = str_->Replace("\\systemroot\\system32",Environment::SystemDirectory); 
			} 

			//部分驱动返回的路径事宜system32开始，所以也要等效替换 
			if(str_->Contains("system32") && str_->IndexOf("system32")==0) 
			{ 
				str_ = str_->Replace("system32",Environment::SystemDirectory); 
			}  

			//处理svchost.exe -k
			if (str_->Contains("svchost"))
			{
				str_ = str_->Split(gcnew array<String^>{"svchost"},StringSplitOptions::RemoveEmptyEntries)[0]+"svchost.exe";
			}

			//返回处理后的字符串
			return str_;
		}

		//保存信息
		static System::Boolean common_changeServiceInformain(String^ str_serName,String^ str_display,ULONG ulStartMode,String^ str_desc) 
		{ 
			try
			{
				String^ str_path = "SYSTEM\\CurrentControlSet\\Services\\"+str_serName;    
				RegistryKey^ rk_ = Registry::LocalMachine->OpenSubKey(str_path, true);   

				rk_->SetValue("DisplayName",str_display,RegistryValueKind::String);
				rk_->SetValue("Start", ulStartMode,RegistryValueKind::DWord);  
				rk_->SetValue("Description",str_desc,RegistryValueKind::String);

				rk_->Close();

				//返回
				return true;
			}
			catch(Exception^ ex){ return false; }
		} 

		//改变服务的启动模式
		//2:自动启动,3:手动启动,4:禁用
		static System::Boolean common_changeServiceStartMode(String^ str_serName,ULONG ulStartMode) 
		{ 
			try
			{
				String^ str_path = "SYSTEM\\CurrentControlSet\\Services\\"+str_serName;    
				RegistryKey^ rk_ = Registry::LocalMachine->OpenSubKey(str_path, true);   
				rk_->SetValue("Start", ulStartMode,RegistryValueKind::DWord);  
				rk_->Close();

				//返回
				return true;
			}
			catch(Exception^ ex){ return false; }
		} 

		//更改用户密码
		static bool common_changeUserPassword(String^ str_name,String^ str_pass)
		{
			try 
			{ 
				DirectoryEntry^ de_1 = gcnew DirectoryEntry("WinNT://" + Environment::MachineName);
				//遍历目录，找到该用户
				DirectoryEntry^ de_2 = de_1->Children->Find(str_name,"user");
				//设置新密码
				de_2->Invoke("SetPassword", str_pass); 
				//提交保存
				de_2->CommitChanges(); 
				//关闭
				de_2->Close(); 
				de_1->Close(); 

				return true; 
			} 
            catch(Exception^ ex){ return false; }	
		}

		//判断指定rk_下是否存在str_子键
		static bool common_checkRegkey(RegistryKey^ rk_ ,String^ str_){
			try
			{
				for each(String^ str_sub in rk_->GetSubKeyNames())
				{
					if (str_sub->ToLower()==str_->ToLower())
					{
						//如果存在的话，返回true
						return true;
					}
				}
				//返回false
				return false; 
			}
			catch(Exception^ ex){ return false; }		
		}

		//判断指定rk_下是否存在str_值
		static bool common_checkRegkeyValue(RegistryKey^ rk_ ,String^ str_){
			try
			{
				for each(String^ str_sub in rk_->GetValueNames())
				{
					if (str_sub->ToLower()==str_->ToLower())
					{
						//如果存在的话，返回true
						return true;
					}
				}
				//返回false
				return false; 
			}
			catch(Exception^ ex){ return false; }		
		}

		//创建共享
		//ShareResourceType
		//DiskDrive = 0x0
		//PrintQueue = 0x1
		//Device = 0x2
		//IPC = 0x3
		//DiskDrive Admin = 0x80000000
		//PrintQueue Admin = 0x80000001
		//Device Admin = 0x80000002
		//IPC Admin = 0x80000003
		//返回错误消息
		static System::String^ common_createShare(String^ str_name,String^ str_path,int uType,Object^ uMaximumAllowed,String^ str_desc)
		{
			String^ str_msg = String::Empty;

			try
			{
				ManagementClass^ mc_ = gcnew ManagementClass("win32_share");
				//Create方法参数
				ManagementBaseObject^ mbo_ = mc_->GetMethodParameters("Create");
				//共享名称
				mbo_["Name"] = str_name;
				//路径
				mbo_["Path"] = str_path;
				//类型
				mbo_["Type"] = uType;
				//nullptr用户数连接无限制
				mbo_["MaximumAllowed"] = uMaximumAllowed; 
				//描述
				mbo_["Description"] = str_desc;
				//密码
				mbo_["Password"] = nullptr;
				//nullptr使Everyone拥有完全控制权限
				mbo_["Access"] = nullptr; 

				//获得返回结果
				ManagementBaseObject^ mbo_2 = mc_->InvokeMethod("Create", mbo_, nullptr);
				int uRet = Convert::ToInt32(mbo_2->Properties["ReturnValue"]->Value);

				switch (uRet)
				{
				case 0: //Success
					str_msg = "Success";
					break;
				case 2: //Access denied 
					str_msg = "Access denied ";
					break;
				case 8: //Unknown failure 
					str_msg = "Unknown failure";
					break;
				case 9: //Invalid name 
					str_msg = "Invalid name ";
					break;
				case 10: //Invalid level 
					str_msg = "Invalid level ";
					break;
				case 21: //Invalid parameter 
					str_msg = "Invalid parameter ";
					break;
				case 22: //Duplicate share
					str_msg = "Invalid parameter ";
					break;
				case 23: //Redirected path 
					str_msg = "Redirected path ";
					break;
				case 24: //Unknown device or directory 
					str_msg = "Unknown device or directory ";
					break;
				case 25: //Net name not found 
					str_msg = "Net name not found ";
					break;
				default:
					break;
				}
			}
			catch(Exception^ ex){ str_msg = ex->Message; }

			return str_msg;
		}

		//创建用户
		static bool common_createUser(String^ str_name,String^ str_pass,String^ str_path,String^ str_group,String^ str_desc)
		{
			try 
			{ 
				DirectoryEntry^ de_1 = gcnew DirectoryEntry("WinNT://" + Environment::MachineName);
				//增加一个用户名目录
				DirectoryEntry^ de_2 = de_1->Children->Add(str_name, "user");
				//设置相关属性
				//密码
				de_2->Invoke("SetPassword", str_pass);
				//描述		
				de_2->Invoke("Put", "Description",str_desc);
				//密码永不过期 
				de_2->Invoke("Put","UserFlags",UF_DONT_EXPIRE_PASSWD);
				//用户主目录
				de_2->Invoke("Put","HomeDirectory",str_path);
				//提交修改
				de_2->CommitChanges();

				//添加组
				if (!String::IsNullOrEmpty(str_group))
				{
					DirectoryEntry^ de_3 = de_1->Children->Find(str_group, "group");
					de_3->Invoke("Add",de_2->Path->ToString());
					de_3->Close();
				}

				de_2->Close();
				de_1->Close();

				return true; 
			} 
			catch(Exception^ ex){ MessageBox::Show(ex->Message);return false; }
		}

		//删除用户
		static bool common_delUser(String^ str_name)
		{
			try 
			{ 
				DirectoryEntry^ de_1 = gcnew DirectoryEntry("WinNT://" + Environment::MachineName);
				//遍历目录，找到该用户
				DirectoryEntry^ de_2 = de_1->Children->Find(str_name,"user");
				//从目录表中移除
				de_1->Children->Remove(de_2); 

				de_1->Close();
				//返回成功
				return true; 
			} 
			catch(Exception^ ex){ return false; }		
		}

		//禁用和启用用户
		static bool common_disableUser(String^ str_name, bool isDisable)
		{
			try 
			{ 
				DirectoryEntry^ de_1 = gcnew DirectoryEntry("WinNT://" + Environment::MachineName);
				//遍历目录，找到该用户
				DirectoryEntry^ de_2 = de_1->Children->Find(str_name,"user");
				//从目录表中移除
				de_2->InvokeSet("AccountDisabled", isDisable);
				de_2->CommitChanges();

				de_2->Close();
				de_1->Close();
				//返回成功
				return true; 
			} 
			catch(Exception^ ex){ return false; }	
		}

		//拖放保存
		//返回路径
		static System::String^ common_dragDropToSave(ListView^ lv_){
			try
			{
				String^ str_path = Environment::GetEnvironmentVariable("temp") + "\\" + DateTime::Now.ToString()->Replace("-","_")->Replace(" ","_")->Replace(":","_") + ".txt";

				//
				StreamWriter^ sw = gcnew StreamWriter(str_path);		

				//开始写入列标题
				ListView::ColumnHeaderCollection^ chc_ = lv_->Columns;

				for each(ColumnHeader^ ch_ in chc_)
				{
					sw->Write(ch_->Text+"\t");
				}

				//换行
				sw->Write("\r\n");

				//这里写入数据
				for each(ListViewItem^ item in lv_->Items)
				{
					for (int i=0;i<chc_->Count;i++)
					{
						try
						{
							sw->Write(item->SubItems[i]->Text+"\t");
						}
						catch(Exception^ ex){ }			
					}

					//换行
					sw->Write("\r\n");
				}

				//最后关闭流
				sw->Close();

				//返回路径
				return str_path;
			}
			catch(Exception^ ex){  return nullptr; }
		}

		//获得IP物理地址
		static System::String^ common_getIPAddr(String^ str_) {
			try
			{
				//web客户端
				WebClient^ webclient = gcnew WebClient();
				//下载数据
				str_ = webclient->DownloadString("http://www.ip138.com/ips8.asp?ip="+str_+"&action=2");
				//正则
				String^ str_regex = "<li>.*</li>";
				//忽略大小写
				Regex^ regex = gcnew Regex(str_regex, RegexOptions::IgnoreCase);
				//开始匹配
				MatchCollection^ matchCollection = regex->Matches(str_);
				//获得匹配字符串
				for (int i = 0; i < matchCollection->Count; i++)
				{
					return matchCollection[i]->Value->Split(gcnew array<String^>{"</li><li>"},StringSplitOptions::RemoveEmptyEntries)[0]->Replace("<li>","")->Replace("本站主数据：","");
				}

				return nullptr;
			}
			catch(Exception^ ex){ return nullptr; }
		}

		//导出ListView列中的数据
		static System::Void common_getListViewData(ListView^ lv_) 
		{ 
			//实例化一个文件保存对话框
			SaveFileDialog^ SFD = gcnew SaveFileDialog();	
			SFD->Filter = "txt files(*.txt)|*.txt|all files(*.*)|*.*";

			//如果确认打开对话框
			if (SFD->ShowDialog()==System::Windows::Forms::DialogResult::OK)
			{
				try
				{
					StreamWriter^ sw = gcnew StreamWriter(SFD->FileName);		

					//开始写入列标题
					ListView::ColumnHeaderCollection^ chc_ = lv_->Columns;

					for each(ColumnHeader^ ch_ in chc_)
					{
						sw->Write(ch_->Text+"\t");
					}
					//换行
					sw->Write("\r\n");
					//这里写入数据
					for each(ListViewItem^ item in lv_->Items)
					{
						for (int i=0;i<chc_->Count;i++)
						{
							try
							{
								sw->Write(item->SubItems[i]->Text+"\t");
							}
							catch(Exception^ ex){}			
						}
						//换行
						sw->Write("\r\n");
					}
					//最后关闭流
					sw->Close();
				}
				catch(Exception^ ex){ }
			}	
		}

		//获得服务动态链接库文件名称 
		static System::String^ common_getServiceDllName(String^ str_serName) 
		{ 
			try 
			{ 
				//从注册表中得到动态链接库完整路径 
				String^ str_ = common_getServiceInformain(str_serName,"PathName")->ToString()->ToLower();
				if (str_->Contains("svchost") && str_->Contains("-k")) 
				{ 
					RegistryKey^ regKey=Registry::LocalMachine->OpenSubKey("SYSTEM\\CurrentControlSet\\Services\\"+str_serName); 
					regKey=regKey->OpenSubKey("Parameters"); 
					regKey->GetValue("ServiceDll"); 
					return regKey->GetValue("ServiceDll")->ToString(); 
				} 

				return nullptr;
			} 
			catch(Exception^ ex) { return nullptr; } 
		}

		//获取服务的相关信息，因为NET组件提供的ServiceController类不能获得具体的信息 
		static System::Object^ common_getServiceInformain(String^ str_serName,String^ str_field) 
		{ 
			String^ str_path="Win32_Service.Name='"+str_serName+"'"; 
			ManagementPath^ mp=gcnew ManagementPath(str_path); 
			ManagementObject^ mo=gcnew ManagementObject(mp); 
			return mo[str_field]; 
		} 


};
}

#endif //_COMMON_NET_