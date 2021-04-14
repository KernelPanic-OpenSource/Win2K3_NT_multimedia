/*==========================================================================
 *
 *  Copyright (C) 1994-1997 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:	   helpcli.c
 *  Content:	client code to talk to dplaysvr.exe
 *					allows multiple dplay winscock clients to share
 *					a single port.  see %manroot%\dplay\dplaysvr\dphelp.c
 *  History:
 *   Date		By		Reason
 *   ====		==		======
 *	2/15/97		andyco	created from w95help.h
 *
 ***************************************************************************/
#include "helpcli.h"

extern DWORD	dwHelperPid;


//**********************************************************************
// Globals
//**********************************************************************
BOOL					g_fDaclInited = FALSE;
SECURITY_ATTRIBUTES		g_sa;
BYTE					g_abSD[SECURITY_DESCRIPTOR_MIN_LENGTH];
PSECURITY_ATTRIBUTES	g_psa = NULL;
PACL					g_pEveryoneACL = NULL;





//**********************************************************************
// ------------------------------
// DNGetNullDacl - Get a SECURITY_ATTRIBUTE structure that specifies a 
//					NULL DACL which is accessible by all users.
//					Taken from IDirectPlay8 code base.
//
// Entry:		Nothing
//
// Exit:		PSECURITY_ATTRIBUTES
// ------------------------------
#undef DPF_MODNAME 
#define DPF_MODNAME "DNGetNullDacl"
PSECURITY_ATTRIBUTES DNGetNullDacl()
{
	PSID                     psidEveryone      = NULL;
	SID_IDENTIFIER_AUTHORITY siaWorld = SECURITY_WORLD_SID_AUTHORITY;
	DWORD					 dwAclSize;

	// This is done to make this function independent of DNOSIndirectionInit so that the debug
	// layer can call it before the indirection layer is initialized.
	if (!g_fDaclInited)
	{
		if (!InitializeSecurityDescriptor((SECURITY_DESCRIPTOR*)g_abSD, SECURITY_DESCRIPTOR_REVISION))
		{
			DPF(0, "Failed to initialize security descriptor" );
			goto Error;
		}

		// Create SID for the Everyone group.
		if (!AllocateAndInitializeSid(&siaWorld, 1, SECURITY_WORLD_RID, 0,
                                      0, 0, 0, 0, 0, 0, &psidEveryone))
		{
			DPF(0, "Failed to allocate Everyone SID" );
			goto Error;
		}

		dwAclSize = sizeof(ACL) + sizeof(ACCESS_ALLOWED_ACE) + GetLengthSid(psidEveryone) - sizeof(DWORD);

		// Allocate the ACL, this won't be a tracked allocation and we will let process cleanup destroy it
		g_pEveryoneACL = (PACL)HeapAlloc(GetProcessHeap(), 0, dwAclSize);
		if (g_pEveryoneACL == NULL)
		{
			DPF(0, "Failed to allocate ACL buffer" );
			goto Error;
		}

		// Intialize the ACL.
		if (!InitializeAcl(g_pEveryoneACL, dwAclSize, ACL_REVISION))
		{
			DPF(0, "Failed to initialize ACL" );
			goto Error;
		}

		// Add the ACE.
		if (!AddAccessAllowedAce(g_pEveryoneACL, ACL_REVISION, GENERIC_ALL, psidEveryone))
		{
			DPF(0, "Failed to add ACE to ACL" );
			goto Error;
		}

		// We no longer need the SID that was allocated.
		FreeSid(psidEveryone);
		psidEveryone = NULL;

		// Add the ACL to the security descriptor..
		if (!SetSecurityDescriptorDacl((SECURITY_DESCRIPTOR*)g_abSD, TRUE, g_pEveryoneACL, FALSE))
		{
			DPF(0, "Failed to add ACL to security descriptor" );
			goto Error;
		}

		g_sa.nLength = sizeof(SECURITY_ATTRIBUTES);
		g_sa.lpSecurityDescriptor = g_abSD;
		g_sa.bInheritHandle = FALSE;

		g_psa = &g_sa;

		g_fDaclInited = TRUE;
	}
Error:
	if (psidEveryone)
	{
		FreeSid(psidEveryone);
		psidEveryone = NULL;
	}
	return g_psa;
}
//**********************************************************************


