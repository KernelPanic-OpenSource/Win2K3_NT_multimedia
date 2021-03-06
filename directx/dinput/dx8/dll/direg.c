/*****************************************************************************
 *
 *  DIReg.c
 *
 *  Copyright (c) 1996 Microsoft Corporation.  All Rights Reserved.
 *
 *  Abstract:
 *
 *      OLE self-registration.
 *
 *  Contents:
 *
 *      DllRegisterServer()
 *      DllUnregisterServer()
 *
 *****************************************************************************/

#include "dinputpr.h"

/*****************************************************************************
 *
 *  The sqiffle for this file.
 *
 *****************************************************************************/

#define sqfl sqflDll

/*****************************************************************************
 *
 *      RegSetStringEx
 *
 *      Add a REG_SZ to hkey\sub::value.
 *
 *****************************************************************************/

void INTERNAL
RegSetStringEx(HKEY hk, LPCTSTR ptszValue, LPCTSTR ptszData)
{
    LONG lRc = RegSetValueEx(hk, ptszValue, 0, REG_SZ,
                             (PV)ptszData, cbCtch(lstrlen(ptszData)+1));
}

/*****************************************************************************
 *
 *      RegDelStringEx
 *
 *      Remove a REG_SZ from hkey\sub::value.  The data is ignored.
 *      It's passed so that RegDelStringEx matches the prototype for a
 *      REGSTRINGACTION.
 *
 *****************************************************************************/

void INTERNAL
RegDelStringEx(HKEY hk, LPCTSTR ptszValue, LPCTSTR ptszData)
{
    LONG lRc = RegDeleteValue(hk, ptszValue);
}

/*****************************************************************************
 *
 *      RegCloseFinish
 *
 *      Just close the subkey already.
 *
 *****************************************************************************/

void INTERNAL
RegCloseFinish(HKEY hk, LPCTSTR ptszSub, HKEY hkSub)
{
    LONG lRc = RegCloseKey(hkSub);
}

/*****************************************************************************
 *
 *      RegDelFinish
 *
 *      Delete a key if there is nothing in it.
 *
 *      OLE unregistration rules demand that you not delete a key if OLE
 *      has added something to it.
 *
 *****************************************************************************/

void INTERNAL
RegDelFinish(HKEY hk, LPCTSTR ptszSub, HKEY hkSub)
{
    LONG lRc;
    DWORD cKeys = 0, cValues = 0;
    RegQueryInfoKey(hkSub, 0, 0, 0, &cKeys, 0, 0, &cValues, 0, 0, 0, 0);
    RegCloseKey(hkSub);
    if ((cKeys | cValues) == 0) {

#ifdef WINNT
        lRc = DIWinnt_RegDeleteKey(hk, ptszSub);
#else
        lRc = RegDeleteKey(hk, ptszSub);
#endif

    } else {
        lRc = 0;
    }
}


#ifdef WINNT //The following are only used on WINNT

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   void | RegSetPermissionsOnDescendants |
 *
 *			Sets the specified permissions on all descendants of the specified key.
 *
 *  @parm   HKEY | hKey |
 *
 *          The reg key on whose descendants we're operating.
 *
 *  @parm   SECURITY_DESCRIPTOR* | psd |
 *
 *          Ptr to the SECURITY_DESCRIPTOR we're using.
 *
 *  @returns
 *
 *          Nothing.
 *			Note that this recurses while having TCHAR szKeyName[MAX_PATH+1]
 *			for each level. If stack space is a concern, can allocate it on the heap,
 *			and free after obtain the HKEY (i.e. before recursing).
 *
 *****************************************************************************/

void INTERNAL
RegSetPermissionsOnDescendants(HKEY hKey, SECURITY_DESCRIPTOR* psd)
{
	DWORD dwIndex = 0;
	LONG lRetCode = ERROR_SUCCESS;

	while (lRetCode == ERROR_SUCCESS)
	{
		TCHAR szKeyName[MAX_PATH+1];
		DWORD cbKeyName = MAX_PATH+1;
		lRetCode = RegEnumKeyEx(hKey, 
							dwIndex,
							szKeyName,
							&cbKeyName,
							NULL, NULL, NULL, NULL);

		if (lRetCode == ERROR_SUCCESS)
		{
			LONG lRetSub;
			HKEY hkSubKey;
			lRetSub = RegOpenKeyEx(hKey, szKeyName, 0, DI_KEY_ALL_ACCESS | WRITE_DAC, &hkSubKey);
			if (lRetSub == ERROR_SUCCESS)
			{
				//set security on it and its descendants
				lRetSub = RegSetKeySecurity(hkSubKey,
											(SECURITY_INFORMATION)DACL_SECURITY_INFORMATION,
											psd);
				RegSetPermissionsOnDescendants(hkSubKey, psd);
				RegCloseKey(hkSubKey);
					
				if(lRetSub != ERROR_SUCCESS) 
				{
					RPF("Couldn't RegSetKeySecurity on %hs", szKeyName);
				}
			}
			else
			{
				RPF("Couldn't open enumed subkey %hs", szKeyName);
			}

			dwIndex++;
		}
	}

}


