/*********************************************************
   DllData file -- partially generated by MIDL compiler 

  we are building the .idls over in published but we want the actual proxy stub to live in
  msvidctl.dll *not* quartz.dll 
  this is because registering the typelibs wipes out the proxy stub registration and
  we want to avoid any registration order dependencies.  so , we must register the proxy/stubs in 
  the vidctl after the typelib registration and it wouldn't be good to do it in both places and
  make quartz.dll unecessarily larger.

  consequently, we are manually maintaining this file.  if you add a new .idl to the vidctl you
  must update these entries

  for simplicity sake since we know we're always merging the proxy stub we're combining dlldata.c and
  dlldatax.c as well.

*********************************************************/


#define REGISTER_PROXY_DLL //DllRegisterServer, etc.

#define USE_STUBLESS_PROXY	//defined only with MIDL switch /Oicf

#pragma comment(lib, "rpcndr.lib")
#pragma comment(lib, "rpcns4.lib")
#pragma comment(lib, "rpcrt4.lib")

#define DllMain				PrxDllMain
#define DllRegisterServer	PrxDllRegisterServer
#define DllUnregisterServer PrxDllUnregisterServer
#define DllGetClassObject   PrxDllGetClassObject
#define DllCanUnloadNow     PrxDllCanUnloadNow


//#include "dlldata.c" as follows...

// wrapper for dlldata.c
#define PROXY_DELEGATION

#include <rpcproxy.h>

#ifdef __cplusplus
extern "C"   {
#endif

#define USE_STUBLESS_PROXY

EXTERN_PROXY_FILE( regbag )
EXTERN_PROXY_FILE( tuner )
#ifndef TUNING_MODEL_ONLY
    EXTERN_PROXY_FILE( segment )
    EXTERN_PROXY_FILE( msvidctl )
#endif


PROXYFILE_LIST_START
    REFERENCE_PROXY_FILE( regbag ),
    REFERENCE_PROXY_FILE( tuner ),
#ifndef TUNING_MODEL_ONLY
        REFERENCE_PROXY_FILE( segment ),
        REFERENCE_PROXY_FILE( msvidctl ),
#endif
/* End of list */
PROXYFILE_LIST_END


DLLDATA_ROUTINES( aProxyFileList, GET_DLL_CLSID )

#ifdef __cplusplus
}  /*extern "C" */
#endif

/* end of generated dlldatax.c file */

