#ifndef METHODS_CALLS_H
#define METHODS_CALLS_H

#undef EXTENSIONMETHODS
#undef BEGINENUM
#undef NumOfSelector
#undef ENDENUM

/*Generate the method selector.*/
#define BEGINENUM enum{
#define EXTENSIONMETHODS(returnType, methodName, params) methodName##SEL,
#define NumOfSelector(name) name##MethodsNum
#define ENDENUM };
#include "methodsTmpl.h"

#undef EXTENSIONMETHODS
#undef BEGINENUM
#undef NumOfSelector
#undef ENDENUM

/*Generate the prototypes of the methods.*/
#define BEGINENUM
#define NumOfSelector(name)
#define ENDENUM
#define EXTENSIONMETHODS(returnType, methodName, params) \
typedef returnType (*methodName##PROTO)params;
#include "methodsTmpl.h"

/*The extension HFT name.*/
#define EXTENSION_HFT_NAME "ExtensionHFT"

/*The extension HFT version.*/
#define EXTENSION_HFT_VERSION 1

extern FS_HFT extensionHFT;

/*Define the extension methods as macros.*/
#define ExtensionHFTFunction1 (*(Function1PROTO)(FSExtensionHFTMgrGetEntry(extensionHFT, Function1SEL)))
#define ExtensionHFTFunction2 (*(Function2PROTO)(FSExtensionHFTMgrGetEntry(extensionHFT, Function2SEL)))

#endif
