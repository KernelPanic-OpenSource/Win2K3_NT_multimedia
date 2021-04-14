/*==========================================================================
 *
 *  Copyright (C) 1998-2000 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:		ThreadPool.cpp
 *  Content:	main job thread pool
 *
 *
 *  History:
 *   Date		By		Reason
 *   ====		==		======
 *	11/25/98	jtk		Created
 ***************************************************************************/

#include "dnmdmi.h"


#undef DPF_SUBCOMP
#define DPF_SUBCOMP DN_SUBCOMP_MODEM

//**********************************************************************
// Constant definitions
//**********************************************************************

//
// events for threads
//
enum
{
	EVENT_INDEX_STOP_ALL_THREADS = 0,
	EVENT_INDEX_PENDING_JOB = 1,
	EVENT_INDEX_WAKE_NT_TIMER_THREAD = 1,
	EVENT_INDEX_SEND_COMPLETE = 2,
	EVENT_INDEX_RECEIVE_COMPLETE = 3,
	EVENT_INDEX_TAPI_MESSAGE = 4,

	EVENT_INDEX_MAX
};

//
// times to wait in milliseconds when polling for work thread shutdown
//
#define	WORK_THREAD_CLOSE_WAIT_TIME		3000
#define	WORK_THREAD_CLOSE_SLEEP_TIME	100

//**********************************************************************
// Macro definitions
//**********************************************************************

//**********************************************************************
// Structure definitions
//**********************************************************************

//
// structure for common data in Win9x thread
//
typedef	struct	_WIN9X_CORE_DATA
{
	DWORD		dwNextTimerJobTime;					// time when the next timer job needs service
	DNHANDLE	hWaitHandles[ EVENT_INDEX_MAX ];	// handles for waiting on
	DWORD		dwWaitHandleCount;					// count of handles to wait on
	DWORD		dwTimeToNextJob;					// time to next job
	BOOL		fTimerJobsActive;					// Boolean indicating that there are active jobs
	BOOL		fLooping;							// Boolean indicating that this thread is still active

} WIN9X_CORE_DATA;

//
// information passed to the Win9x workhorse thread
//
typedef struct	_WIN9X_THREAD_DATA
{
	CModemThreadPool		*pThisThreadPool;	// pointer to this object
} WIN9X_THREAD_DATA;

//
// information passed to the IOCompletion thread
//
typedef struct	_IOCOMPLETION_THREAD_DATA
{
	CModemThreadPool		*pThisThreadPool;	// pointer to this object
} IOCOMPLETION_THREAD_DATA;

//
// structure passed to dialog threads
//
typedef	struct	_DIALOG_THREAD_PARAM
{
	DIALOG_FUNCTION	*pDialogFunction;
	void			*pContext;
	CModemThreadPool		*pThisThreadPool;
} DIALOG_THREAD_PARAM;

//**********************************************************************
// Variable definitions
//**********************************************************************

//**********************************************************************
// Function prototypes
//**********************************************************************

//**********************************************************************
// Function definitions
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::PoolAllocFunction
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::PoolAllocFunction"

BOOL CModemThreadPool::PoolAllocFunction( void* pvItem, void* pvContext )
{
	CModemThreadPool* pThreadPool = (CModemThreadPool*)pvItem;

	pThreadPool->m_iTotalThreadCount = 0;
#ifdef WINNT
	pThreadPool->m_iNTCompletionThreadCount = 0;
	pThreadPool->m_fNTTimerThreadRunning = FALSE;
	pThreadPool->m_hIOCompletionPort = NULL;
#endif // WINNT
	pThreadPool->m_fAllowThreadCountReduction = FALSE;
	pThreadPool->m_iIntendedThreadCount = 0;
	pThreadPool->m_hStopAllThreads = NULL;
#ifdef WIN95
	pThreadPool->m_hSendComplete = NULL;
	pThreadPool->m_hReceiveComplete = NULL;
	pThreadPool->m_hTAPIEvent = NULL;
	pThreadPool->m_hFakeTAPIEvent = NULL;
#endif // WIN95
	pThreadPool->m_fTAPIAvailable = FALSE; 
	pThreadPool->m_iRefCount = 0;

	pThreadPool->m_Sig[0] = 'T';
	pThreadPool->m_Sig[1] = 'H';
	pThreadPool->m_Sig[2] = 'P';
	pThreadPool->m_Sig[3] = 'L';
	
	pThreadPool->m_OutstandingReadList.Initialize();
	pThreadPool->m_OutstandingWriteList.Initialize();
	memset( &pThreadPool->m_InitFlags, 0x00, sizeof( pThreadPool->m_InitFlags ) );
	memset( &pThreadPool->m_TAPIInfo, 0x00, sizeof( pThreadPool->m_TAPIInfo ) );
	pThreadPool->m_TimerJobList.Initialize();

	return TRUE;
}
//**********************************************************************

//**********************************************************************
// ------------------------------
// CModemThreadPool::PoolInitFunction
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::PoolInitFunction"