/*
 * sendRequest
 *
 * communicate a request to DPHELP
 */
static BOOL sendRequest( LPDPHELPDATA req_phd )
{
	OSVERSIONINFOA	VersionInfo;
	BOOL			fUseGlobalNamespace;
	LPDPHELPDATA	phd;
	HANDLE			hmem;
	HANDLE			hmutex;
	HANDLE			hackevent;
	HANDLE			hstartevent;
	BOOL			rc;


	// Determine if we're running on NT.
	memset(&VersionInfo, 0, sizeof(VersionInfo));
	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
	if (GetVersionExA(&VersionInfo))
	{
		if (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			DPF(2, "Running on NT version %u.%u.%u, using global namespace.",
				VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, VersionInfo.dwBuildNumber);
			fUseGlobalNamespace = TRUE;
		}
		else
		{
			DPF(2, "Running on 9x version %u.%u.%u, not using global namespace.",
				VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, LOWORD(VersionInfo.dwBuildNumber));
			fUseGlobalNamespace = FALSE;
		}
	}
	else
	{
		DPF(0, "Could not determine OS version, assuming global namespace not needed.");
		fUseGlobalNamespace = FALSE;
	}


	/*
	 * get events start/ack events
	 */
	if (fUseGlobalNamespace)
	{
		hstartevent = CreateEvent( DNGetNullDacl(), FALSE, FALSE, "Global\\" DPHELP_EVENT_NAME );
	}
	else
	{
		hstartevent = CreateEvent( NULL, FALSE, FALSE, DPHELP_EVENT_NAME );
	}
	if( hstartevent == NULL )
	{
		return FALSE;
	}

	if (fUseGlobalNamespace)
	{
		hackevent = CreateEvent( DNGetNullDacl(), FALSE, FALSE, "Global\\" DPHELP_ACK_EVENT_NAME );
	}
	else
	{
		hackevent = CreateEvent( NULL, FALSE, FALSE, DPHELP_ACK_EVENT_NAME );
	}
	if( hackevent == NULL )
	{
		CloseHandle( hstartevent );
		return FALSE;
	}

	/*
	 * create shared memory area
	 */
	if (fUseGlobalNamespace)
	{
		hmem = CreateFileMapping( INVALID_HANDLE_VALUE, DNGetNullDacl(),
								PAGE_READWRITE, 0, sizeof( DPHELPDATA ),
								"Global\\" DPHELP_SHARED_NAME );
	}
	else
	{
		hmem = CreateFileMapping( INVALID_HANDLE_VALUE, NULL,
								PAGE_READWRITE, 0, sizeof( DPHELPDATA ),
								DPHELP_SHARED_NAME );
	}
	if( hmem == NULL )
	{
		DPF( 1, "Could not create file mapping!" );
		CloseHandle( hstartevent );
		CloseHandle( hackevent );
		return FALSE;
	}

	phd = (LPDPHELPDATA) MapViewOfFile( hmem, FILE_MAP_ALL_ACCESS, 0, 0, 0 );
	if( phd == NULL )
	{
		DPF( 1, "Could not create view of file!" );
		CloseHandle( hmem );
		CloseHandle( hstartevent );
		CloseHandle( hackevent );
		return FALSE;
	}

	/*
	 * wait for access to the shared memory
	 */
	if (fUseGlobalNamespace)
	{
		hmutex = OpenMutex( SYNCHRONIZE, FALSE, "Global\\" DPHELP_MUTEX_NAME );
	}
	else
	{
		hmutex = OpenMutex( SYNCHRONIZE, FALSE, DPHELP_MUTEX_NAME );
	}
	if( hmutex == NULL )
	{
		DPF( 1, "Could not create mutex!" );
		UnmapViewOfFile( phd );
		CloseHandle( hmem );
		CloseHandle( hstartevent );
		CloseHandle( hackevent );
		return FALSE;
	}
	WaitForSingleObject( hmutex, INFINITE );

	/*
	 * wake up DPHELP with our request
	 */
	memcpy( phd, req_phd, sizeof( DPHELPDATA ) );
	if( SetEvent( hstartevent ) )
	{
		WaitForSingleObject( hackevent, INFINITE );
		memcpy( req_phd, phd, sizeof( DPHELPDATA ) );
		rc = TRUE;
	}
	else
	{
		DPF( 1, "Could not signal event to notify DPHELP!" );
		rc = FALSE;
	}

	/*
	 * done with things
	 */
	ReleaseMutex( hmutex );
	CloseHandle( hmutex );
	CloseHandle( hstartevent );
	CloseHandle( hackevent );
	UnmapViewOfFile( phd );
	CloseHandle( hmem );
	return rc;

} /* sendRequest */


