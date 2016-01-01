//************************************************************************************
//*
//*          A盾电脑防护 http://www.3600safe.com/
//*        
//*说明：
//*          终于有点魄力开源了。谨以此薄码献给天下的母亲们 2012-5-13。
//*
//*AD下：
//*          www.3600safe.com 是一个内核安全编程技术网站，收集了各种神牛的技术文章，
//*          搞内核编程的不妨把它当成一个资料库 ：）
//*          
//************************************************************************************

文件列表：

I:.
│  3600safe.sln
│  readme.txt
│
├─3600safe
│  │  3600safe.aps
│  │  3600safe.cpp
│  │  3600safe.h
│  │  3600safe.rc
│  │  3600safe.vcxproj
│  │  3600safe.vcxproj.filters
│  │  3600safe.vcxproj.user
│  │  3600safeDoc.cpp
│  │  3600safeDoc.h
│  │  3600safeView.cpp
│  │  3600safeView.h
│  │  AboutDlg.cpp
│  │  AboutDlg.h
│  │  Atapi.cpp
│  │  Atapi.h
│  │  C3600Splash.cpp
│  │  C3600Splash.h
│  │  ClrListCtrl.cpp
│  │  ClrListCtrl.h
│  │  DLLModule.cpp
│  │  DLLModule.h
│  │  DpcTimer.cpp
│  │  DpcTimer.h
│  │  FilterDriver.cpp
│  │  FilterDriver.h
│  │  FsdHook.cpp
│  │  FsdHook.h
│  │  HipsLog.cpp
│  │  HipsLog.h
│  │  Install.cpp
│  │  Install.h
│  │  Kbdclass.cpp
│  │  Kbdclass.h
│  │  KernelHook.cpp
│  │  KernelHook.h
│  │  KernelModule.cpp
│  │  KernelModule.h
│  │  KernelThread.cpp
│  │  KernelThread.h
│  │  MainFrm.cpp
│  │  MainFrm.h
│  │  Md5.cpp
│  │  Md5.h
│  │  Mouclass.cpp
│  │  Mouclass.h
│  │  Nsiproxy.cpp
│  │  Nsiproxy.h
│  │  ntdll.lib
│  │  ObjectHook.cpp
│  │  ObjectHook.h
│  │  Process.cpp
│  │  Process.h
│  │  ProcessHandle.cpp
│  │  ProcessHandle.h
│  │  ProcessThread.cpp
│  │  ProcessThread.h
│  │  ReadMe.txt
│  │  resource.h
│  │  Services.cpp
│  │  Services.h
│  │  ShadowSSDT.cpp
│  │  ShadowSSDT.h
│  │  SSDT.cpp
│  │  SSDT.h
│  │  stdafx.cpp
│  │  stdafx.h
│  │  SubModule.cpp
│  │  SubModule.h
│  │  SystemNotify.cpp
│  │  SystemNotify.h
│  │  SystemThread.cpp
│  │  SystemThread.h
│  │  targetver.h
│  │  Tcpip.cpp
│  │  Tcpip.h
│  │  TcpView.cpp
│  │  TcpView.h
│  │  uninstall360.cpp
│  │  uninstall360.h
│  │  UnloadDllModule.h
│  │  UserImages.bmp
│  │  Windows2003SP1_CN.h
│  │  Windows2003SP2_CN.h
│  │  Windows7Home_CN.h
│  │  Windows7SP1_CN.h
│  │  WindowsXPSP2_CN.h
│  │  WindowsXPSP3_CN.h
│  │
│  ├─Release
│  └─res
│          3600safe - 副本.ico
│          3600safe.bmp
│          3600safe.ico
│          Dispatch.ico
│          GDriver.ico
│          Hips.ico
│          KernelHook.ico
│          KernelModule.ico
│          KernelThread.ico
│          My3600safe.rc2
│          Process.ico
│          ring3.ico
│          Services.ico
│          Tcpview.ico
│          Toolbar.bmp
│          Toolbar256.bmp
│
├─bin
│  │  A-Protect.exe
│  │  A-Protect.txt
│  │
│  └─A-ProtectSDK
│      │  readme.txt
│      │
│      ├─example
│      │      A-ProtectConsole.cpp
│      │      A-ProtectConsole.h
│      │      A-ProtectConsole.sln
│      │      A-ProtectConsole.vcxproj
│      │      Install.cpp
│      │      Install.h
│      │      KernelModule.h
│      │      ntdll.lib
│      │
│      └─include
│              A-ProtectSDK.h
│
└─Driver
    │  AntiInlineHook.c
    │  AntiInlineHook.h
    │  Atapi.c
    │  Atapi.h
    │  buildchk_win7_x86.log
    │  buildchk_win7_x86.wrn
    │  Common.h
    │  Control.c
    │  Control.h
    │  DeleteFile.c
    │  DeleteFile.h
    │  DpcTimer.c
    │  DpcTimer.h
    │  DriverHips.c
    │  DriverHips.h
    │  drvcommon.h
    │  drvversion.h
    │  drvversion.rc
    │  dump.c
    │  dump.h
    │  file.c
    │  file.h
    │  FileSystem.c
    │  FileSystem.h
    │  Fixrelocation.c
    │  Fixrelocation.h
    │  Function.c
    │  Function.h
    │  InitWindowsVersion.c
    │  InitWindowsVersion.h
    │  InlineHook.c
    │  InlineHook.h
    │  kbdclass.c
    │  kbdclass.h
    │  KernelFilterDriver.c
    │  KernelFilterDriver.h
    │  KernelHookCheck.c
    │  KernelHookCheck.h
    │  KernelReload.c
    │  KernelReload.h
    │  KernelThread.c
    │  KernelThread.h
    │  KillProcess.c
    │  KillProcess.h
    │  ldasm.c
    │  ldasm.h
    │  libdasm.c
    │  libdasm.h
    │  makefile
    │  Mouclass.c
    │  Mouclass.h
    │  NetworkDefense.c
    │  NetworkDefense.h
    │  nsiproxy.c
    │  nsiproxy.h
    │  Ntfs.c
    │  Ntfs.h
    │  ntifs.h
    │  ntos.c
    │  ntos.h
    │  ObjectHookCheck.c
    │  ObjectHookCheck.h
    │  Port.c
    │  Port.h
    │  Process.c
    │  Process.h
    │  ProcessModule.c
    │  ProcessModule.h
    │  Protect.c
    │  Protect.h
    │  ReLoadSSDTTableHook.c
    │  ReLoadSSDTTableHook.h
    │  SafeSystem.c
    │  SafeSystem.h
    │  SafeSystem.sln
    │  SafeSystem.vcxproj
    │  SafeSystem.vcxproj.filters
    │  SafeSystem.vcxproj.user
    │  SDTShadowRestore.h
    │  Services.c
    │  Services.h
    │  ShadowSSDT.c
    │  ShadowSSDT.h
    │  sources
    │  SSDT.c
    │  SSDT.h
    │  SysModule.c
    │  SysModule.h
    │  SystemNotify.c
    │  SystemNotify.h
    │  SystemThread.c
    │  SystemThread.h
    │  tables.h
    │  Tcpip.c
    │  Tcpip.h
    │  win32k.c
    │  win32k.h
    │
    ├─objchk_win7_x86
    │  └─i386
    │          a.bat
    │          bin2c.exe
    │
    └─Release

//********************************************************************
//end
//********************************************************************