void CModemThreadPool::PoolInitFunction( void* pvItem, void* pvContext )
{
	CModemThreadPool* pThreadPool = (CModemThreadPool*)pvItem;

	DNASSERT(pThreadPool->m_iRefCount == 0);
	pThreadPool->m_iRefCount = 1;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::PoolDeallocFunction
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::PoolDeallocFunction"

void CModemThreadPool::PoolDeallocFunction( void* pvItem )
{
	const CModemThreadPool* pThreadPool = (CModemThreadPool*)pvItem;

	DNASSERT( pThreadPool->m_iTotalThreadCount == 0 );
#ifdef WINNT
	DNASSERT( pThreadPool->m_iNTCompletionThreadCount == 0 );
	DNASSERT( pThreadPool->m_fNTTimerThreadRunning == FALSE );
	DNASSERT( pThreadPool->m_hIOCompletionPort == NULL );
#endif // WINNT
	DNASSERT( pThreadPool->m_fAllowThreadCountReduction == FALSE );
	DNASSERT( pThreadPool->m_iIntendedThreadCount == 0 );
	DNASSERT( pThreadPool->m_hStopAllThreads == NULL );
#ifdef WIN95
	DNASSERT( pThreadPool->m_hSendComplete == NULL );
	DNASSERT( pThreadPool->m_hReceiveComplete == NULL );
	DNASSERT( pThreadPool->m_hTAPIEvent == NULL );
	DNASSERT( pThreadPool->m_hFakeTAPIEvent == NULL );
#endif // WIN95
	DNASSERT( pThreadPool->m_fTAPIAvailable == FALSE );

	DNASSERT( pThreadPool->m_OutstandingReadList.IsEmpty() != FALSE );
	DNASSERT( pThreadPool->m_OutstandingReadList.IsEmpty() != FALSE );
	DNASSERT( pThreadPool->m_TimerJobList.IsEmpty() != FALSE );

	DNASSERT( pThreadPool->m_InitFlags.fTAPILoaded == FALSE );
	DNASSERT( pThreadPool->m_InitFlags.fLockInitialized == FALSE );
	DNASSERT( pThreadPool->m_InitFlags.fIODataLockInitialized == FALSE );
	DNASSERT( pThreadPool->m_InitFlags.fJobDataLockInitialized == FALSE );
	DNASSERT( pThreadPool->m_InitFlags.fTimerDataLockInitialized == FALSE );
	DNASSERT( pThreadPool->m_InitFlags.fJobQueueInitialized == FALSE );
	DNASSERT( pThreadPool->m_iRefCount == 0 );
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::Initialize - initialize work threads
//
// Entry:		Nothing
//
// Exit:		Boolean indicating success
//				TRUE = success
//				FALSE = failure
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::Initialize"

BOOL	CModemThreadPool::Initialize( void )
{
	HRESULT	hTempResult;
	BOOL	fReturn;


	//
	// initialize
	//
	fReturn = TRUE;
	DNASSERT( m_InitFlags.fTAPILoaded == FALSE );
	DNASSERT( m_InitFlags.fLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fIODataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fJobDataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fTimerDataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fDataPortHandleTableInitialized == FALSE );
	DNASSERT( m_InitFlags.fJobQueueInitialized == FALSE );

	//
	// try to load TAPI before anything else
	//
	hTempResult = LoadTAPILibrary();
	if ( hTempResult == DPN_OK )
	{
		m_InitFlags.fTAPILoaded = TRUE;
	}
	else
	{
		DPFX(DPFPREP,  0, "Failed to load TAPI!" );
		DisplayDNError( 0, hTempResult );
	}

	//
	// initialize critical sections
	//
	if ( DNInitializeCriticalSection( &m_Lock ) == FALSE )
	{
		goto Failure;
	}
	DebugSetCriticalSectionRecursionCount( &m_Lock, 0 );
	DebugSetCriticalSectionGroup( &m_Lock, &g_blDPNModemCritSecsHeld );	 // separate dpnmodem CSes from the rest of DPlay's CSes
	m_InitFlags.fLockInitialized = TRUE;

	//
	// Win9x has poor APC support and as part of the workaround, the read and
	// write data locks need to be taken twice.  Adjust the recursion counts
	// accordingly.
	//
	if ( DNInitializeCriticalSection( &m_IODataLock ) == FALSE )
	{
	    goto Failure;
	}
	DebugSetCriticalSectionRecursionCount( &m_IODataLock, 1 );
	DebugSetCriticalSectionGroup( &m_IODataLock, &g_blDPNModemCritSecsHeld );	 // separate dpnmodem CSes from the rest of DPlay's CSes
	m_InitFlags.fIODataLockInitialized = TRUE;

	if ( DNInitializeCriticalSection( &m_JobDataLock ) == FALSE )
	{
		goto Failure;
	}
	DebugSetCriticalSectionRecursionCount( &m_JobDataLock, 0 );
	DebugSetCriticalSectionGroup( &m_JobDataLock, &g_blDPNModemCritSecsHeld );	 // separate dpnmodem CSes from the rest of DPlay's CSes
	m_InitFlags.fJobDataLockInitialized = TRUE;

	if ( DNInitializeCriticalSection( &m_TimerDataLock ) == FALSE )
	{
		goto Failure;
	}
	DebugSetCriticalSectionRecursionCount( &m_TimerDataLock, 0 );
	DebugSetCriticalSectionGroup( &m_TimerDataLock, &g_blDPNModemCritSecsHeld );	 // separate dpnmodem CSes from the rest of DPlay's CSes
	m_InitFlags.fTimerDataLockInitialized = TRUE;

	//
	// handle table
	//
	if ( m_DataPortHandleTable.Initialize() != DPN_OK )
	{
		goto Failure;
	}
	m_InitFlags.fDataPortHandleTableInitialized = TRUE;

	//
	// initialize job queue
	//
	if ( m_JobQueue.Initialize() == FALSE )
	{
		goto Failure;
	}
	m_InitFlags.fJobQueueInitialized = TRUE;

	//
	// Create event to stop all threads.  Win9x needs this to stop processing
	// and the NT enum thread uses this to stop processing
	//
	DNASSERT( m_hStopAllThreads == NULL );
	m_hStopAllThreads = DNCreateEvent( NULL,		// pointer to security (none)
			    					 TRUE,		// manual reset
			    					 FALSE,		// start unsignalled
			    					 NULL );	// pointer to name (none)
	if ( m_hStopAllThreads == NULL )
	{
		DWORD   dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to create event to stop all threads!" );
		DisplayErrorCode( 0, dwError );
		goto Failure;
	}

	DNASSERT( m_fAllowThreadCountReduction == FALSE );
	m_fAllowThreadCountReduction = TRUE;

	//
	// OS-specific initialization
	//
#ifdef WINNT
	//
	// WinNT
	//
	if (FAILED(WinNTInit()))
	{
		goto Failure;
	}
#else // WIN95
	//
	// Windows 9x
	//
	if (FAILED(Win9xInit()))
	{
		goto Failure;
	}
#endif // WINNT

	//
	// Verify all internal flags.  It's possible that TAPI didn't load
	// so don't check it (it's not a fatal condition).
	//
	DNASSERT( m_InitFlags.fLockInitialized != FALSE );
	DNASSERT( m_InitFlags.fIODataLockInitialized != FALSE );
	DNASSERT( m_InitFlags.fJobDataLockInitialized != FALSE );
	DNASSERT( m_InitFlags.fTimerDataLockInitialized != FALSE );
	DNASSERT( m_InitFlags.fJobQueueInitialized != FALSE );

Exit:
	return	fReturn;

Failure:
	fReturn = FALSE;
	StopAllThreads();
	Deinitialize();

	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::Deinitialize - destroy work threads
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::Deinitialize"

void	CModemThreadPool::Deinitialize( void )
{
//	DNASSERT( m_JobQueue.IsEmpty() != FALSE );

	//
	// request that all threads stop and then cycle our timeslice to
	// allow the threads a chance for cleanup
	//
	m_fAllowThreadCountReduction = FALSE;
	DPFX(DPFPREP, 9, "SetIntendedThreadCount 0");
	SetIntendedThreadCount( 0 );
	StopAllThreads();
	SleepEx( 0, TRUE );

#ifdef WINNT
	if ( m_hIOCompletionPort != NULL )
	{
		if ( CloseHandle( m_hIOCompletionPort ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem closing handle to I/O completion port!" );
			DisplayErrorCode( 0, dwError );
		}
		m_hIOCompletionPort = NULL;
	}
#endif // WINNT

	//
	// close StopAllThreads handle
	//
	if ( m_hStopAllThreads != NULL )
	{
		if ( DNCloseHandle( m_hStopAllThreads ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to close StopAllThreads handle!" );
			DisplayErrorCode( 0, dwError );
		}

		m_hStopAllThreads = NULL;
	}

#ifdef WIN95
	//
	// close handles for I/O events
	//
	if ( m_hSendComplete != NULL )
	{
		if ( DNCloseHandle( m_hSendComplete ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem closing SendComplete handle!" );
			DisplayErrorCode( 0, dwError );
		}

		m_hSendComplete = NULL;
	}

	if ( m_hReceiveComplete != NULL )
	{
		if ( DNCloseHandle( m_hReceiveComplete ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem closing ReceiveComplete handle!" );
			DisplayErrorCode( 0, dwError );
		}

		m_hReceiveComplete = NULL;
	}
#endif // WIN95
	//
	// Now that all of the threads are stopped, clean up any outstanding I/O.
	// this can be done without taking any locks
	//
	if (m_InitFlags.fJobQueueInitialized)
	{
		CancelOutstandingIO();	
	}

	//
	// double-check empty IO lists
	//
	DNASSERT( m_OutstandingWriteList.IsEmpty() != FALSE );
	DNASSERT( m_OutstandingReadList.IsEmpty() != FALSE );

	//
	// deinitialize handle table
	//
	if ( m_InitFlags.fDataPortHandleTableInitialized != FALSE )
	{
		m_DataPortHandleTable.Deinitialize();
		m_InitFlags.fDataPortHandleTableInitialized = FALSE;
	}

	//
	// deinitialize job queue
	//
	if ( m_InitFlags.fJobQueueInitialized != FALSE )
	{
		m_JobQueue.Deinitialize();
		m_InitFlags.fJobQueueInitialized = FALSE;
	}

	if ( m_InitFlags.fTimerDataLockInitialized != FALSE )
	{
		DNDeleteCriticalSection( &m_TimerDataLock );
		m_InitFlags.fTimerDataLockInitialized = FALSE;
	}

	if ( m_InitFlags.fJobDataLockInitialized != FALSE )
	{
		DNDeleteCriticalSection( &m_JobDataLock );
		m_InitFlags.fJobDataLockInitialized = FALSE;
	}

	if ( m_InitFlags.fIODataLockInitialized != FALSE )
	{
		DNDeleteCriticalSection( &m_IODataLock );
		m_InitFlags.fIODataLockInitialized = FALSE;
	}

	if ( m_InitFlags.fLockInitialized != FALSE )
	{
		DNDeleteCriticalSection( &m_Lock );
		m_InitFlags.fLockInitialized = FALSE;
	}

	//
	// unload TAPI
	//
	if ( m_TAPIInfo.hApplicationInstance != NULL )
	{
		DNASSERT( p_lineShutdown != NULL );
		p_lineShutdown( m_TAPIInfo.hApplicationInstance );
		m_TAPIInfo.hApplicationInstance = NULL;
	}
	m_fTAPIAvailable = FALSE;
	memset( &m_TAPIInfo, 0x00, sizeof( m_TAPIInfo ) );

#ifdef WIN95
	if (m_hTAPIEvent != NULL && m_hFakeTAPIEvent == NULL)
	{
		// In the case that we got the event from lineInitializeEx, lineShutdown will have closed the event handle,
		// so we can tell the handle tracking code that the handle is already cleaned up.
		REMOVE_DNHANDLE(m_hTAPIEvent);
	}
	m_hTAPIEvent = NULL;

	if ( m_hFakeTAPIEvent != NULL )
	{
		if ( DNCloseHandle( m_hFakeTAPIEvent ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem closing fake TAPI event!" );
			DisplayErrorCode( 0, GetLastError() );
		}

		m_hFakeTAPIEvent = NULL;
	}
#endif // WIN95
	//
	// close TAPI
	//
	if ( m_InitFlags.fTAPILoaded != FALSE )
	{
		UnloadTAPILibrary();
		m_InitFlags.fTAPILoaded = FALSE;
	}

	DNASSERT( m_InitFlags.fTAPILoaded == FALSE );
	DNASSERT( m_InitFlags.fLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fIODataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fJobDataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fTimerDataLockInitialized == FALSE );
	DNASSERT( m_InitFlags.fJobQueueInitialized == FALSE );
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::CreateReadIOData - create read IO data
//
// Entry:		Nothing
//
// Exit:		Pointer to Read IO Data
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::CreateReadIOData"

CModemReadIOData	*CModemThreadPool::CreateReadIOData( void )
{
	CModemReadIOData	*pReadData;


	LockReadData();
#ifdef WIN95
	pReadData = (CModemReadIOData*)g_ModemReadIODataPool.Get( GetReceiveCompleteEvent() );
#else
	pReadData = (CModemReadIOData*)g_ModemReadIODataPool.Get( NULL );
#endif // WIN95
	if ( pReadData != NULL )
	{
		pReadData->SetThreadPool( this );
		pReadData->m_OutstandingReadListLinkage.InsertBefore( &m_OutstandingReadList );
	}

	UnlockReadData();
	return	pReadData;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::ReturnReadIOData - return read data to pool
//
// Entry:		Pointer to read data
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ReturnReadIOData"

void	CModemThreadPool::ReturnReadIOData( CModemReadIOData *const pReadIOData )
{
	DNASSERT( pReadIOData != NULL );

	LockReadData();

	pReadIOData->m_OutstandingReadListLinkage.RemoveFromList();
	DNASSERT( pReadIOData->m_OutstandingReadListLinkage.IsEmpty() != FALSE );

	g_ModemReadIODataPool.Release( pReadIOData );

	UnlockReadData();
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::CreateWriteIOData - create Write IO data
//
// Entry:		Nothing
//
// Exit:		Pointer to Write IO Data
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::CreateWriteIOData"

CModemWriteIOData	*CModemThreadPool::CreateWriteIOData( void )
{
	CModemWriteIOData	*pWriteData;


	LockWriteData();
#ifdef WIN95
	pWriteData = (CModemWriteIOData*)g_ModemWriteIODataPool.Get( GetSendCompleteEvent() );
#else // WINNT
	pWriteData = (CModemWriteIOData*)g_ModemWriteIODataPool.Get( NULL );
#endif // WIN95
	if ( pWriteData != NULL )
	{
		pWriteData->m_OutstandingWriteListLinkage.InsertBefore( &m_OutstandingWriteList );
	}

	UnlockWriteData();
	return	pWriteData;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::ReturnWriteIOData - return write data to pool
//
// Entry:		Pointer to Write data
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ReturnWriteIOData"

void	CModemThreadPool::ReturnWriteIOData( CModemWriteIOData *const pWriteIOData )
{
	DNASSERT( pWriteIOData != NULL );

	LockWriteData();

	pWriteIOData->m_OutstandingWriteListLinkage.RemoveFromList();

	g_ModemWriteIODataPool.Release( pWriteIOData );

	UnlockWriteData();
}
//**********************************************************************

#ifdef WINNT
//**********************************************************************
// ------------------------------
// CModemThreadPool::WinNTInit - initialize WinNT components
//
// Entry:		Nothing
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::WinNTInit"

HRESULT	CModemThreadPool::WinNTInit( void )
{
	HRESULT					hr;
	LINEINITIALIZEEXPARAMS	LineInitializeExParams;
	LONG					lReturn;


	//
	// initialize
	//
	hr = DPN_OK;

	DNASSERT( m_hIOCompletionPort == NULL );
	m_hIOCompletionPort = CreateIoCompletionPort( INVALID_HANDLE_VALUE,	    // don't associate a file handle yet
												  NULL,					    // handle of existing completion port (none)
												  NULL,					    // completion key for callback (none)
												  0						    // number of concurent threads (0 = use number of processors)
												  );
	if ( m_hIOCompletionPort == NULL )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Could not create NT IOCompletionPort!" );
		DisplayErrorCode( 0, GetLastError() );
		goto Failure;
	}

	//
	// Initialize TAPI.  If TAPI doesn't start, it's not a problem, but note
	// the failure.
	//
	DNASSERT( m_fTAPIAvailable == FALSE );
	memset( &m_TAPIInfo, 0x00, sizeof( m_TAPIInfo ) );
	memset( &LineInitializeExParams, 0x00, sizeof( LineInitializeExParams ) );
	m_TAPIInfo.dwVersion = TAPI_CURRENT_VERSION;
	LineInitializeExParams.dwTotalSize = sizeof( LineInitializeExParams );
	LineInitializeExParams.dwOptions = LINEINITIALIZEEXOPTION_USECOMPLETIONPORT;
	LineInitializeExParams.dwCompletionKey = IO_COMPLETION_KEY_TAPI_MESSAGE;
	DNASSERT( GetIOCompletionPort() != NULL );
	LineInitializeExParams.Handles.hCompletionPort = GetIOCompletionPort();

	lReturn = LINEERR_UNINITIALIZED;

	if ( p_lineInitializeEx != NULL )
	{
		lReturn = p_lineInitializeEx( &m_TAPIInfo.hApplicationInstance,		// pointer to application TAPI instance handle
									  DNGetApplicationInstance(),			// instance handle of .DLL
									  NULL,									// callback function (not used)
									  NULL,									// friendly application name (none)
									  &m_TAPIInfo.dwLinesAvailable,			// pointer to number of devices available to TAPI
									  &m_TAPIInfo.dwVersion,				// pointer to input/output TAPI version
									  &LineInitializeExParams );			// pointer to extra params
	}

	if ( lReturn == LINEERR_NONE )
	{
		m_fTAPIAvailable = TRUE;
	}
	else
	{
		DPFX(DPFPREP,  0, "Failed to initialize TAPI for NT!" );
	}


	//
	// Prepare to spin up IOCompletionPort threads
	//
	DNASSERT( ThreadCount() == 0 );
	DNASSERT( NTCompletionThreadCount() == 0 );

	DPFX(DPFPREP, 9, "SetIntendedThreadCount %i", g_iThreadCount);
	SetIntendedThreadCount( g_iThreadCount );

Exit:
	return	hr;

Failure:
	DPFX(DPFPREP,  0, "Failed WinNT initialization!" );
	DisplayDNError( 0, hr );

	goto Exit;
}
//**********************************************************************
#endif // WINNT

#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::Win9xInit - initialize Win9x components
//
// Entry:		Nothing
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::Win9xInit"

HRESULT	CModemThreadPool::Win9xInit( void )
{
	HRESULT		hr;
	DNHANDLE	hPrimaryThread;
	DWORD		dwPrimaryThreadID;
	WIN9X_THREAD_DATA	*pPrimaryThreadInput;
	LINEINITIALIZEEXPARAMS	LineInitializeExParams;
	LONG		lReturn;


	//
	// initialize
	//
	hr = DPN_OK;
	hPrimaryThread = NULL;
	pPrimaryThreadInput = NULL;

	//
	// Initialize TAPI.  If this succeeds, it will give us an event to use for
	// TAPI messages.  If not, create a fake event that will take the place of
	// the TAPI event for the Win9x threads.
	//
	DNASSERT( m_fTAPIAvailable == FALSE );
	memset( &m_TAPIInfo, 0x00, sizeof( m_TAPIInfo ) );
	memset( &LineInitializeExParams, 0x00, sizeof( LineInitializeExParams ) );
	m_TAPIInfo.dwVersion = TAPI_CURRENT_VERSION;
	LineInitializeExParams.dwTotalSize = sizeof( LineInitializeExParams );
	LineInitializeExParams.dwOptions = LINEINITIALIZEEXOPTION_USEEVENT;

	lReturn = LINEERR_UNINITIALIZED;
	if ( p_lineInitializeEx != NULL )
	{
		lReturn = p_lineInitializeEx( &m_TAPIInfo.hApplicationInstance,		// pointer to application TAPI instance handle
									  DNGetApplicationInstance(),			// instance handle of .DLL
									  NULL,									// callback function (not used)
									  NULL,									// friendly application name (none)
									  &m_TAPIInfo.dwLinesAvailable,			// pointer to number of devices available to TAPI
									  &m_TAPIInfo.dwVersion,				// pointer to input/output TAPI version
									  &LineInitializeExParams );			// pointer to extra params
	}

	if ( lReturn == LINEERR_NONE )
	{
		m_hTAPIEvent = MAKE_DNHANDLE(LineInitializeExParams.Handles.hEvent);
		m_fTAPIAvailable = TRUE;
	}
	else
	{
		DPFX(DPFPREP,  0, "Failed to initialize TAPI for Win9x!" );
		DNASSERT( m_hTAPIEvent == NULL );
		DNASSERT( m_hFakeTAPIEvent == NULL );
		m_hFakeTAPIEvent = DNCreateEvent( NULL,		// pointer to security (none)
										TRUE,		// manual reset
										FALSE,		// start unsignalled
										NULL );		// pointer to name (none)
		if ( m_hFakeTAPIEvent == NULL )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to create fake TAPI event!" );
			DisplayErrorCode( 0, hr );
			hr = DPNERR_OUTOFMEMORY;
			goto Failure;
		}

		m_hTAPIEvent = m_hFakeTAPIEvent;
		DNASSERT( m_fTAPIAvailable == FALSE );
	}

	//
	// create send complete event
	//
	DNASSERT( m_hSendComplete == NULL );
	m_hSendComplete = DNCreateEvent( NULL,	// pointer to security (none)
								   TRUE,	// manual reset
								   FALSE,	// start unsignalled
								   NULL		// pointer to name (none)
								   );
	if ( m_hSendComplete == NULL )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to create event for Send!" );
		DisplayErrorCode( 0, dwError );
		hr = DPNERR_OUTOFMEMORY;
		goto Failure;
	}

	//
	// create receive complete event
	//
	DNASSERT( m_hReceiveComplete == NULL );
	m_hReceiveComplete = DNCreateEvent( NULL,		// pointer to security (none)
									  TRUE,		// manual reset
									  FALSE,	// start unsignalled
									  NULL		// pointer to name (none)
									  );
	if ( m_hReceiveComplete == NULL )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to create event for Receive!" );
		DisplayErrorCode( 0, dwError );
		hr = DPNERR_OUTOFMEMORY;
		goto Failure;
	}


	//
	// create parameters to worker threads
	//
	pPrimaryThreadInput = static_cast<WIN9X_THREAD_DATA*>( DNMalloc( sizeof( *pPrimaryThreadInput ) ) );
	if ( pPrimaryThreadInput == NULL )
	{
		DPFX(DPFPREP,  0, "Problem allocating memory for primary Win9x thread!" );
		hr = DPNERR_OUTOFMEMORY;
		goto Failure;
	}

	memset( pPrimaryThreadInput, 0x00, sizeof( *pPrimaryThreadInput ) );
	pPrimaryThreadInput->pThisThreadPool = this;

	//
	// Create one worker thread and boost its priority.  If the primary thread
	// can be created and boosted, create a secondary thread.  Do not create a
	// secondary thread if the primary could not be boosted because the system
	// is probably low on resources.
	//
	IncrementActiveThreadCount();
	hPrimaryThread = DNCreateThread( NULL,					// pointer to security attributes (none)
								   0,						// stack size (default)
								   PrimaryWin9xThread,		// pointer to thread function
								   pPrimaryThreadInput,		// pointer to input parameter
								   0,						// let it run
								   &dwPrimaryThreadID		// pointer to destination of thread ID
								   );
	if ( hPrimaryThread == NULL )
	{
		DWORD	dwError;


		//
		// Failed to create thread, decrement active thread count and report
		// error.
		//
		DecrementActiveThreadCount();

		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Problem creating Win9x thread!" );
		DisplayErrorCode( 0, dwError );
		hr = DPNERR_OUTOFMEMORY;

		goto Failure;
	}
	pPrimaryThreadInput = NULL;


	DPFX(DPFPREP,  8, "Created primary Win9x thread: 0x%x\tTotal Thread Count: %d", dwPrimaryThreadID, ThreadCount() );
	DNASSERT( hPrimaryThread != NULL );

#if ADJUST_THREAD_PRIORITY
	if ( SetThreadPriority( hPrimaryThread, THREAD_PRIORITY_ABOVE_NORMAL ) == FALSE )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to boost priority of primary Win9x read thread!  Not starting secondary thread" );
		DisplayErrorCode( 0, dwError );
	}
#endif // ADJUST_THREAD_PRIORITY

	//
	// Disallow thread reduction right off the bat.
	// We give them these two threads and that's what they're stuck with.
	//
	m_fAllowThreadCountReduction = FALSE;


Exit:
	if ( pPrimaryThreadInput != NULL )
	{
		DNFree( pPrimaryThreadInput );
		pPrimaryThreadInput = NULL;
	}

	if ( hPrimaryThread != NULL )
	{
		if ( DNCloseHandle( hPrimaryThread ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem closing Win9x thread hanle!" );
			DisplayErrorCode( 0, dwError );
		}

		hPrimaryThread = NULL;
	}

	return	hr;

Failure:
	DPFX(DPFPREP,  0, "Failed Win9x Initialization!" );
	DisplayDNError( 0, hr );
	goto Exit;
}
//**********************************************************************
#endif // WIN95

#ifdef WINNT
//**********************************************************************
// ------------------------------
// CModemThreadPool::StartNTCompletionThread - start a WinNT completion thread
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::StartNTCompletionThread"

void	CModemThreadPool::StartNTCompletionThread( void )
{
	DNHANDLE	hThread;
	DWORD		dwThreadID;
	IOCOMPLETION_THREAD_DATA	*pIOCompletionThreadData;


	pIOCompletionThreadData = static_cast<IOCOMPLETION_THREAD_DATA*>( DNMalloc( sizeof( *pIOCompletionThreadData ) ) );
	if ( pIOCompletionThreadData != NULL )
	{
		pIOCompletionThreadData->pThisThreadPool = this;
		hThread = NULL;
		hThread = DNCreateThread( NULL,						// pointer to security attributes (none)
								0,							// stack size (default)
								WinNTIOCompletionThread,	// thread function
								pIOCompletionThreadData,	// thread parameter
								0,							// start thread immediately
								&dwThreadID					// pointer to thread ID destination
								);
		if ( hThread != NULL )
		{
			//
			// note that a thread was created, and close the handle
			// to the thread because it's no longer needed.
			//
			IncrementActiveNTCompletionThreadCount();

			DPFX(DPFPREP,  8, "Creating I/O completion thread: 0x%x\tTotal Thread Count: %d\tNT Completion Thread Count: %d", dwThreadID, ThreadCount(), NTCompletionThreadCount() );
			if ( DNCloseHandle( hThread ) == FALSE )
			{
				DWORD	dwError;


				dwError = GetLastError();
				DPFX(DPFPREP,  0, "Problem creating thread for I/O completion port" );
				DisplayErrorCode( 0, dwError );
			}
		}
		else
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to create I/O completion thread!" );
			DisplayErrorCode( 0, dwError );

			DNFree( pIOCompletionThreadData );
		}
	}
}
//**********************************************************************
#endif // WINNT

//**********************************************************************
// ------------------------------
// CModemThreadPool::StopAllThreads - stop all work threads
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::StopAllThreads"

void	CModemThreadPool::StopAllThreads( void )
{
	//
	// stop all non-I/O completion threads
	//
	if ( m_hStopAllThreads != NULL )
	{
		if ( DNSetEvent( m_hStopAllThreads ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to set event to stop all threads!" );
			DisplayErrorCode( 0, dwError );
		}
	}

	//
	// If we're running on NT submit enough jobs to stop all threads.
	//
#ifdef WINNT
	UINT_PTR	uIndex;


	uIndex = NTCompletionThreadCount();
	while ( uIndex > 0 )
	{
		uIndex--;
		if ( PostQueuedCompletionStatus( m_hIOCompletionPort,		    // handle of completion port
										 0,							    // number of bytes transferred
										 IO_COMPLETION_KEY_SP_CLOSE,    // completion key
										 NULL						    // pointer to overlapped structure (none)
										 ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem submitting Stop job to IO completion port!" );
			DisplayErrorCode( 0, dwError );
		}
	}
#endif // WINNT
	//
	// check for outstanding threads (no need to lock thread pool count)
	//
	DPFX(DPFPREP,  8, "Number of outstanding threads: %d", ThreadCount() );
	while ( ThreadCount() != 0 )
	{
		DPFX(DPFPREP,  8, "Waiting for %d threads to quit.", ThreadCount() );
		SleepEx( WORK_THREAD_CLOSE_SLEEP_TIME, TRUE );
	}

	DNASSERT( ThreadCount() == 0 );
	m_iTotalThreadCount = 0;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::CancelOutstandingIO - cancel outstanding IO
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::CancelOutstandingIO"

void	CModemThreadPool::CancelOutstandingIO( void )
{
	CBilink	*pTemp;


	DNASSERT( ThreadCount() == 0 );

	//
	// stop any receives with the notification that they were cancelled
	//
	pTemp = m_OutstandingReadList.GetNext();
	while ( pTemp != &m_OutstandingReadList )
	{
		CModemReadIOData	*pReadData;


		pReadData = CModemReadIOData::ReadDataFromBilink( pTemp );
		pTemp = pTemp->GetNext();
		pReadData->m_OutstandingReadListLinkage.RemoveFromList();

		DPFX(DPFPREP, 1, "Forcing read data 0x%p to be cancelled.", pReadData);

#ifdef WIN95
		DNASSERT( pReadData->Win9xOperationPending() != FALSE );
		pReadData->SetWin9xOperationPending( FALSE );
#endif // WIN95
		pReadData->DataPort()->ProcessReceivedData( 0, ERROR_OPERATION_ABORTED );
	}

	//
	// stop any pending writes with the notification that the user cancelled it.
	//
	pTemp = m_OutstandingWriteList.GetNext();
	while ( pTemp != &m_OutstandingWriteList )
	{
		CModemWriteIOData	*pWriteData;


		pWriteData = CModemWriteIOData::WriteDataFromBilink( pTemp );
		pTemp = pTemp->GetNext();
		pWriteData->m_OutstandingWriteListLinkage.RemoveFromList();

		DPFX(DPFPREP, 1, "Forcing write data 0x%p to be cancelled.", pWriteData);

#ifdef WIN95
		DNASSERT( pWriteData->Win9xOperationPending() != FALSE );
		pWriteData->SetWin9xOperationPending( FALSE );
#endif // WIN95
		pWriteData->DataPort()->SendComplete( pWriteData, DPNERR_USERCANCEL );
	}

	while ( m_JobQueue.IsEmpty() == FALSE )
	{
		THREAD_POOL_JOB	*pJob;


		pJob = m_JobQueue.DequeueJob();
		DNASSERT( pJob != NULL );
		if (pJob->pCancelFunction != NULL)
		{
			pJob->pCancelFunction( pJob );
		}
		pJob->JobType = JOB_UNINITIALIZED;

		g_ModemThreadPoolJobPool.Release( pJob );
	};
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::ReturnSelfToPool - return this object to the pool
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ReturnSelfToPool"

void	CModemThreadPool::ReturnSelfToPool( void )
{
	g_ModemThreadPoolPool.Release( this );
}
//**********************************************************************



//**********************************************************************
// ------------------------------
// CModemThreadPool::ProcessTimerJobs - process timed jobs
//
// Entry:		Pointer to job list
//				Pointer to destination for time of next job
//
// Exit:		Boolean indicating active jobs exist
//				TRUE = there are active jobs
//				FALSE = there are no active jobs
//
// Notes:	The input job queue is expected to be locked for the duration
//			of this function call!
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ProcessTimerJobs"

BOOL	CModemThreadPool::ProcessTimerJobs( const CBilink *const pJobList, DWORD *const pdwNextJobTime )
{
	BOOL		fReturn;
	CBilink		*pWorkingEntry;
	INT_PTR		iActiveTimerJobCount;
	DWORD		dwCurrentTime;


	DNASSERT( pJobList != NULL );
	DNASSERT( pdwNextJobTime != NULL );

	//
	// Initialize.  Set the next job time to be infinitely far in the future
	// so this thread will wake up for any jobs that need to completed before
	// then.
	//
	fReturn = FALSE;
	DBG_CASSERT( OFFSETOF( TIMER_OPERATION_ENTRY, Linkage ) == 0 );
	pWorkingEntry = pJobList->GetNext();
	iActiveTimerJobCount = 0;
	dwCurrentTime = GETTIMESTAMP();
	(*pdwNextJobTime) = dwCurrentTime - 1;

	//
	// loop through all timer items
	//
	while ( pWorkingEntry != pJobList )
	{
		TIMER_OPERATION_ENTRY	*pTimerEntry;
		BOOL	fJobActive;


		pTimerEntry = TIMER_OPERATION_ENTRY::TimerOperationFromLinkage( pWorkingEntry );
		pWorkingEntry = pWorkingEntry->GetNext();

		fJobActive = ProcessTimedOperation( pTimerEntry, dwCurrentTime, pdwNextJobTime );
		DNASSERT( ( fJobActive == FALSE ) || ( fJobActive == TRUE ) );

		fReturn |= fJobActive;

		if ( fJobActive == FALSE )
		{
			RemoveTimerOperationEntry( pTimerEntry, DPN_OK );
		}
	}

	DNASSERT( ( fReturn == FALSE ) || ( fReturn == TRUE ) );

	return	fReturn;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::ProcessTimedOperation - process a timed operation
//
// Entry:		Pointer to job information
//				Current time
//				Pointer to time to be updated
//
// Exit:		Boolean indicating that the job is still active
//				TRUE = operation active
//				FALSE = operation not active
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ProcessTimedOperation"

BOOL	CModemThreadPool::ProcessTimedOperation( TIMER_OPERATION_ENTRY *const pTimedJob,
											const DWORD dwCurrentTime,
											DWORD *const pdwNextJobTime )
{
	BOOL	fEnumActive;


	DNASSERT( pTimedJob != NULL );
	DNASSERT( pdwNextJobTime != NULL );


	DPFX(DPFPREP, 9, "(0x%p) Parameters: (0x%p, %u, 0x%p)",
		this, pTimedJob, dwCurrentTime, pdwNextJobTime);

	//
	// Assume that this enum will remain active.  If we retire this enum, this
	// value will be reset.
	//
	fEnumActive = TRUE;

	//
	// If this enum has completed sending enums and is waiting only
	// for responses, decrement the wait time (assuming it's not infinite)
	// and remove the enum if the we've exceeded its wait time.
	//
	if ( pTimedJob->uRetryCount == 0 )
	{
		if ( (int) ( pTimedJob->dwIdleTimeout - dwCurrentTime ) <= 0 )
		{
			fEnumActive = FALSE;
		}
		else
		{
			//
			// This enum isn't complete, check to see if it's the next enum
			// to need service.
			//
			if ( (int) ( pTimedJob->dwIdleTimeout - (*pdwNextJobTime) ) < 0 )
			{
				(*pdwNextJobTime) = pTimedJob->dwIdleTimeout;
			}
		}
	}
	else
	{
		//
		// This enum is still sending.  Determine if it's time to send a new enum
		// and adjust the wakeup time if appropriate.
		//
		if ( (int) ( pTimedJob->dwNextRetryTime - dwCurrentTime ) <= 0 )
		{
#ifdef DBG
			DWORD	dwDelay;


			dwDelay = dwCurrentTime - pTimedJob->dwNextRetryTime;

			DPFX(DPFPREP, 7, "Threadpool 0x%p performing timed job 0x%p approximately %u ms after intended time of %u.",
				this, pTimedJob, dwDelay, pTimedJob->dwNextRetryTime);
#endif // DBG

			//
			// Timeout, execute this timed item
			//
			pTimedJob->pTimerCallback( pTimedJob->pContext );

			//
			// If this job isn't running forever, decrement the retry count.
			// If there are no more retries, set up wait time.  If the job
			// is waiting forever, set max wait timeout.
			//
			if ( pTimedJob->fRetryForever == FALSE )
			{
				pTimedJob->uRetryCount--;
				if ( pTimedJob->uRetryCount == 0 )
				{
					if ( pTimedJob->fIdleWaitForever == FALSE )
					{
						//
						// Compute stopping time for this job's 'Timeout' phase and
						// see if this will be the next job to need service.
						//
						pTimedJob->dwIdleTimeout += dwCurrentTime;
						if ( (int) (pTimedJob->dwIdleTimeout - (*pdwNextJobTime) ) < 0 )
						{
							(*pdwNextJobTime) = pTimedJob->dwIdleTimeout;
						}
					}
					else
					{
						//
						// We're waiting forever for enum returns.  ASSERT that we
						// have the maximum timeout and don't bother checking to see
						// if this will be the next enum to need service (it'll never
						// need service).
						//
						DNASSERT( pTimedJob->dwIdleTimeout == -1 );
					}

					goto SkipNextRetryTimeComputation;
				}
			}

			pTimedJob->dwNextRetryTime = dwCurrentTime + pTimedJob->dwRetryInterval;
		}

		//
		// is this the next enum to fire?
		//
		if ( (int) ( (*pdwNextJobTime) - pTimedJob->dwNextRetryTime ) < 0 )
		{
			(*pdwNextJobTime) = pTimedJob->dwNextRetryTime;


			DPFX(DPFPREP, 8, "Job 0x%p is the next job to fire (at time %u).",
				pTimedJob, pTimedJob->dwNextRetryTime);
		}
		else
		{
			//
			// Not next job to fire.
			//
		}

SkipNextRetryTimeComputation:
		//
		// the following blank line is there to shut up the compiler
		//
		;
	}


	DPFX(DPFPREP, 9, "(0x%p) Returning [%i]",
		this, fEnumActive);

	return	fEnumActive;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::SubmitWorkItem - submit a work item for processing and inform
//		work thread that another job is available
//
// Entry:		Pointer to job information
//
// Exit:		Error code
//
// Note:	This function assumes that the job data is locked.
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::SubmitWorkItem"

HRESULT	CModemThreadPool::SubmitWorkItem( THREAD_POOL_JOB *const pJobInfo )
{
	HRESULT	hr;


	DNASSERT( pJobInfo != NULL );
	AssertCriticalSectionIsTakenByThisThread( &m_JobDataLock, TRUE );

	//
	// initialize
	//
	hr = DPN_OK;

	//
	// add job to queue and tell someone that there's a job available
	//
	m_JobQueue.Lock();
	m_JobQueue.EnqueueJob( pJobInfo );
	m_JobQueue.Unlock();

#ifdef WINNT
	//
	// WinNT, submit new I/O completion item
	//
	DNASSERT( m_hIOCompletionPort != NULL );
	if ( PostQueuedCompletionStatus( m_hIOCompletionPort,			// completion port
									 0,								// number of bytes written (unused)
									 IO_COMPLETION_KEY_NEW_JOB,		// completion key
									 NULL							// pointer to overlapped structure (unused)
									 ) == FALSE )
	{
		DWORD	dwError;


		hr = DPNERR_OUTOFMEMORY;
		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Problem posting completion item for new job!" );
		DisplayErrorCode( 0, dwError );
		goto Failure;
	}
#else // WIN95
	//
	// Win9x, set event that the work thread will listen for
	//
	DNASSERT( m_JobQueue.GetPendingJobHandle() != NULL );
	if ( m_JobQueue.SignalPendingJob() == FALSE )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Failed to signal pending job!" );
		goto Failure;
	}
#endif // WINNT

Exit:
	if ( hr != DPN_OK )
	{
		DPFX(DPFPREP,  0, "Problem with SubmitWorkItem!" );
		DisplayDNError( 0, hr );
	}

	return	hr;

Failure:
	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::GetWorkItem - get a work item from the job queue
//
// Entry:		Nothing
//
// Exit:		Pointer to job information (may be NULL)
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::GetWorkItem"

THREAD_POOL_JOB	*CModemThreadPool::GetWorkItem( void )
{
	THREAD_POOL_JOB	*pReturn;


	//
	// initialize
	//
	pReturn = NULL;

	m_JobQueue.Lock();
	pReturn = m_JobQueue.DequeueJob();

	//
	// if we're under Win9x (we have a 'pending job' handle),
	// see if the handle needs to be reset
	//
	if ( m_JobQueue.IsEmpty() != FALSE )
	{
		DNASSERT( m_JobQueue.GetPendingJobHandle() != NULL );
		if ( DNResetEvent( m_JobQueue.GetPendingJobHandle() ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem resetting event for pending Win9x jobs!" );
			DisplayErrorCode( 0, dwError );
		}
	}

	m_JobQueue.Unlock();

	return	pReturn;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::SubmitTimerJob - add a timer job to the timer list
//
// Entry:		Retry count
//				Boolean indicating that we retry forever
//				Retry interval
//				Boolean indicating that we wait forever
//				Idle wait interval
//				Pointer to callback when event fires
//				Pointer to callback when event complete
//				User context
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::SubmitTimerJob"

HRESULT	CModemThreadPool::SubmitTimerJob( const UINT_PTR uRetryCount,
									 const BOOL fRetryForever,
									 const DWORD dwRetryInterval,
									 const BOOL fIdleWaitForever,
									 const DWORD dwIdleTimeout,
									 TIMER_EVENT_CALLBACK *const pTimerCallbackFunction,
									 TIMER_EVENT_COMPLETE *const pTimerCompleteFunction,
									 void *const pContext )
{
	HRESULT					hr;
	TIMER_OPERATION_ENTRY	*pEntry;
	THREAD_POOL_JOB			*pJob;
	BOOL					fTimerJobListLocked;


	DNASSERT( uRetryCount != 0 );
	DNASSERT( pTimerCallbackFunction != NULL );
	DNASSERT( pTimerCompleteFunction != NULL );
	DNASSERT( pContext != NULL );				// must be non-NULL because it's the lookup key to remove job

	//
	// initialize
	//
	hr = DPN_OK;
	pEntry = NULL;
	pJob = NULL;
	fTimerJobListLocked = FALSE;

	LockJobData();

	//
	// If we're on NT, attempt to start the enum thread here so we can return
	// an error if it fails to start.  If it does start, it'll sit until it's
	// informed that an enum job has been added.
	//
#ifdef WINNT
	hr = StartNTTimerThread();
	if ( hr != DPN_OK )
	{
		DPFX(DPFPREP,  0, "Cannot spin up NT timer thread!" );
		DisplayDNError( 0, hr );
		goto Failure;
	}
#endif // WINNT
	//
	// allocate new enum entry
	//
	pEntry = static_cast<TIMER_OPERATION_ENTRY*>( g_ModemTimerEntryPool.Get( ) );
	if ( pEntry == NULL )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Cannot allocate memory to add to timer list!" );
		goto Failure;
	}
	DNASSERT( pEntry->pContext == NULL );

	//
	// build timer entry block
	//
	pEntry->pContext = pContext;
	pEntry->uRetryCount = uRetryCount;
	pEntry->fRetryForever = fRetryForever;
	pEntry->dwRetryInterval = dwRetryInterval;
	pEntry->dwIdleTimeout = dwIdleTimeout;
	pEntry->fIdleWaitForever = fIdleWaitForever;
	pEntry->pTimerCallback = pTimerCallbackFunction;
	pEntry->pTimerComplete = pTimerCompleteFunction;

	//
	// set this enum to fire as soon as it gets a chance
	//
	pEntry->dwNextRetryTime = 0;

	pJob = static_cast<THREAD_POOL_JOB*>( g_ModemThreadPoolJobPool.Get( ) );
	if ( pJob == NULL )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Cannot allocate memory for enum job!" );
		goto Failure;
	}

	//
	// Create job for work thread.
	//
	pJob->pCancelFunction = NULL;
	pJob->JobType = JOB_REFRESH_TIMER_JOBS;

	// set our dummy paramter to simulate passing data
	DEBUG_ONLY( pJob->JobData.JobRefreshTimedJobs.uDummy = 0 );

	LockTimerData();
	fTimerJobListLocked = TRUE;

	//
	// we can submit the 'ENUM_REFRESH' job before inserting the enum entry
	// into the active enum list because nobody will be able to pull the
	// 'ENUM_REFRESH' job from the queue since we have the queue locked
	//
	hr = SubmitWorkItem( pJob );
	if ( hr != DPN_OK )
	{
		DPFX(DPFPREP,  0, "Problem submitting enum work item" );
		DisplayDNError( 0, hr );
		goto Failure;
	}

	//
	// debug block to check for duplicate contexts
	//
	DEBUG_ONLY(
				{
					CBilink	*pTempLink;


					pTempLink = m_TimerJobList.GetNext();
					while ( pTempLink != &m_TimerJobList )
					{
						TIMER_OPERATION_ENTRY	*pTempTimerEntry;


						pTempTimerEntry = TIMER_OPERATION_ENTRY::TimerOperationFromLinkage( pTempLink );
						DNASSERT( pTempTimerEntry->pContext != pContext );
						pTempLink = pTempLink->GetNext();
					}
				}
			);

	//
	// link to rest of list
	//
	pEntry->Linkage.InsertAfter( &m_TimerJobList );

Exit:
	if ( fTimerJobListLocked != FALSE )
	{
		UnlockTimerData();
		fTimerJobListLocked = FALSE;
	}

	UnlockJobData();

	if ( hr != DPN_OK )
	{
		DPFX(DPFPREP,  0, "Problem with SubmitEnumJob" );
		DisplayDNError( 0, hr );
	}

	return	hr;

Failure:
	if ( pEntry != NULL )
	{
		g_ModemTimerEntryPool.Release( pEntry );
		DEBUG_ONLY( pEntry = NULL );
	}

	if ( pJob != NULL )
	{
		g_ModemThreadPoolJobPool.Release( pJob );
		DEBUG_ONLY( pJob = NULL );
	}

	//
	// It's possible that the enum thread has been started for this enum.
	// Since there's no way to stop it without completing the enums or
	// closing the SP, leave it running.
	//

	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::StopTimerJob - remove timer job from list
//
// Entry:		Pointer to job context (these MUST be uniquie for jobs)
//				Command result
//
// Exit:		Boolean indicating whether a job was stopped or not
//
// Note:	This function is for the forced removal of a job from the timed job
//			list.  It is assumed that the caller of this function will clean
//			up any messes.
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::StopTimerJob"

BOOL	CModemThreadPool::StopTimerJob( void *const pContext, const HRESULT hCommandResult )
{
	BOOL						fComplete = FALSE;
	CBilink *					pTempEntry;
	TIMER_OPERATION_ENTRY *		pTimerEntry;


	DNASSERT( pContext != NULL );

	DPFX(DPFPREP, 9, "Parameters (0x%p, 0x%lx)", pContext, hCommandResult);
	
	//
	// initialize
	//
	LockTimerData();

	pTempEntry = m_TimerJobList.GetNext();
	while ( pTempEntry != &m_TimerJobList )
	{
		pTimerEntry = TIMER_OPERATION_ENTRY::TimerOperationFromLinkage( pTempEntry );
		if ( pTimerEntry->pContext == pContext )
		{
			//
			// remove this link from the list
			//
			pTimerEntry->Linkage.RemoveFromList();

			fComplete = TRUE;

			//
			// terminate loop
			//
			break;
		}

		pTempEntry = pTempEntry->GetNext();
	}

	UnlockTimerData();

	//
 	// tell owner that the job is complete and return the job to the pool
 	// outside of the lock
 	//
	if (fComplete)
	{
		pTimerEntry->pTimerComplete( hCommandResult, pTimerEntry->pContext );
		g_ModemTimerEntryPool.Release( pTimerEntry );
	}


	DPFX(DPFPREP, 9, "Returning [%i]", fComplete);

	return fComplete;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::SpawnDialogThread - start a secondary thread to display service
//		provider UI.
//
// Entry:		Pointer to dialog function
//				Dialog context
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::SpawnDialogThread"

HRESULT	CModemThreadPool::SpawnDialogThread( DIALOG_FUNCTION *const pDialogFunction, void *const pDialogContext )
{
	HRESULT	hr;
	DNHANDLE	hDialogThread;
	DIALOG_THREAD_PARAM		*pThreadParam;
	DWORD	dwThreadID;


	DNASSERT( pDialogFunction != NULL );
	DNASSERT( pDialogContext != NULL );		// why would anyone not want a dialog context??


	//
	// initialize
	//
	hr = DPN_OK;
	pThreadParam = NULL;

	//
	// create and initialize thread param
	//
	pThreadParam = static_cast<DIALOG_THREAD_PARAM*>( DNMalloc( sizeof( *pThreadParam ) ) );
	if ( pThreadParam == NULL )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Failed to allocate memory for dialog thread!" );
		goto Failure;
	}

	pThreadParam->pDialogFunction = pDialogFunction;
	pThreadParam->pContext = pDialogContext;
	pThreadParam->pThisThreadPool = this;

	//
	// assume that a thread will be created
	//
	IncrementActiveThreadCount();

	//
	// create thread
	//
	hDialogThread = DNCreateThread( NULL,				// pointer to security (none)
								  0,					// stack size (default)
								  DialogThreadProc,		// thread procedure
								  pThreadParam,			// thread param
								  0,					// creation flags (none)
								  &dwThreadID );		// pointer to thread ID
	if ( hDialogThread == NULL )
	{
		DWORD	dwError;


		//
		// decrement active thread count and report error
		//
		DecrementActiveThreadCount();

		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to start dialog thread!" );
		DisplayErrorCode( 0, dwError );
		goto Failure;
	}

	if ( DNCloseHandle( hDialogThread ) == FALSE )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Problem closing handle from create dialog thread!" );
		DisplayErrorCode( 0, dwError );
	}

Exit:	
	return	hr;

Failure:
	if ( pThreadParam != NULL )
	{
		DNFree( pThreadParam );
		pThreadParam = NULL;
	}

	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::GetIOThreadCount - get I/O thread count
//
// Entry:		Pointer to variable to fill
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define	DPF_MODNAME	"CModemThreadPool::GetIOThreadCount"

HRESULT	CModemThreadPool::GetIOThreadCount( LONG *const piThreadCount )
{
	HRESULT	hr;


	DNASSERT( piThreadCount != NULL );

	//
	// initialize
	//
	hr = DPN_OK;

	Lock();

	if ( IsThreadCountReductionAllowed() )
	{
		*piThreadCount = GetIntendedThreadCount();
	}
	else
	{
#ifdef WIN95
		*piThreadCount = ThreadCount();
#else // WINNT
		DNASSERT( NTCompletionThreadCount() != 0 );
		*piThreadCount = NTCompletionThreadCount();
#endif // WIN95
	}

	
	Unlock();

	return	hr;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::SetIOThreadCount - set I/O thread count
//
// Entry:		New thread count
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define	DPF_MODNAME	"CModemThreadPool::SetIOThreadCount"

HRESULT	CModemThreadPool::SetIOThreadCount( const LONG iThreadCount )
{
	HRESULT	hr;


	DNASSERT( iThreadCount > 0 );

	//
	// initialize
	//
	hr = DPN_OK;

	Lock();

	if ( IsThreadCountReductionAllowed() )
	{
		DPFX(DPFPREP, 4, "Thread pool not locked down, setting intended thread count to %i.", iThreadCount );
		SetIntendedThreadCount( iThreadCount );
	}
	else
	{
#ifdef WIN95
		//
		// Win9x can not adjust thread count.
		//
		DPFX(DPFPREP, 4, "Thread pool locked down and already has %i 9x threads, not adjusting to %i.",
			ThreadCount(), iThreadCount );
#else // WINNT
		//
		// WinNT can have many threads.  If the user wants more threads, attempt
		// to boost the thread pool to the requested amount (if we fail to
		// start a new thread, too bad).  If the user wants fewer threads, check
		// to see if the thread pool has been locked out of changes.  If not,
		// start killing off the threads.
		//
		if ( iThreadCount > NTCompletionThreadCount() )
		{
			INT_PTR	iDeltaThreads;


			iDeltaThreads = iThreadCount - NTCompletionThreadCount();

			DPFX(DPFPREP, 4, "Thread pool locked down, spawning %i new NT threads (for a total of %i).",
				iDeltaThreads, iThreadCount );
			
			while ( iDeltaThreads > 0 )
			{
				iDeltaThreads--;
				StartNTCompletionThread();
			}
		}
		else
		{
			DPFX(DPFPREP, 4, "Thread pool locked down and already has %i NT threads, not adjusting to %i.",
				NTCompletionThreadCount(), iThreadCount );
		}
#endif // WIN95
	}

	Unlock();

	return	hr;
}
//**********************************************************************



//**********************************************************************
// ------------------------------
// CModemThreadPool::PreventThreadPoolReduction - prevents the thread pool size from being reduced
//
// Entry:		None
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define	DPF_MODNAME	"CModemThreadPool::PreventThreadPoolReduction"

HRESULT CModemThreadPool::PreventThreadPoolReduction( void )
{
	HRESULT				hr = DPN_OK;
	LONG				iDesiredThreads;
#ifdef DBG
	DWORD				dwStartTime;
#endif // DBG


	Lock();

	//
	// If we haven't already clamped down, do so, and spin up the threads.
	//
	if ( IsThreadCountReductionAllowed() )
	{
		m_fAllowThreadCountReduction = FALSE;

		DNASSERT( GetIntendedThreadCount() > 0 );
		DNASSERT( ThreadCount() == 0 );

		iDesiredThreads = GetIntendedThreadCount();
		SetIntendedThreadCount( 0 );
		

		DPFX(DPFPREP, 3, "Locking down thread count at %i.", iDesiredThreads );

#ifdef DBG
		dwStartTime = GETTIMESTAMP();
#endif // DBG
		

		//
		// OS-specific thread starting.
		//
#ifdef WINNT
		//
		// WinNT
		//
		DNASSERT( NTCompletionThreadCount() == 0 );
		
		while ( iDesiredThreads > 0 )
		{
			iDesiredThreads--;
			StartNTCompletionThread();
		}

		//
		// If at least one thread was created, the SP will perform in a
		// non-optimal fashion, but we will still function.  If no threads
		// were created, fail.
		//
		if ( ThreadCount() == 0 )
		{
			hr = DPNERR_OUTOFMEMORY;
			DPFX(DPFPREP, 0, "Unable to create any threads to service NT I/O completion port!" );
			goto Failure;
		}
#else // WIN95
		//
		// Windows 9x
		//
		//
		// We should never get here because there will always only
		// be 1 thread.
		//
		DNASSERT( FALSE );
#endif // WINNT
		
#ifdef DBG
		DPFX(DPFPREP, 8, "Spent %u ms starting %i threads.", (GETTIMESTAMP() - dwStartTime), ThreadCount());
#endif // DBG
	}
	else
	{
		DPFX(DPFPREP, 3, "Thread count already locked down (at %i).", ThreadCount() );
	}

#ifdef WINNT
Exit:
#endif // WINNT
	
	Unlock();

	return	hr;

#ifdef WINNT
Failure:

	goto Exit;
#endif // WINNT
}
//**********************************************************************





//**********************************************************************
// ------------------------------
// CModemThreadPool::CreateDataPortHandle - create a new handle and assign a CDataPort
//		to it
//
// Entry:		Nothing
//
// Exit:		Error code
// ------------------------------
HRESULT	CModemThreadPool::CreateDataPortHandle( CDataPort *const pDataPort )
{
	HRESULT	hr;
	DPNHANDLE	hDataPort;


	DNASSERT( pDataPort != NULL );

	//
	// initialize
	//
	hr = DPN_OK;
	hDataPort = 0;

	hr = m_DataPortHandleTable.Create( pDataPort, &hDataPort );
	if ( hr != DPN_OK )
	{
		DNASSERT( hDataPort == 0 );
		DPFX(DPFPREP,  0, "Failed to create handle!" );
		DisplayDNError( 0, hr );
		goto Failure;
	}

	pDataPort->SetHandle( hDataPort );
	pDataPort->AddRef();

Exit:
	return	hr;

Failure:
	DNASSERT( hDataPort == 0 );
	DNASSERT( pDataPort->GetHandle() == 0 );
	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::CloseDataPortHandle - invalidate a handle for a CDataPort
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
void	CModemThreadPool::CloseDataPortHandle( CDataPort *const pDataPort )
{
	DPNHANDLE	hDataPort;


	DNASSERT( pDataPort != NULL );

	hDataPort = pDataPort->GetHandle();

	if (SUCCEEDED(m_DataPortHandleTable.Destroy( hDataPort, NULL )))
	{
		pDataPort->SetHandle( 0 );
		pDataPort->DecRef();
	}
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::DataPortFromHandle - convert a handle to a CDataPort
//
// Entry:		Handle
//
// Exit:		Pointer to CDataPort (may be NULL for invalid handle)
// ------------------------------
CDataPort	*CModemThreadPool::DataPortFromHandle( const DPNHANDLE hDataPort )
{
	CDataPort	*pDataPort;


	DNASSERT( hDataPort != 0 );

	pDataPort = NULL;

	m_DataPortHandleTable.Lock();
	if (SUCCEEDED(m_DataPortHandleTable.Find( hDataPort, (PVOID*)&pDataPort )))
	{
		pDataPort->AddRef();
	}
	m_DataPortHandleTable.Unlock();

	return	pDataPort;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::SubmitDelayedCommand - submit request to enum query to remote session
//
// Entry:		Pointer to callback function
//				Pointer to cancel function
//				Pointer to callback context
//
// Exit:		Error code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::SubmitDelayedCommand"

HRESULT	CModemThreadPool::SubmitDelayedCommand( JOB_FUNCTION *const pFunction, JOB_FUNCTION *const pCancelFunction, void *const pContext )
{
	HRESULT			hr;
	THREAD_POOL_JOB	*pJob;
	BOOL			fJobDataLocked;


	DNASSERT( pFunction != NULL );
	DNASSERT( pCancelFunction != NULL );

	//
	// initialize
	//
	hr = DPN_OK;
	pJob = NULL;
	fJobDataLocked = FALSE;

	pJob = static_cast<THREAD_POOL_JOB*>( g_ModemThreadPoolJobPool.Get( ) );
	if ( pJob == NULL )
	{
		hr = DPNERR_OUTOFMEMORY;
		DPFX(DPFPREP,  0, "Cannot allocate job for DelayedCommand!" );
		goto Failure;
	}

	pJob->JobType = JOB_DELAYED_COMMAND;
	pJob->pCancelFunction = pCancelFunction;
	pJob->JobData.JobDelayedCommand.pCommandFunction = pFunction;
	pJob->JobData.JobDelayedCommand.pContext = pContext;

	LockJobData();
	fJobDataLocked = TRUE;

	hr = SubmitWorkItem( pJob );
	if ( hr != DPN_OK )
	{
		DPFX(DPFPREP,  0, "Problem submitting DelayedCommand job!" );
		DisplayDNError( 0, hr );
		goto Failure;
	}

Exit:
	if ( fJobDataLocked != FALSE )
	{
		UnlockJobData();
		fJobDataLocked = FALSE;
	}

	return	hr;

Failure:
	if ( pJob != NULL )
	{
		g_ModemThreadPoolJobPool.Release( pJob );
	}

	goto Exit;
}
//**********************************************************************

#ifdef WINNT
//**********************************************************************
// ------------------------------
// CModemThreadPool::StartNTTimerThread - start the timer thread for NT
//
// Entry:		Nothing
//
// Exit:		Error code
//
// Note:	This function assumes that the enum data is locked.
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::StartNTTimerThread"

HRESULT	CModemThreadPool::StartNTTimerThread( void )
{
	HRESULT	hr;
	DNHANDLE	hThread;
	DWORD	dwThreadID;


	//
	// initialize
	//
	hr = DPN_OK;
	DNASSERT( m_JobQueue.GetPendingJobHandle() != NULL );

	if ( m_fNTTimerThreadRunning != FALSE )
	{
		//
		// the enum thread is already running, poke it to note new enums
		//
		if ( DNSetEvent( m_JobQueue.GetPendingJobHandle() ) == FALSE )
		{
			DWORD	dwError;


			hr = DPNERR_OUTOFMEMORY;
			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem setting event to wake NTTimerThread!" );
			DisplayErrorCode( 0, dwError );
			goto Failure;
		}

		goto Exit;
	}

//	DNASSERT( m_hWakeNTEnumThread != NULL );
//	m_NTEnumThreadData.hEventList[ EVENT_INDEX_ENUM_WAKEUP ] = m_hWakeNTEnumThread;

	IncrementActiveThreadCount();
	hThread = DNCreateThread( NULL,				// pointer to security attributes (none)
							0,					// stack size (default)
							WinNTTimerThread,	// thread function
							this,				// thread parameter
							0,					// creation flags (none, start running now)
							&dwThreadID			// pointer to thread ID
							);
	if ( hThread == NULL )
	{
		DWORD	dwError;


		hr = DPNERR_OUTOFMEMORY;
		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Failed to create NT timer thread!" );
		DisplayErrorCode( 0, dwError );
		DecrementActiveThreadCount();

		goto Failure;
	}

	//
	// note that the thread is running and close the handle to the thread
	//
	m_fNTTimerThreadRunning = TRUE;
	DPFX(DPFPREP,  8, "Creating NT-Timer thread: 0x%x\tTotal Thread Count: %d\tNT Completion Thread Count: %d", dwThreadID, ThreadCount(), NTCompletionThreadCount() );

	if ( DNCloseHandle( hThread ) == FALSE )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Problem closing handle after starting NTTimerThread!" );
		DisplayErrorCode( 0, dwError );
	}

Exit:
	return	hr;

Failure:
	goto Exit;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// CModemThreadPool::WakeNTTimerThread - wake the timer thread because a timed event
//		has been added
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::WakeNTTimerThread"

void	CModemThreadPool::WakeNTTimerThread( void )
{
	LockJobData();
	DNASSERT( m_JobQueue.GetPendingJobHandle() != FALSE );
	if ( DNSetEvent( m_JobQueue.GetPendingJobHandle() ) == FALSE )
	{
		DWORD	dwError;


		dwError = GetLastError();
		DPFX(DPFPREP,  0, "Problem setting event to wake up NT timer thread!" );
		DisplayErrorCode( 0, dwError );
	}
	UnlockJobData();
}
//**********************************************************************
#endif // WINNT

//**********************************************************************
// ------------------------------
// CModemThreadPool::RemoveTimerOperationEntry - remove timer operation job	from list
//
// Entry:		Pointer to timer operation
//				Result code to return
//
// Exit:		Nothing
//
// Note:	This function assumes that the list is appropriately locked
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::RemoveTimerOperationEntry"

void	CModemThreadPool::RemoveTimerOperationEntry( TIMER_OPERATION_ENTRY *const pTimerEntry, const HRESULT hJobResult )
{
	DNASSERT( pTimerEntry != NULL );
	AssertCriticalSectionIsTakenByThisThread( &m_TimerDataLock, TRUE );

	//
	// remove this link from the list, tell owner that the job is complete and
	// return the job to the pool
	//
	pTimerEntry->Linkage.RemoveFromList();
	pTimerEntry->pTimerComplete( hJobResult, pTimerEntry->pContext );
	g_ModemTimerEntryPool.Release( pTimerEntry );
}
//**********************************************************************


#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::CompleteOutstandingSends - check for completed sends and
//		indicate send completion for them.
//
// Entry:		Send complete event
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::CompleteOutstandingSends"

void	CModemThreadPool::CompleteOutstandingSends( const DNHANDLE hSendCompleteEvent )
{
	CBilink		*pCurrentOutstandingWrite;
	CBilink		WritesToBeProcessed;


	DNASSERT( hSendCompleteEvent != NULL );
	WritesToBeProcessed.Initialize();

	//
	// Loop through the list out outstanding sends.  Any completed sends are
	// removed from the list and processed after we release the write data lock.
	//
	LockWriteData();
	pCurrentOutstandingWrite = m_OutstandingWriteList.GetNext();
	while ( pCurrentOutstandingWrite != &m_OutstandingWriteList )
	{
		CDataPort		*pDataPort;
		CModemWriteIOData	*pWriteIOData;
		DWORD			dwFlags;
		DWORD			dwBytesSent;


		//
		// note this send and advance pointer to the next pending send
		//
		pWriteIOData = CModemWriteIOData::WriteDataFromBilink( pCurrentOutstandingWrite );
		pCurrentOutstandingWrite = pCurrentOutstandingWrite->GetNext();

		if ( pWriteIOData->Win9xOperationPending() != FALSE )
		{
			if ( GetOverlappedResult( HANDLE_FROM_DNHANDLE(pWriteIOData->DataPort()->GetFileHandle()),		// file handle
									  pWriteIOData->Overlap(),							// pointer to overlap structure
									  &dwBytesSent,										// pointer to bytes sent
									  FALSE												// wait for completion (don't wait)
									  ) != FALSE )
			{
				pWriteIOData->jkm_hSendResult = DPN_OK;
			}
			else
			{
				DWORD	dwError;


				dwError = GetLastError();
				switch( dwError )
				{
					//
					// ERROR_IO_PENDING = operation pending
					//
					case ERROR_IO_PENDING:
					{
						goto SkipSendCompletion;
						break;
					}

					//
					// ERROR_IO_INCOMPLETE = overlapped I/O is not in a signalled state
					// 						 this is expected since the event is always
					//						 cleared before checking I/O.  Assume complete.
					//
					case ERROR_IO_INCOMPLETE:
					{
						pWriteIOData->jkm_hSendResult = DPN_OK;
						break;
					}

					//
					// ERROR_OPERATION_ABORTED = operation complete because of cancel or
					//							 a thread quit.  (Com port was closed)
					//
					case ERROR_OPERATION_ABORTED:
					{
						pWriteIOData->jkm_hSendResult = DPNERR_USERCANCEL;
						break;
					}		

					//
					// ERROR_INVALID_HANDLE = the serial port is gone.  This is
					//						  possible when a modem hangs up.
					//
					case ERROR_INVALID_HANDLE:
					{
						pWriteIOData->jkm_hSendResult = DPNERR_NOCONNECTION;
						break;
					}

					//
					// other error, stop and take a look
					//
					default:
					{
						pWriteIOData->jkm_hSendResult = DPNERR_GENERIC;
						DisplayErrorCode( 0, dwError );

						DNASSERT( FALSE );
						break;
					}
				}
			}

			DNASSERT( pWriteIOData->Win9xOperationPending() != FALSE );
			pWriteIOData->SetWin9xOperationPending( FALSE );

			pWriteIOData->m_OutstandingWriteListLinkage.RemoveFromList();
			pWriteIOData->m_OutstandingWriteListLinkage.InsertBefore( &WritesToBeProcessed );
		}

SkipSendCompletion:
		//
		// the following line is present to prevent the compiler from whining
		// about a blank line
		//
		;
	}

	//
	// If there are no more outstanding reads, reset the write complete event.
	// It will be signalled when the next posted write completes.  No other read
	// can be posted at this time because the write data lock is held.
	//
	if ( m_OutstandingWriteList.IsEmpty() != FALSE )
	{
		if ( DNResetEvent( hSendCompleteEvent ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to reset send event!" );
			DisplayErrorCode( 0, dwError );
		}
	}
	UnlockWriteData();

	//
	// process all writes that have been pulled to the side.
	//
	while ( WritesToBeProcessed.GetNext() != &WritesToBeProcessed )
	{
		CModemWriteIOData	*pTempWrite;
		CDataPort		*pDataPort;


		pTempWrite = CModemWriteIOData::WriteDataFromBilink( WritesToBeProcessed.GetNext() );
		pTempWrite->m_OutstandingWriteListLinkage.RemoveFromList();
		pDataPort = pTempWrite->DataPort();
		DNASSERT( pDataPort != NULL );

		pDataPort->SendComplete( pTempWrite, pTempWrite->jkm_hSendResult );
//		pDataPort->SendFromWriteQueue();
//		pDataPort->DecRef();
	}
}
//**********************************************************************
#endif // WIN95


#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::CompleteOutstandingReceives - check for completed receives and
//		indicate completion for them.
//
// Entry:		Receive complete event
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::CompleteOutstandingReceives"

void	CModemThreadPool::CompleteOutstandingReceives( const DNHANDLE hReceiveCompleteEvent )
{
	CBilink		*pCurrentOutstandingRead;
	CBilink		ReadsToBeProcessed;


	DNASSERT( hReceiveCompleteEvent != NULL );
	ReadsToBeProcessed.Initialize();
	LockReadData();

	//
	// Loop through the list of outstanding reads and pull out the ones that need
	// to be serviced.  We don't want to service them while the read data lock
	// is taken.
	//
	pCurrentOutstandingRead = m_OutstandingReadList.GetNext();
	while ( pCurrentOutstandingRead != &m_OutstandingReadList )
	{
		CDataPort		*pDataPort;
		CModemReadIOData		*pReadIOData;
		DWORD			dwFlags;


		pReadIOData = CModemReadIOData::ReadDataFromBilink( pCurrentOutstandingRead );
		pCurrentOutstandingRead = pCurrentOutstandingRead->GetNext();

		//
		// Make sure this operation is really pending before attempting to check
		// for completion.  It's possible that the read was added to the list, but
		// we haven't actually called Winsock yet.
		//
		if ( pReadIOData->Win9xOperationPending() != FALSE )
		{
			if ( GetOverlappedResult( HANDLE_FROM_DNHANDLE(pReadIOData->DataPort()->GetFileHandle()),
									  pReadIOData->Overlap(),
									  &pReadIOData->jkm_dwOverlappedBytesReceived,
									  FALSE
									  ) != FALSE )
			{
				DBG_CASSERT( ERROR_SUCCESS == 0 );
//				pReadIOData->jkm_hReceiveReturn = DPN_OK;
				pReadIOData->m_dwWin9xReceiveErrorReturn = ERROR_SUCCESS;
			}
			else
			{
				DWORD	dwError;


				dwError = GetLastError();
				switch( dwError )
				{
					//
					// ERROR_IO_INCOMPLETE = treat as I/O complete.  Event isn't
					//						 signalled, but that's expected because
					//						 it's cleared before checking for I/O
					//
					case ERROR_IO_INCOMPLETE:
					{
						pReadIOData->jkm_dwOverlappedBytesReceived = pReadIOData->m_dwBytesToRead;
						pReadIOData->m_dwWin9xReceiveErrorReturn = ERROR_SUCCESS;
					    break;
					}

					//
					// ERROR_IO_PENDING = io still pending
					//
					case ERROR_IO_PENDING:
					{
						DNASSERT( FALSE );
						goto SkipReceiveCompletion;
						break;
					}

					//
					// other error, stop if not 'known'
					//
					default:
					{
						switch ( dwError )
						{
							//
							// ERROR_OPERATION_ABORTED = operation was cancelled (COM port closed)
							// ERROR_INVALID_HANDLE = operation was cancelled (COM port closed)
							//
							case ERROR_OPERATION_ABORTED:
							case ERROR_INVALID_HANDLE:
							{
								break;
							}

							default:
							{
								DisplayErrorCode( 0, dwError );
								DNASSERT( FALSE );
								break;
							}
						}

						pReadIOData->m_dwWin9xReceiveErrorReturn = dwError;

						break;
					}
				}
			}

			DNASSERT( pReadIOData->Win9xOperationPending() != FALSE );
			pReadIOData->SetWin9xOperationPending( FALSE );

			pReadIOData->m_OutstandingReadListLinkage.RemoveFromList();
			pReadIOData->m_OutstandingReadListLinkage.InsertBefore( &ReadsToBeProcessed );
		}

SkipReceiveCompletion:
		//
		// the following line is present to prevent the compiler from whining
		// about a blank line
		//
		;
	}

	//
	// If there are no more outstanding reads, reset the read complete event.
	// It will be signalled when the next posted read completes.  No other read
	// can be posted at this time because the read data lock is held.
	//
	if ( m_OutstandingReadList.IsEmpty() != FALSE )
	{
		if ( DNResetEvent( hReceiveCompleteEvent ) == FALSE )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Failed to reset receive event!" );
			DisplayErrorCode( 0, dwError );
		}
	}

	UnlockReadData();

	//
	// loop through the list of reads that have completed and dispatch them
	//
	while ( ReadsToBeProcessed.GetNext() != &ReadsToBeProcessed )
	{
		CModemReadIOData		*pTempRead;
		CDataPort		*pDataPort;


		pTempRead = CModemReadIOData::ReadDataFromBilink( ReadsToBeProcessed.GetNext() );
		pTempRead->m_OutstandingReadListLinkage.RemoveFromList();

		pDataPort = pTempRead->DataPort();
		DNASSERT( pDataPort != NULL );
		pDataPort->ProcessReceivedData( pTempRead->jkm_dwOverlappedBytesReceived, pTempRead->m_dwWin9xReceiveErrorReturn );
	}
}
//**********************************************************************
#endif // WIN95


#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::PrimaryWin9xThread - main thread to do everything that the SP is
//		supposed to do under Win9x.
//
// Entry:		Pointer to startup parameter
//
// Exit:		Error Code
//
// Note:	The startup parameter is allocated for this thread and must be
//			deallocated by this thread when it exits
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::PrimaryWin9xThread"

DWORD	WINAPI	CModemThreadPool::PrimaryWin9xThread( void *pParam )
{
	WIN9X_CORE_DATA		CoreData;
	DWORD	    		dwCurrentTime;
	DWORD	    		dwMaxWaitTime;
	BOOL				fComInitialized;


	CModemThreadPool		*const pThisThreadPool = static_cast<WIN9X_THREAD_DATA *>( pParam )->pThisThreadPool;


	DNASSERT( pParam != NULL );
	DNASSERT( pThisThreadPool != NULL );

	//
	// initialize
	//
	memset( &CoreData, 0x00, sizeof CoreData );
	fComInitialized = FALSE;

	//
	// before we do anything we need to make sure COM is happy
	//
	switch ( COM_CoInitialize( NULL ) )
	{
		//
		// no problem
		//
		case S_OK:
		{
			fComInitialized = TRUE;
			break;
		}

		//
		// COM already initialized, huh?
		//
		case S_FALSE:
		{
			DNASSERT( FALSE );
			break;
		}

		//
		// COM init failed!
		//
		default:
		{
			DPFX(DPFPREP,  0, "Primary Win9x thread failed to initialize COM!" );
			DNASSERT( FALSE );
			break;
		}
	}

	DNASSERT( CoreData.fTimerJobsActive == FALSE );

	//
	// set enums to happen infinitely in the future
	//
	CoreData.dwNextTimerJobTime = GETTIMESTAMP() - 1;

	//
	// set wait handles
	//
	CoreData.hWaitHandles[ EVENT_INDEX_STOP_ALL_THREADS ] = pThisThreadPool->m_hStopAllThreads;
	CoreData.hWaitHandles[ EVENT_INDEX_PENDING_JOB ] = pThisThreadPool->m_JobQueue.GetPendingJobHandle();
	CoreData.hWaitHandles[ EVENT_INDEX_SEND_COMPLETE ] = pThisThreadPool->GetSendCompleteEvent();
	CoreData.hWaitHandles[ EVENT_INDEX_RECEIVE_COMPLETE ] = pThisThreadPool->GetReceiveCompleteEvent();
	CoreData.hWaitHandles[ EVENT_INDEX_TAPI_MESSAGE ] = pThisThreadPool->GetTAPIMessageEvent();

	DNASSERT( CoreData.hWaitHandles[ EVENT_INDEX_STOP_ALL_THREADS ] != NULL );
	DNASSERT( CoreData.hWaitHandles[ EVENT_INDEX_PENDING_JOB ] != NULL );
	DNASSERT( CoreData.hWaitHandles[ EVENT_INDEX_SEND_COMPLETE ] != NULL );
	DNASSERT( CoreData.hWaitHandles[ EVENT_INDEX_RECEIVE_COMPLETE ] != NULL );
	DNASSERT( CoreData.hWaitHandles[ EVENT_INDEX_TAPI_MESSAGE ] );

	//
	// go until we're told to stop
	//
	CoreData.fLooping = TRUE;
	while ( CoreData.fLooping != FALSE )
	{
		DWORD   dwWaitReturn;


		//
		// Update the job time so we know how long to wait.  We can
		// only get here if a socket was just added to the socket list, or
		// we've been servicing sockets.
		//
		dwCurrentTime = GETTIMESTAMP();
		if ( (int) ( dwCurrentTime - CoreData.dwNextTimerJobTime ) >= 0 )
		{
			pThisThreadPool->LockTimerData();
			CoreData.fTimerJobsActive = pThisThreadPool->ProcessTimerJobs( &pThisThreadPool->m_TimerJobList,
			    														   &CoreData.dwNextTimerJobTime );
			if ( CoreData.fTimerJobsActive != FALSE )
			{
			    DPFX(DPFPREP,  8, "There are active jobs left with Winsock1 sockets active!" );
			}
			pThisThreadPool->UnlockTimerData();
		}

		dwMaxWaitTime = CoreData.dwNextTimerJobTime - dwCurrentTime;

		//
		// Check for I/O
		//
		dwWaitReturn = DNWaitForMultipleObjectsEx( LENGTHOF( CoreData.hWaitHandles ),		// count of handles
			    								 CoreData.hWaitHandles,					// handles to wait on
			    								 FALSE,									// don't wait for all to be signalled
			    								 dwMaxWaitTime,							// wait timeout
			    								 TRUE									// we're alertable for APCs
			    								 );
		switch ( dwWaitReturn )
		{
			//
			// timeout, don't do anything, we'll probably process timer jobs on
			// the next loop
			//
			case WAIT_TIMEOUT:
			{
			    break;
			}

			case ( WAIT_OBJECT_0 + EVENT_INDEX_PENDING_JOB ):
			case ( WAIT_OBJECT_0 + EVENT_INDEX_STOP_ALL_THREADS ):
			case ( WAIT_OBJECT_0 + EVENT_INDEX_SEND_COMPLETE ):
			case ( WAIT_OBJECT_0 + EVENT_INDEX_RECEIVE_COMPLETE ):
			case ( WAIT_OBJECT_0 + EVENT_INDEX_TAPI_MESSAGE ):
			{
				pThisThreadPool->ProcessWin9xEvents( &CoreData );
				break;
			}


			//
			// There are I/O completion routines scheduled on this thread.
			// This is not a good thing!
			//
			case WAIT_IO_COMPLETION:
			{
			    DPFX(DPFPREP,  1, "WARNING: APC was serviced on the primary Win9x IO service thread!  What is the application doing??" );
			    break;
			}

			//
			// wait failed
			//
			case WAIT_FAILED:
			{
			    DWORD	dwError;


			    dwError = GetLastError();
			    DPFX(DPFPREP,  0, "Primary Win9x thread wait failed!" );
			    DisplayDNError( 0, dwError );
			    break;
			}

			//
			// problem
			//
			default:
			{
			    DWORD	dwError;


			    dwError = GetLastError();
			    DPFX(DPFPREP,  0, "Primary Win9x thread unknown problem in wait!" );
			    DisplayDNError( 0, dwError );
			    DNASSERT( FALSE );
			    break;
			}
		}
	}

	pThisThreadPool->DecrementActiveThreadCount();

	DNFree( pParam );

	if ( fComInitialized != FALSE )
	{
		COM_CoUninitialize();
		fComInitialized = FALSE;
	}

	return	0;
}
//**********************************************************************
#endif // WIN95


#ifdef WINNT
//**********************************************************************
// ------------------------------
// CModemThreadPool::WinNTIOCompletionThread - thread to service I/O completion port
//
// Entry:		Pointer to startup parameter
//
// Exit:		Error Code
//
// Note:	The startup parameter is allocated for this thread and must be
//			deallocated by this thread when it exits
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::WinNTIOCompletionThread"

DWORD	WINAPI	CModemThreadPool::WinNTIOCompletionThread( void *pParam )
{
	IOCOMPLETION_THREAD_DATA	*pInput;
	BOOL	fLooping;
	HANDLE	hIOCompletionPort;
	BOOL	fComInitialized;


	DNASSERT( pParam != NULL );

	//
	// initialize
	//
	pInput = static_cast<IOCOMPLETION_THREAD_DATA*>( pParam );
	DNASSERT( pInput->pThisThreadPool != NULL );
	fLooping = TRUE;
	hIOCompletionPort = pInput->pThisThreadPool->m_hIOCompletionPort;
	DNASSERT( hIOCompletionPort != NULL );
	fComInitialized = FALSE;

	//
	// before we do anything we need to make sure COM is happy
	//
	switch ( COM_CoInitialize( NULL ) )
	{
		//
		// no problem
		//
		case S_OK:
		{
			fComInitialized = TRUE;
			break;
		}

		//
		// COM already initialized, huh?
		//
		case S_FALSE:
		{
			DNASSERT( FALSE );
			break;
		}

		//
		// COM init failed!
		//
		default:
		{
			DNASSERT( FALSE );
			DPFX(DPFPREP,  0, "Failed to initialize COM!" );
			break;
		}
	}

	//
	// go until we're told to stop
	//
	while ( fLooping != FALSE )
	{
		BOOL		fStatusReturn;
		DWORD		dwBytesTransferred;
		ULONG_PTR	uCompletionKey;
		OVERLAPPED	*pOverlapped;


		DNASSERT( hIOCompletionPort != NULL );
		fStatusReturn = GetQueuedCompletionStatus( hIOCompletionPort,		// handle of completion port
												   &dwBytesTransferred,		// pointer to number of bytes read
												   &uCompletionKey,			// pointer to completion key
												   &pOverlapped,			// pointer to overlapped structure
												   INFINITE					// wait forever
												   );
		if ( ( fStatusReturn == FALSE ) && ( pOverlapped == FALSE ) )
		{
			DWORD	dwError;


			dwError = GetLastError();
			DPFX(DPFPREP,  0, "Problem getting item from completion port!" );
			DisplayErrorCode( 0, dwError );
		}
		else
		{
			switch ( uCompletionKey )
			{
				//
				// ReadFile or WriteFile completed.  Check error status and
				// complete the appropriate operation.
				//
				case IO_COMPLETION_KEY_IO_COMPLETE:
				{
					CIOData		*pIOData;
					DWORD		dwError;


					DNASSERT( pOverlapped != NULL );
					if ( fStatusReturn != FALSE )
					{
						dwError = ERROR_SUCCESS;
					}
					else
					{
						dwError = GetLastError();
					}

					pIOData = CIOData::IODataFromOverlap( pOverlapped );
					if ( pIOData->NTIOOperationType() == NT_IO_OPERATION_RECEIVE )
					{
						DNASSERT( ( dwError == ERROR_SUCCESS ) || ( dwBytesTransferred == 0 ) );
						pIOData->DataPort()->ProcessReceivedData( dwBytesTransferred, dwError );
					}
					else
					{
						HRESULT		hOperationResult;


						DNASSERT( pIOData->NTIOOperationType() == NT_IO_OPERATION_SEND );
						switch ( dwError )
						{
							//
							// no problem
							//
							case ERROR_SUCCESS:
							{
								hOperationResult = DPN_OK;
								break;
							}

							//
							// ERROR_OPERATION_ABORTED = operation was stopped, most likely
							//		because of a user cancel
							//
							case ERROR_OPERATION_ABORTED:
							{
								hOperationResult = DPNERR_USERCANCEL;
								break;
							}

							default:
							{
								DNASSERT( FALSE );
								DPFX(DPFPREP,  0, "Failed on I/O completion send!" );
								DisplayErrorCode( 0, dwError );
								hOperationResult = DPNERR_GENERIC;
								break;
							}
						}

						pIOData->DataPort()->SendComplete( static_cast<CModemWriteIOData*>( pIOData ), hOperationResult );
					}

					break;
				}

				//
				// SP is closing, stop all threads
				//
				case IO_COMPLETION_KEY_SP_CLOSE:
				{
					DNASSERT( DNWaitForSingleObjectEx( pInput->pThisThreadPool->m_hStopAllThreads, 0, TRUE ) == WAIT_OBJECT_0 );
					fLooping = FALSE;
					break;
				}

				//
				// a new job was submitted to the job queue, or the SP is closing from above
				//
				case IO_COMPLETION_KEY_NEW_JOB:
				{
					THREAD_POOL_JOB	*pJobInfo;


					//
					// SP is still running, process our job
					//
					pJobInfo = pInput->pThisThreadPool->GetWorkItem();
					if ( pJobInfo != NULL )
					{
						switch ( pJobInfo->JobType )
						{
							//
							// enum refresh
							//
							case JOB_REFRESH_TIMER_JOBS:
							{
								DPFX(DPFPREP,  8, "IOCompletion job REFRESH_TIMER_JOBS" );
								DNASSERT( pJobInfo->JobData.JobRefreshTimedJobs.uDummy == 0 );

								pInput->pThisThreadPool->WakeNTTimerThread();

								break;
							}

							//
							// issue callback for this job
							//
							case JOB_DELAYED_COMMAND:
							{
								DPFX(DPFPREP,  8, "IOCompletion job DELAYED_COMMAND" );
								DNASSERT( pJobInfo->JobData.JobDelayedCommand.pCommandFunction != NULL );

								pJobInfo->JobData.JobDelayedCommand.pCommandFunction( pJobInfo );
								break;
							}

							//
							// other job
							//
							default:
							{
								DPFX(DPFPREP,  0, "IOCompletion job unknown!" );
								DNASSERT( FALSE );
								break;
							}
						}

						pJobInfo->JobType = JOB_UNINITIALIZED;
						g_ModemThreadPoolJobPool.Release( pJobInfo );
					}

					break;
				}

				//
				// TAPI message, pointer to line message is in pOverlapped and
				// we are responsible for freeing it via LocalFree()
				//
				case IO_COMPLETION_KEY_TAPI_MESSAGE:
				{
					LINEMESSAGE	*pLineMessage;
					CDataPort	*pModemPort;
					DPNHANDLE	hModemPort;


					DNASSERT( pOverlapped != NULL );
					DBG_CASSERT( sizeof( pLineMessage ) == sizeof( pOverlapped ) );
					pLineMessage = reinterpret_cast<LINEMESSAGE*>( pOverlapped );

					DBG_CASSERT( sizeof( pModemPort ) == sizeof( pLineMessage->dwCallbackInstance ) );
					hModemPort = (DPNHANDLE)( pLineMessage->dwCallbackInstance );

					pModemPort = static_cast<CDataPort*>( pInput->pThisThreadPool->DataPortFromHandle( hModemPort ) );
					if ( pModemPort != NULL )
					{
						pModemPort->ProcessTAPIMessage( pLineMessage );

						if ( LocalFree( pOverlapped ) != NULL )
						{
							DWORD	dwError;


							dwError = GetLastError();
							DPFX(DPFPREP,  0, "Problem with LocalFree in NTProcessTAPIMessage" );
							DisplayErrorCode( 0, dwError );
						}

						pModemPort->DecRef();
					}
					else
					{
						DPFX(DPFPREP,  5, "Dropping TAPI message on closed modem port!" );
						DisplayTAPIMessage( 5, pLineMessage );
					}

					break;
				}

				//
				// unknown I/O completion message type
				//
				default:
				{
					DNASSERT( FALSE );
					break;
				}
			}
		}
	}

	pInput->pThisThreadPool->DecrementActiveNTCompletionThreadCount();
	DNFree( pInput );

	if ( fComInitialized != FALSE )
	{
		COM_CoUninitialize();
		fComInitialized = FALSE;
	}

	return	0;
}
//**********************************************************************
#endif // WINNT

#ifdef WINNT
//**********************************************************************
// ------------------------------
// CModemThreadPool::WinNTTimerThread - timer thread for NT
//
// Entry:		Pointer to startup parameter
//
// Exit:		Error Code
//
// Note:	The startup parameter is a static memory chunk and cannot be freed.
//			Cleanup of this memory is the responsibility of this thread.
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::WinNTTimerThread"

DWORD	WINAPI	CModemThreadPool::WinNTTimerThread( void *pParam )
{
	CModemThreadPool	*pThisThreadPool;
	BOOL			fLooping;
	DWORD			dwWaitReturn;
	DWORD			dwNextEnumTime;
	DNHANDLE		hEvents[ 2 ];
	BOOL			fComInitialized;


	DNASSERT( pParam != NULL );

	//
	// initialize
	//
	DNASSERT( pParam != NULL );
	pThisThreadPool = static_cast<CModemThreadPool*>( pParam );
	DNASSERT( pThisThreadPool->m_JobQueue.GetPendingJobHandle() != NULL );

	dwNextEnumTime = GETTIMESTAMP() - 1;

	hEvents[ EVENT_INDEX_STOP_ALL_THREADS ] = pThisThreadPool->m_hStopAllThreads;
	hEvents[ EVENT_INDEX_WAKE_NT_TIMER_THREAD ] = pThisThreadPool->m_JobQueue.GetPendingJobHandle();

	fComInitialized = FALSE;


	//
	// before we do anything we need to make sure COM is happy
	//
	switch ( COM_CoInitialize( NULL ) )
	{
		//
		// no problem
		//
		case S_OK:
		{
			fComInitialized = TRUE;
			break;
		}

		//
		// COM already initialized, huh?
		//
		case S_FALSE:
		{
			DNASSERT( FALSE );
			fComInitialized = TRUE;
			break;
		}

		//
		// COM init failed!
		//
		default:
		{
			DNASSERT( FALSE );
			DPFX(DPFPREP, 0, "Failed to initialize COM!" );
			break;
		}
	}



	//
	// there were no active enums so we want to wait forever for something to
	// happen
	//
	fLooping = TRUE;

	//
	// go until we're told to stop
	//
	while ( fLooping != FALSE )
	{
		DWORD		dwCurrentTime;
		DWORD		dwMaxWaitTime;


		dwCurrentTime = GETTIMESTAMP();

		if ( (int) ( dwNextEnumTime - dwCurrentTime ) <= 0 )
		{

			//
			// acknowledge that we've handled this event and then process the
			// enums
			//
			pThisThreadPool->LockTimerData();

			if ( DNResetEvent( hEvents[ EVENT_INDEX_WAKE_NT_TIMER_THREAD ] ) == FALSE )
			{
				DWORD	dwError;


				dwError = GetLastError();
				DPFX(DPFPREP,  0, "Problem resetting event to wake NT timer thread!" );
				DisplayErrorCode( 0, dwError );
			}

			pThisThreadPool->ProcessTimerJobs( &pThisThreadPool->m_TimerJobList, &dwNextEnumTime );
			pThisThreadPool->UnlockTimerData();
		}

		dwMaxWaitTime = dwNextEnumTime - dwCurrentTime;

		DPFX(DPFPREP, 9, "Waiting %u ms until next timed job.", dwMaxWaitTime);

		dwWaitReturn = DNWaitForMultipleObjectsEx( LENGTHOF( hEvents ),	// number of events
												 hEvents,				// event list
												 FALSE,					// wait for any one event to be signalled
												 dwMaxWaitTime,			// timeout
												 TRUE					// be nice and allow APCs
												 );
		switch ( dwWaitReturn )
		{
			//
			// SP closing
			//
			case ( WAIT_OBJECT_0 + EVENT_INDEX_STOP_ALL_THREADS ):
			{
				DPFX(DPFPREP,  8, "NT timer thread thread detected SPClose!" );
				fLooping = FALSE;
				break;
			}

			//
			// Enum wakeup event, someone added an enum to the list.  Clear
			// our enum time and go back to the top of the loop where we
			// will process enums.
			//
			case ( WAIT_OBJECT_0 + EVENT_INDEX_WAKE_NT_TIMER_THREAD ):
			{
				dwNextEnumTime = GETTIMESTAMP();
				break;
			}

			//
			// Wait timeout.  We're probably going to process enums, go back
			// to the top of the loop.
			//
			case WAIT_TIMEOUT:
			{
				break;
			}

			//
			// wait failed
			//
			case WAIT_FAILED:
			{
				DPFX(DPFPREP,  0, "NT timer thread WaitForMultipleObjects failed: 0x%x", dwWaitReturn );
				DNASSERT( FALSE );
				break;
			}

			//
			// problem
			//
			default:
			{
				DNASSERT( FALSE );
				break;
			}
		}
	}

	DPFX(DPFPREP,  8, "NT timer thread is exiting!" );
	pThisThreadPool->LockTimerData();

	pThisThreadPool->m_fNTTimerThreadRunning = FALSE;
	pThisThreadPool->DecrementActiveThreadCount();

	pThisThreadPool->UnlockTimerData();


	if ( fComInitialized != FALSE )
	{
		COM_CoUninitialize();
		fComInitialized = FALSE;
	}

	return	0;
}
//**********************************************************************
#endif // WINNT

//**********************************************************************
// ------------------------------
// CModemThreadPool::DialogThreadProc - thread proc for spawning dialogs
//
// Entry:		Pointer to startup parameter
//
// Exit:		Error Code
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::DialogThreadProc"

DWORD WINAPI	CModemThreadPool::DialogThreadProc( void *pParam )
{
	const DIALOG_THREAD_PARAM	*pThreadParam;
	BOOL	fComInitialized;


	//
	// Initialize COM.  If this fails, we'll have problems later.
	//
	fComInitialized = FALSE;
	switch ( COM_CoInitialize( NULL ) )
	{
		case S_OK:
		{
			fComInitialized = TRUE;
			break;
		}

		case S_FALSE:
		{
			DNASSERT( FALSE );
			break;
		}

		//
		// COM init failed!
		//
		default:
		{
			DPFX(DPFPREP,  0, "Failed to initialize COM!" );
			DNASSERT( FALSE );
			break;
		}
	}

	DNASSERT( pParam != NULL );
	pThreadParam = static_cast<DIALOG_THREAD_PARAM*>( pParam );

	pThreadParam->pDialogFunction( pThreadParam->pContext );

	pThreadParam->pThisThreadPool->DecrementActiveThreadCount();
	DNFree( pParam );

	if ( fComInitialized != FALSE )
	{
		COM_CoUninitialize();
		fComInitialized = FALSE;
	}

	return	0;
}
//**********************************************************************

#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::ProcessWin9xEvents - process a Win9x events
//
// Entry:		Pointer core data
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ProcessWin9xEvents"

void	CModemThreadPool::ProcessWin9xEvents( WIN9X_CORE_DATA *const pCoreData )
{
	BOOL	fAllIOComplete;
	DNASSERT( pCoreData != NULL );


	//
	// this funciton checks each of the handles to see if they're signalled
	// to prevent I/O from starving the rest of the handles
	//
	fAllIOComplete = TRUE;

	//
	// New job.  Account for the time spent in the wait.  Don't
	// account for time after the job is complete because it's
	// possible that the job was an job submission which will want
	// to reset the wait time.
	//
	switch ( DNWaitForSingleObject( pCoreData->hWaitHandles[ EVENT_INDEX_PENDING_JOB ], 0 ) )
	{
		case WAIT_OBJECT_0:
		{
			DPFX(DPFPREP,  8, "Primary Win9x thread has a pending job!" );
			ProcessWin9xJob( pCoreData );
			break;
		}

		case WAIT_TIMEOUT:
		{
			break;
		}

		default:
		{
			DNASSERT( FALSE );
			break;
		}
	}


	//
	// TAPI message
	//
	switch ( DNWaitForSingleObject( pCoreData->hWaitHandles[ EVENT_INDEX_TAPI_MESSAGE ], 0 ) )
	{
		case WAIT_OBJECT_0:
		{
			DPFX(DPFPREP,  8, "Processing TAPI event!" );
			ProcessTapiEvent();
			break;
		}

		case WAIT_TIMEOUT:
		{
			break;
		}

		default:
		{
			DNASSERT( FALSE );
			break;
		}
	}


	//
	// send complete
	//
	switch ( DNWaitForSingleObject( pCoreData->hWaitHandles[ EVENT_INDEX_SEND_COMPLETE ], 0 ) )
	{
		case WAIT_OBJECT_0:
		{
//		    DPFX(DPFPREP,  0, "\n\n\nPrimary Win9x thread servicing sends!\n\n\n" );
			CompleteOutstandingSends( pCoreData->hWaitHandles[ EVENT_INDEX_SEND_COMPLETE ] );
			break;
		}

		case WAIT_TIMEOUT:
		{
			break;
		}

		default:
		{
			DNASSERT( FALSE );
			break;
		}
	}

	//
	// receive complete
	//
	switch ( DNWaitForSingleObject( pCoreData->hWaitHandles[ EVENT_INDEX_RECEIVE_COMPLETE ], 0 ) )
	{
		case WAIT_OBJECT_0:
		{
//		    DPFX(DPFPREP,  0, "\n\n\nPrimary Win9x thread servicing receives!\n\n\n" );
			CompleteOutstandingReceives( pCoreData->hWaitHandles[ EVENT_INDEX_RECEIVE_COMPLETE ] );
			break;
		}

		case WAIT_TIMEOUT:
		{
			break;
		}

		default:
		{
			DNASSERT( FALSE );
			break;
		}
	}

	//
	// SP closing
	//
	switch ( DNWaitForSingleObject( pCoreData->hWaitHandles[ EVENT_INDEX_STOP_ALL_THREADS ], 0 ) )
	{
		case WAIT_OBJECT_0:
		{
			DPFX(DPFPREP,  8, "Primary Win9x thread exit because SP closing!" );
			pCoreData->fLooping = FALSE;
			break;
		}

		case WAIT_TIMEOUT:
		{
			break;
		}

		default:
		{
			DNASSERT( FALSE );
			break;
		}
	}



	//
	// If there is I/O pending the Read/Write handles are probably still signalled.
	// Wait 5 milliseconds to process it before running through the handles again.
	//
	LockReadData();
	LockWriteData();

	if ( ( m_OutstandingReadList.IsEmpty() == FALSE ) ||
		 ( m_OutstandingWriteList.IsEmpty() == FALSE ) )
	{
		fAllIOComplete = FALSE;
	}

	UnlockReadData();
	UnlockWriteData();

	if ( fAllIOComplete == FALSE )
	{
		SleepEx( 5, TRUE );
	}
}
//**********************************************************************
#endif // WIN95


#ifdef WIN95
//**********************************************************************
// ------------------------------
// CModemThreadPool::ProcessWin9xJob - process a Win9x job
//
// Entry:		Pointer core data
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "CModemThreadPool::ProcessWin9xJob"

void	CModemThreadPool::ProcessWin9xJob( WIN9X_CORE_DATA *const pCoreData )
{
	THREAD_POOL_JOB	*pJobInfo;


	//
	// remove and process a single job from the list
	//
	pJobInfo = GetWorkItem();
	if ( pJobInfo != NULL )
	{
		switch ( pJobInfo->JobType )
		{
			//
			// enum refresh
			//
			case JOB_REFRESH_TIMER_JOBS:
			{
				DPFX(DPFPREP,  8, "WorkThread job REFRESH_ENUM" );
				DNASSERT( pJobInfo->JobData.JobRefreshTimedJobs.uDummy == 0 );
				LockTimerData();
				pCoreData->fTimerJobsActive = ProcessTimerJobs( &m_TimerJobList, &pCoreData->dwNextTimerJobTime );
				UnlockTimerData();

				if ( pCoreData->fTimerJobsActive != FALSE )
				{
					DPFX(DPFPREP,  8, "There are active timer jobs left after processing a Win9x REFRESH_TIMER_JOBS" );
				}

				break;
			}

			//
			// issue callback for this job
			//
			case JOB_DELAYED_COMMAND:
			{
				DPFX(DPFPREP,  8, "WorkThread job DELAYED_COMMAND" );
				DNASSERT( pJobInfo->JobData.JobDelayedCommand.pCommandFunction != NULL );
				pJobInfo->JobData.JobDelayedCommand.pCommandFunction( pJobInfo );
				break;
			}

			//
			// other job
			//
			default:
			{
				DPFX(DPFPREP,  0, "WorkThread Win9x job unknown!" );
				DNASSERT( FALSE );
				break;
			}
		}

		pJobInfo->JobType = JOB_UNINITIALIZED;
		g_ModemThreadPoolJobPool.Release( pJobInfo );
	}
}
//**********************************************************************
#endif // WIN95


//**********************************************************************
// ------------------------------
// CModemThreadPool::ProcessTapiEvent - process TAPI event
//
// Entry:		Nothing
//
// Exit:		Nothing
// ------------------------------
void	CModemThreadPool::ProcessTapiEvent( void )
{
	LONG		lTapiReturn;
	LINEMESSAGE	LineMessage;


	lTapiReturn = p_lineGetMessage( GetTAPIInfo()->hApplicationInstance, &LineMessage, 0 );
	if ( lTapiReturn != LINEERR_NONE )
	{
		DPFX(DPFPREP,  0, "Failed to process Win9x TAPI message!" );
		DisplayTAPIError( 0, lTapiReturn );
	}
	else
	{
		CDataPort	*pModemPort;
		DPNHANDLE	hModemPort;


		DNASSERT( sizeof( hModemPort ) == sizeof( LineMessage.dwCallbackInstance ) );
		hModemPort = (DPNHANDLE)( LineMessage.dwCallbackInstance );

		pModemPort = static_cast<CDataPort*>( DataPortFromHandle( hModemPort ) );
		if ( pModemPort != NULL )
		{
			pModemPort->ProcessTAPIMessage( &LineMessage );
			pModemPort->DecRef();
		}
	}
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ThreadPoolJob_Alloc - allocate a new job
//
// Entry:		Pointer to new entry
//
// Exit:		Boolean indicating success
//				TRUE = initialization successful
//				FALSE = initialization failed
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ThreadPoolJob_Alloc"

BOOL ThreadPoolJob_Alloc( void *pvItem, void* pvContext )
{
	BOOL			fReturn;
	THREAD_POOL_JOB	*pJob;


	//
	// initialize
	//
	fReturn = TRUE;
	pJob = static_cast<THREAD_POOL_JOB*>( pvItem );

	memset( pJob, 0x00, sizeof( *pJob ) );

	return	fReturn;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ThreadPoolJob_Get - a job is being removed from the pool
//
// Entry:		Pointer to job
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ThreadPoolJob_Get"

void ThreadPoolJob_Get( void *pvItem, void* pvContext )
{
	THREAD_POOL_JOB	*pJob;


	//
	// initialize
	//
	pJob = static_cast<THREAD_POOL_JOB*>( pvItem );
	DNASSERT( pJob->JobType == JOB_UNINITIALIZED );
	DNASSERT( pJob->pNext == NULL );
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ThreadPoolJob_Release - a job is being returned to the pool
//
// Entry:		Pointer to job
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ThreadPoolJob_Release"

void ThreadPoolJob_Release( void *pvItem )
{
	THREAD_POOL_JOB	*pJob;


	DNASSERT( pvItem != NULL );
	pJob = static_cast<THREAD_POOL_JOB*>( pvItem );

	DNASSERT( pJob->JobType == JOB_UNINITIALIZED );
	pJob->pNext = NULL;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ModemTimerEntry_Alloc - allocate a new timer job entry
//
// Entry:		Pointer to new entry
//
// Exit:		Boolean indicating success
//				TRUE = initialization successful
//				FALSE = initialization failed
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ModemTimerEntry_Alloc"

BOOL ModemTimerEntry_Alloc( void *pvItem, void* pvContext )
{
	BOOL					fReturn;
	TIMER_OPERATION_ENTRY	*pTimerEntry;


	DNASSERT( pvItem != NULL );

	//
	// initialize
	//
	fReturn = TRUE;
	pTimerEntry = static_cast<TIMER_OPERATION_ENTRY*>( pvItem );
	memset( pTimerEntry, 0x00, sizeof( *pTimerEntry ) );
	pTimerEntry->pContext = NULL;
	pTimerEntry->Linkage.Initialize();

	return	fReturn;
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ModemTimerEntry_Get - get new timer job entry from pool
//
// Entry:		Pointer to new entry
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ModemTimerEntry_Get"

void ModemTimerEntry_Get( void *pvItem, void* pvContext )
{
	TIMER_OPERATION_ENTRY	*pTimerEntry;


	DNASSERT( pvItem != NULL );

	pTimerEntry = static_cast<TIMER_OPERATION_ENTRY*>( pvItem );

	pTimerEntry->Linkage.Initialize();
	DNASSERT( pTimerEntry->pContext == NULL );
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ModemTimerEntry_Release - return timer job entry to pool
//
// Entry:		Pointer to entry
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ModemTimerEntry_Release"

void ModemTimerEntry_Release( void *pvItem )
{
	TIMER_OPERATION_ENTRY	*pTimerEntry;


	DNASSERT( pvItem != NULL );

	pTimerEntry = static_cast<TIMER_OPERATION_ENTRY*>( pvItem );
	pTimerEntry->pContext= NULL;

	DNASSERT( pTimerEntry->Linkage.IsEmpty() != FALSE );
}
//**********************************************************************


//**********************************************************************
// ------------------------------
// ModemTimerEntry_Dealloc - deallocate a timer job entry
//
// Entry:		Pointer to entry
//
// Exit:		Nothing
// ------------------------------
#undef DPF_MODNAME
#define DPF_MODNAME "ModemTimerEntry_Dealloc"

void	ModemTimerEntry_Dealloc( void *pvItem )
{
	TIMER_OPERATION_ENTRY	*pTimerEntry;


	DNASSERT( pvItem != NULL );

	//
	// initialize
	//
	pTimerEntry = static_cast<TIMER_OPERATION_ENTRY*>( pvItem );

	//
	// return associated poiner to write data
	//
	DNASSERT( pTimerEntry->Linkage.IsEmpty() != FALSE );
	DNASSERT( pTimerEntry->pContext == NULL );
}
//**********************************************************************