/*
 * HelpcliFini
 *
 * Free resources allocated to talk to DPlaySvr.
 */
void HelpcliFini(void)
{
	if (g_pEveryoneACL)
	{
		HeapFree(GetProcessHeap(), 0, g_pEveryoneACL);
		g_pEveryoneACL = NULL;
	}

	g_psa = NULL;
	g_fDaclInited = FALSE;
} /* HelpcliFini */


/*
 * WaitForHelperStartup
 */
BOOL WaitForHelperStartup( void )
{
	OSVERSIONINFOA	VersionInfo;
	BOOL			fUseGlobalNamespace;
	HANDLE			hevent;
	DWORD			rc;

	
	// Determine if we're running on NT.
	memset(&VersionInfo, 0, sizeof(VersionInfo));
	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
	if (GetVersionExA(&VersionInfo))
	{
		if (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			DPF(2, "Running on NT version %u.%u.%u, using global namespace.",
				VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, VersionInfo.dwBuildNumber);
			fUseGlobalNamespace = TRUE;
		}
		else
		{
			DPF(2, "Running on 9x version %u.%u.%u, not using global namespace.",
				VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, LOWORD(VersionInfo.dwBuildNumber));
			fUseGlobalNamespace = FALSE;
		}
	}
	else
	{
		rc = GetLastError();
		DPF(0, "Could not determine OS version (err = %u), assuming global namespace not needed.", rc);
		fUseGlobalNamespace = FALSE;
	}

	if (fUseGlobalNamespace)
	{
		hevent = CreateEvent( DNGetNullDacl(), TRUE, FALSE, "Global\\" DPHELP_STARTUP_EVENT_NAME );
	}
	else
	{
		hevent = CreateEvent( NULL, TRUE, FALSE, DPHELP_STARTUP_EVENT_NAME );
	}
	if( hevent == NULL )
	{
		return FALSE;
	}
	DPF( 3, "Wait DPHELP startup event to be triggered" );
	rc = WaitForSingleObject( hevent, INFINITE );
	CloseHandle( hevent );
	return TRUE;

} /* WaitForHelperStartup */

/*
 * CreateHelperProcess
 */
