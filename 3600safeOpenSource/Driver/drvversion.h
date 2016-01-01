///////////////////////////////////////////////////////////////////////////////
///
/// Copyright (c) 2010 - <company name here>
///
/// Defines for the version information in the resource file
///
/// (File was in the PUBLIC DOMAIN  - Created by: ddkwizard\.assarbad\.net)
///////////////////////////////////////////////////////////////////////////////

// $Id$

#ifndef __DRVVERSION_H_VERSION__
#define __DRVVERSION_H_VERSION__ 100

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif


// ---------------------------------------------------------------------------
// Several defines have to be given before including this file. These are:
// ---------------------------------------------------------------------------
#define TEXT_AUTHOR            www.3600safe.com // author (optional value)
#define PRD_MAJVER             0 // major product version
#define PRD_MINVER             2 // minor product version
#define PRD_BUILD              0 // build number for product
#define FILE_MAJVER            0 // major file version
#define FILE_MINVER            2 // minor file version
#define FILE_BUILD             0 // build file number
#define DRV_YEAR               2012 // current year or timespan (e.g. 2003-2009)
#define TEXT_WEBSITE           www.3600safe.com // website
#define TEXT_PRODUCTNAME       A-Protect // product's name
#define TEXT_FILEDESC          A-Protect Anti-Rootkit Kernel Module // component description
#define TEXT_COMPANY           www.3600safe.com // company
#define TEXT_MODULE            A-Protect // module name
#define TEXT_COPYRIGHT         Copyright \xA9 DRV_YEAR TEXT_COMPANY // copyright information
// #define TEXT_SPECIALBUILD      // optional comment for special builds
#define TEXT_INTERNALNAME      A-Protect.sys // copyright information
// #define TEXT_COMMENTS          // optional comments
// ---------------------------------------------------------------------------
// ... well, that's it. Pretty self-explanatory ;)
// ---------------------------------------------------------------------------

#endif // __DRVVERSION_H_VERSION__
