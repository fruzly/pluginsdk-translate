﻿/*****************************************************************************

Copyright (C) 2024 Foxit Corporation
All rights reserved.

NOTICE: Foxit permits you to use, modify, and distribute this file
in accordance with the terms of the Foxit license agreement
accompanying it. If you have received this file from a source other
than Foxit, then your use, modification, or distribution of it
 requires the prior written permission of Foxit.
 
*****************************************************************************/

#ifndef FOFD_BASICCALLS_H
#define FOFD_BASICCALLS_H

#ifndef FS_COMMON_H
#include "../basic/fs_common.h"
#endif

#ifndef FS_BASICEXPT_H
#include "../basic/fs_basicExpT.h"
#endif

#ifndef FS_STRINGEXPT_H
#include "../basic/fs_stringExpT.h"
#endif

#ifndef FOFD_BASICEXPT_H
#include "fofd_basicExpT.h"
#endif

#ifdef __cplusplus
extern "C"{
#endif

#undef INTERFACE
#undef BEGINENUM
#undef NumOfSelector
#undef ENDENUM

// Define macros to generate function selectors in the file fofd_basicImpl.h
#define BEGINENUM enum{
#define NumOfSelector(name) name##InterfacesNum
#define ENDENUM };
#define INTERFACE(returnType,interfaceName,params) interfaceName##SEL,
#include "fofd_basicTempl.h"

#undef INTERFACE
#undef BEGINENUM
#undef NumOfSelector
#undef ENDENUM

// Define macros to generate function PROTO in the file fofd_basicImpl.h
#define BEGINENUM
#define NumOfSelector(name)
#define ENDENUM
#define INTERFACE(returnType,interfaceName, params) \
typedef returnType (*interfaceName##SELPROTO)params;
#include "fofd_basicTempl.h"

//----------_V1----------
//----------_V2----------
//----------_V3----------
//----------_V4----------
//----------_V5----------
//----------_V6----------
//----------_V7----------
//----------_V8----------
//----------_V9----------
//----------_V10----------
//*****************************
/* CryptoDict HFT functions */
//*****************************

//*****************************
/* SecurityHandler HFT functions */
//*****************************

//*****************************
/* StdSecurityHandler HFT functions */
//*****************************

//*****************************
/* StdCertSecurityHandler HFT functions */
//*****************************

//*****************************
/* SMSecurityHandler HFT functions */
//*****************************

//*****************************
/* CryptoHandler HFT functions */
//*****************************

//*****************************
/* StdCryptoHandler HFT functions */
//*****************************

//*****************************
/* SM4CryptoHandler HFT functions */
//*****************************

//*****************************
/* FileStream HFT functions */
//*****************************

//*****************************
/* PauseHandler HFT functions */
//*****************************

#define FOFDPauseHandlerCreate (FS_ASSERT(_gpCoreHFTMgr->FRGetSDKVersion() >= SDK_VERSION),(*(FOFDPauseHandlerCreateSELPROTO)FRCOREROUTINE(FOFDPauseHandlerSEL,FOFDPauseHandlerCreateSEL, _gPID)))

#define FOFDPauseHandlerDestroy (FS_ASSERT(_gpCoreHFTMgr->FRGetSDKVersion() >= SDK_VERSION),(*(FOFDPauseHandlerDestroySELPROTO)FRCOREROUTINE(FOFDPauseHandlerSEL,FOFDPauseHandlerDestroySEL, _gPID)))

//*****************************
/* UIMgr HFT functions */
//*****************************

//*****************************
/* DIBAttribute HFT functions */
//*****************************

//*****************************
/* CodeC HFT functions */
//*****************************

//*****************************
/* PrintSetting HFT functions */
//*****************************

//*****************************
/* Sys HFT functions */
//*****************************

//----------_V11----------
//----------_V12----------
//----------_V13----------
//----------_V14----------
//*****************************
/* Fontmapper HFT functions */
//*****************************

//----------_V15----------
//----------_V16----------
//----------_V17----------
//----------_V18----------
//----------_V19----------
//----------_V20----------
#ifdef __cplusplus
}
#endif

#endif