BOOL CreateHelperProcess( LPDWORD ppid )
{
	OSVERSIONINFOA	VersionInfo;
	BOOL			fUseGlobalNamespace;
	DWORD			rc;
	STARTUPINFO		si;
	PROCESS_INFORMATION	pi;
	HANDLE			h;
	char			szDPlaySvrPath[MAX_PATH + sizeof("\\dplaysvr.exe") + 1];
	char			szDPlaySvr[sizeof("dplaysvr.exe")] = "dplaysvr.exe";

	
	// Determine if we're running on NT.
	memset(&VersionInfo, 0, sizeof(VersionInfo));
	VersionInfo.dwOSVersionInfoSize = sizeof(VersionInfo);
	if (GetVersionExA(&VersionInfo))
	{
		if (VersionInfo.dwPlatformId == VER_PLATFORM_WIN32_NT)
		{
			//DPF(2, "Running on NT version %u.%u.%u, using global namespace.",
			//	VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, VersionInfo.dwBuildNumber);
			fUseGlobalNamespace = TRUE;
		}
		else
		{
			//DPF(2, "Running on 9x version %u.%u.%u, not using global namespace.",
			//	VersionInfo.dwMajorVersion, VersionInfo.dwMinorVersion, LOWORD(VersionInfo.dwBuildNumber));
			fUseGlobalNamespace = FALSE;
		}
	}
	else
	{
		rc = GetLastError();
		DPF(0, "Could not determine OS version (err = %u), assuming global namespace not needed.", rc);
		fUseGlobalNamespace = FALSE;
	}
	
	if( dwHelperPid == 0 )
	{
		if (fUseGlobalNamespace)
		{
			h = OpenEvent( SYNCHRONIZE, FALSE, "Global\\" DPHELP_STARTUP_EVENT_NAME );
		}
		else
		{
			h = OpenEvent( SYNCHRONIZE, FALSE, DPHELP_STARTUP_EVENT_NAME );
		}
		if( h == NULL )
		{
			// Get Windows system directory name
			if (GetSystemDirectory(szDPlaySvrPath, (MAX_PATH + 1)) == 0)
			{
				DPF( 0, "Could not get system directory" );
				return FALSE;
			}
			strcat(szDPlaySvrPath, "\\dplaysvr.exe");
			
			si.cb = sizeof(STARTUPINFO);
			si.lpReserved = NULL;
			si.lpDesktop = NULL;
			si.lpTitle = NULL;
			si.dwFlags = 0;
			si.cbReserved2 = 0;
			si.lpReserved2 = NULL;

			DPF( 3, "Creating helper process dplaysvr.exe now" );
			if( !CreateProcess(szDPlaySvrPath, szDPlaySvr,  NULL, NULL, FALSE,
							   NORMAL_PRIORITY_CLASS,
							   NULL, NULL, &si, &pi) )
			{
				DPF( 2, "Could not create dplaysvr.exe" );
				return FALSE;
			}
			dwHelperPid = pi.dwProcessId;
			DPF( 3, "Helper Process created" );
		}
		else
		{
			DPHELPDATA	hd;
			memset(&hd,0,sizeof(DPHELPDATA)); // make prefix happy.
			DPF( 3, "dplaysvr already exists, waiting for dplaysvr event" );
			WaitForSingleObject( h, INFINITE );
			CloseHandle( h );
			DPF( 3, "Asking for DPHELP pid" );
			hd.req = DPHELPREQ_RETURNHELPERPID;
			sendRequest( &hd );
			dwHelperPid = hd.pid;
			DPF( 3, "DPHELP pid = %08lx", dwHelperPid );
		}
		*ppid = dwHelperPid;
		return TRUE;
	}
	*ppid = dwHelperPid;
	return FALSE;

} /* CreateHelperProcess */

// notify dphelp.c that we have a new server on this system
HRESULT HelperAddDPlayServer(USHORT port)
{
	DPHELPDATA hd;
	DWORD pid = GetCurrentProcessId();

	memset(&hd, 0, sizeof(DPHELPDATA));
	hd.req = DPHELPREQ_DPLAYADDSERVER;
	hd.pid = pid;
	hd.port = port;
	if (sendRequest(&hd)) return hd.hr;
	else return E_FAIL;
				
} // HelperAddDPlayServer

// server is going away
BOOL HelperDeleteDPlayServer(USHORT port)
{
	DPHELPDATA hd;
	DWORD pid = GetCurrentProcessId();

	memset(&hd, 0, sizeof(DPHELPDATA));
	hd.req = DPHELPREQ_DPLAYDELETESERVER;
	hd.pid = pid;
	hd.port = port;
	return sendRequest(&hd);

} // HelperDeleteDPlayServer