/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | RegSetSecurity |
 *
 *      Set the security of 
 *        SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\Joystick\\OEM
 *        SYSTEM\\CurrentControlSet\\Control\\MediaResources\\Joystick\\Dinput.dll
 *      to be accessible to Everyone on Win2K,
 *.		to be accessible to Everyone but without WRITE_DAC and WRITE_OWNER permissions on WinXP;
 *        SYSTEM\\CurrentControlSet\\Control\\MediaProperties\\PrivateProperties\\DirectInput
 *		to be accessible to Everyone but without WRITE_DAC and WRITE_OWNER permissions on Win2k and WinXP.
 *
 *  @returns
 *
 *          S_OK on success, E_FAIL on error.
 *
 *****************************************************************************/

HRESULT INTERNAL
RegSetSecurity(void)
{
    HKEY hkJoy, hkDin, hkPP, hkMedR, hkJDi;
    LONG lRetCode;
	
    // Changed for server per Whistler bug 575181
    // open / create the keys
    //
	//MediaProperties/PrivateProperties/DirectInput
	lRetCode = RegOpenKeyEx(
        HKEY_LOCAL_MACHINE,
        REGSTR_PATH_PRIVATEPROPERTIES,
        0,
        KEY_WRITE,
        &hkPP
        );

	if(lRetCode != ERROR_SUCCESS) {
		RPF("Couldn't open REGSTR_PATH_PRIVATEPROPERTIES");
        return E_FAIL;
    }
    
	lRetCode = RegCreateKeyEx(
        hkPP,
        TEXT("DirectInput"),
        0,
		NULL,
		REG_OPTION_NON_VOLATILE,
        DI_KEY_ALL_ACCESS,
		NULL,
        &hkDin,
		NULL
        );

	RegCloseKey(hkPP);

    if(lRetCode != ERROR_SUCCESS) {
		RPF("Couldn't open DirectInput");
        return E_FAIL;
    }

	RegCloseKey(hkDin);

	//MediaResources/Joystick/Dinput.dll
	lRetCode = RegOpenKeyEx(
		HKEY_LOCAL_MACHINE,
		REGSTR_PATH_MEDIARESOURCES,
		0,
		KEY_WRITE,
		&hkMedR
		);

	if(lRetCode != ERROR_SUCCESS) {
		RPF("Couldn't open REGSTR_PATH_MEDIARESOURCES");
		return E_FAIL;
	}

	   
	lRetCode = RegCreateKeyEx(
		hkMedR,
		TEXT("Joystick"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_WRITE,
		NULL,
		&hkJoy,
		NULL
        );

	RegCloseKey(hkMedR);

	if(lRetCode != ERROR_SUCCESS) {
		RPF("Couldn't open Joystick");
		return E_FAIL;
	}

	lRetCode = RegCreateKeyEx(
		hkJoy,
		TEXT("Dinput.dll"),
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		DI_KEY_ALL_ACCESS,
		NULL,
		&hkJDi,
		NULL
		);

	RegCloseKey(hkJoy);
    
	if(lRetCode != ERROR_SUCCESS) {
		RPF("Couldn't open Dinput.dll");
		return E_FAIL;
	}

	RegCloseKey(hkJDi);

    return S_OK;
}


/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   HRESULT | DummyRegSetSecurity |
 *
 *			Do nothing
 *
 *  @returns
 *
 *          S_OK.
 *
 *****************************************************************************/

HRESULT INTERNAL
DummyRegSetSecurity(void)
{
	return S_OK;
}

#endif //WINNT


/*****************************************************************************
 *
 *      REGVTBL
 *
 *      Functions for dorking with a registry key, either coming or going.
 *
 *****************************************************************************/

typedef struct REGVTBL {
    /* How to create/open a key */
    LONG (INTERNAL *KeyAction)(HKEY hk, LPCTSTR ptszSub, PHKEY phkOut);

    /* How to create/delete a string */
    void (INTERNAL *StringAction)(HKEY hk, LPCTSTR ptszValue, LPCTSTR ptszData);

    /* How to finish using a key */
    void (INTERNAL *KeyFinish)(HKEY hk, LPCTSTR ptszSub, HKEY hkSub);
 
#ifdef WINNT
    /* How to set security on OEM key */
    HRESULT (INTERNAL *SetSecurity)( void );
#endif //WINNT

} REGVTBL, *PREGVTBL;
typedef const REGVTBL *PCREGVTBL;

#ifdef WINNT
const REGVTBL c_vtblAdd = { RegCreateKey, RegSetStringEx, RegCloseFinish, RegSetSecurity };
const REGVTBL c_vtblDel = {   RegOpenKey, RegDelStringEx,   RegDelFinish, DummyRegSetSecurity };
#else
const REGVTBL c_vtblAdd = { RegCreateKey, RegSetStringEx, RegCloseFinish };
const REGVTBL c_vtblDel = {   RegOpenKey, RegDelStringEx,   RegDelFinish };
#endif //WINNT

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   void | DllServerAction |
 *
 *          Register or unregister our objects with OLE/COM/ActiveX/
 *          whatever its name is.
 *
 *****************************************************************************/

#pragma BEGIN_CONST_DATA

extern const TCHAR c_tszNil[];

#define ctchClsid       ctchGuid

const TCHAR c_tszClsidGuid[] =
TEXT("CLSID\\{%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}");

const TCHAR c_tszInProcServer32[] = TEXT("InProcServer32");
const TCHAR c_tszThreadingModel[] = TEXT("ThreadingModel");
const TCHAR c_tszBoth[] = TEXT("Both");

#pragma END_CONST_DATA

void INTERNAL
DllServerAction(PCREGVTBL pvtbl)
{
    TCHAR tszThisDll[MAX_PATH];
    UINT iclsidmap;

    GetModuleFileName(g_hinst, tszThisDll, cA(tszThisDll));

    for (iclsidmap = 0; iclsidmap < cclsidmap; iclsidmap++) {
        TCHAR tszClsid[7+ctchClsid];
        HKEY hkClsid;
        HKEY hkSub;
        REFCLSID rclsid = c_rgclsidmap[iclsidmap].rclsid;

        wsprintf(tszClsid, c_tszClsidGuid,
                 rclsid->Data1, rclsid->Data2, rclsid->Data3,
                 rclsid->Data4[0], rclsid->Data4[1],
                 rclsid->Data4[2], rclsid->Data4[3],
                 rclsid->Data4[4], rclsid->Data4[5],
                 rclsid->Data4[6], rclsid->Data4[7]);

        if (pvtbl->KeyAction(HKEY_CLASSES_ROOT, tszClsid, &hkClsid) == 0) {
            TCHAR tszName[127];

            /* Do the type name */
            LoadString(g_hinst, c_rgclsidmap[iclsidmap].ids,
                       tszName, cA(tszName));
            pvtbl->StringAction(hkClsid, 0, tszName);

            /* Do the in-proc server name and threading model */
            if (pvtbl->KeyAction(hkClsid, c_tszInProcServer32, &hkSub) == 0) {
                pvtbl->StringAction(hkSub, 0, tszThisDll);
                pvtbl->StringAction(hkSub, c_tszThreadingModel, c_tszBoth);
                pvtbl->KeyFinish(hkClsid, c_tszInProcServer32, hkSub);
            }

            pvtbl->KeyFinish(HKEY_CLASSES_ROOT, tszClsid, hkClsid);

        }
    }
    
  #ifdef WINNT
    pvtbl->SetSecurity();
  #endif
}



/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   void | DllRegisterServer |
 *
 *          Register our classes with OLE/COM/ActiveX/whatever its name is.
 *
 *****************************************************************************/

void EXTERNAL
DllRegisterServer(void)
{
    DllServerAction(&c_vtblAdd);
}

/*****************************************************************************
 *
 *  @doc    INTERNAL
 *
 *  @func   void | DllUnregisterServer |
 *
 *          Unregister our classes from OLE/COM/ActiveX/whatever its name is.
 *
 *****************************************************************************/

void EXTERNAL
DllUnregisterServer(void)
{
    DllServerAction(&c_vtblDel);
}
