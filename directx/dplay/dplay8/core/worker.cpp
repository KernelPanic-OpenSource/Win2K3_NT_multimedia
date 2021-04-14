/*==========================================================================
 *
 *  Copyright (C) 1999-2002 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       worker.cpp
 *  Content:    DNET worker thread routines
 *@@BEGIN_MSINTERNAL
 *  History:
 *   Date       By      Reason
 *   ====       ==      ======
 *  11/01/99	mjn		Created
 *  12/23/99	mjn		Hand all NameTable update sends from Host to worker thread
 *  12/23/99	mjn		Added SendHostMigration functionality
 *	12/28/99	mjn		Moved Async Op stuff to Async.h
 *	01/06/00	mjn		Moved NameTable stuff to NameTable.h
 *	01/09/00	mjn		Send Connect Info rather than just NameTable at connect
 *	01/10/00	mjn		Added SendUpdateApplicationDesc functionality
 *	01/15/00	mjn		Replaced DN_COUNT_BUFFER with CRefCountBuffer
 *	01/16/00	mjn		Removed user notification jobs
 *	01/23/00	mjn		Implemented TerminateSession
 *	01/24/00	mjn		Added support for NameTable operation list cleanup
 *	01/27/00	mjn		Added support for retention of receive buffers
 *	04/04/00	mjn		Added DNWTSendTerminateSession
 *	04/10/00	mjn		Added DNWTRemoveServiceProvider
 *	04/13/00	mjn		Internal sends use new Protocol Interface VTBL functions
 *				mjn		Internal sends contain dwFlags field
 *	04/16/00	mjn		DNSendMessage uses CAsyncOp
 *	04/17/00	mjn		Replaced BUFFERDESC with DPN_BUFFER_DESC
 *	04/19/00	mjn		Added support to send NameTable operations directly
 *	04/23/00	mjn		Added parameter to DNPerformChildSend()
 *	05/03/00	mjn		Use GetHostPlayerRef() rather than GetHostPlayer()
 *	05/05/00	mjn		Use GetConnectionRef() to send NameTable operations
 *	05/10/00	mjn		Ensure valid local player in DNWTProcessSend()
 *	06/07/00	mjn		Pull assert in send failure case (it was handled)
 *	06/21/00	mjn		Added support to install the NameTable (from Host)
 *	06/22/00	mjn		Fixed DNWTProcessSend() to properly handle voice messages
 *	07/06/00	mjn		Use SP handle instead of interface
 *	07/21/00	mjn		RefCount cleanup
 *	07/30/00	mjn		Added DN_WORKER_JOB_TERMINATE_SESSION
 *	08/02/00	mjn		Changed DNWTProcessSend() to pass voice messages to DNReceiveUserData()
 *				mjn		Added DN_WORKER_JOB_ALTERNATE_SEND
 *  08/05/00    RichGr  IA64: Use %p format specifier in DPFs for 32/64-bit pointers and handles.
 *	08/05/00	mjn		Added pParent to DNSendGroupMessage and DNSendMessage()
 *				mjn		Added fInternal to DNPerformChildSend()
 *	08/06/00	mjn		Added CWorkerJob
 *				mjn		Added DNQueueWorkerJob()
 *				mjn		Made DNWTSendNameTableOperation() more robust
 *	08/07/00	mjn		Removed COM_CoInitialize() and COM_CoUninitialize() calls in DNWorkerThreadProc()
 *	08/08/00	mjn		Added WORKER_JOB_PERFORM_LISTEN,DNWTPerformListen()
 *	03/30/01	mjn		Changes to prevent multiple loading/unloading of SP's
 *	06/06/01	mjn		Added back in COM_CoInitialize() and COM_CoUninitialize() calls to DNWorkerThreadProc()
 *@@END_MSINTERNAL
 *
 ***************************************************************************/

#include "dncorei.h"


#undef DPF_MODNAME
#define DPF_MODNAME "DNGenericWorkerCallback"

void WINAPI DNGenericWorkerCallback(void *const pvContext,
							void *const pvTimerData,
							const UINT uiTimerUnique)
{
	DIRECTNETOBJECT		*pdnObject;
	CWorkerJob			*pWorkerJob;


#ifdef DPNBUILD_NONSEQUENTIALWORKERQUEUE
	pWorkerJob = (CWorkerJob*)pvContext;
	pdnObject = pWorkerJob->GetDNObject();
#else // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE
	BOOL				fOwnProcessingOfJobs;
	CBilink				blDelayedJobs;

	pdnObject = (DIRECTNETOBJECT*)pvContext;
	blDelayedJobs.Initialize();

	//
	//	Process all jobs on queue unless another thread is already doing it.
	//
	fOwnProcessingOfJobs = FALSE;
	do
	{
		pWorkerJob = NULL;
		DNEnterCriticalSection(&pdnObject->csWorkerQueue);
		if (pdnObject->m_bilinkWorkerJobs.GetNext() != &pdnObject->m_bilinkWorkerJobs)
		{
			//
			//	If someone else is processing worker jobs already, don't do anything. 
			//
			if ((fOwnProcessingOfJobs) || (! pdnObject->fProcessingWorkerJobs))
			{
				pdnObject->fProcessingWorkerJobs = TRUE;
				fOwnProcessingOfJobs = TRUE;
				pWorkerJob = CONTAINING_OBJECT(pdnObject->m_bilinkWorkerJobs.GetNext(),CWorkerJob,m_bilinkWorkerJobs);
				pWorkerJob->m_bilinkWorkerJobs.RemoveFromList();
			}
			else
			{
				DPFX(DPFPREP, 5, "Another thread is already processing worker jobs.");
			}
		}
		else
		{
			//
			//	Give up ownership of job processing.
			//
			if (fOwnProcessingOfJobs)
			{
				DPFX(DPFPREP, 5, "No more jobs.");
				pdnObject->fProcessingWorkerJobs = FALSE;
				fOwnProcessingOfJobs = FALSE;
			}
			else
			{
				DPFX(DPFPREP, 5, "No jobs to process.");
			}
		}
		DNLeaveCriticalSection(&pdnObject->csWorkerQueue);

		if (pWorkerJob == NULL)
		{
			break;
		}
#endif // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE

		DPFX(DPFPREP, 5,"Processing job 0x%p id [0x%lx]",pWorkerJob,pWorkerJob->GetJobType());
		switch(pWorkerJob->GetJobType())
		{
			case WORKER_JOB_INSTALL_NAMETABLE:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_INSTALL_NAMETABLE");
					DNWTInstallNameTable(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_INTERNAL_SEND:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_INTERNAL_SEND");
					DNWTProcessSend(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_PERFORM_LISTEN:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_PERFORM_LISTEN");
					DNWTPerformListen(pdnObject,pWorkerJob);
					break;
				}
#if ((! defined(DPNBUILD_LIBINTERFACE)) || (! defined(DPNBUILD_ONLYONESP)))
			case WORKER_JOB_REMOVE_SERVICE_PROVIDER:
				{
					HRESULT		hr;
					DWORD		dwRecursionDepthAllowed;
					DWORD		dwRecursionDepth;


					//
					//	Before we perform the job, make sure we're not in a recursive DoWork,
					//	WaitWhileWorking, or SleepWhileWorking call.  If we are, then delay the
					//	job to avoid deadlocks.  We need service providers to be removed at
					//	the top level of processing.
					//	There is one exception, in DN_Close we expect to be waiting for service
					//	provider removal, so allow a single level of recursion.  We can tell when
					//	we're inside Close by our thread ID matching the one stored off the DN
					//	object.  Note that dwClosingThreadID will be 0 until we enter Close.
					//
					DNEnterCriticalSection(&pdnObject->csDirectNetObject);
					if (pdnObject->dwClosingThreadID == GetCurrentThreadId())
					{
						DNASSERT(pdnObject->dwFlags & DN_OBJECT_FLAG_CLOSING);
						dwRecursionDepthAllowed = 1;
					}
					else
					{
						DNASSERT((pdnObject->dwClosingThreadID == 0) || (pdnObject->dwFlags & DN_OBJECT_FLAG_CLOSING));
						dwRecursionDepthAllowed = 0;
					}
					DNLeaveCriticalSection(&pdnObject->csDirectNetObject);
					
					hr = IDirectPlay8ThreadPoolWork_GetWorkRecursionDepth(pdnObject->pIDPThreadPoolWork,
																		&dwRecursionDepth,
																		0);
					DNASSERT(hr == DPN_OK);
					if (dwRecursionDepth > dwRecursionDepthAllowed)
					{
						DPFX(DPFPREP, 1,"Re-queueing WORKER_JOB_REMOVE_SERVICE_PROVIDER 0x%p because this thread is recursively 'Work'ing (depth = %u).",
							pWorkerJob, dwRecursionDepth);
#ifdef DPNBUILD_NONSEQUENTIALWORKERQUEUE
						DNQueueWorkerJob(pdnObject, pWorkerJob);
#else // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE
						pWorkerJob->m_bilinkWorkerJobs.InsertBefore(&blDelayedJobs);
#endif // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE

						//
						//	Forget about the job so it's not returned to the pool below.
						//
						pWorkerJob = NULL;
					}
					else
					{
						DPFX(DPFPREP, 5,"Job: WORKER_JOB_REMOVE_SERVICE_PROVIDER 0x%p (work recursion depth = %u)",
							pWorkerJob, dwRecursionDepth);
						DNWTRemoveServiceProvider(pdnObject,pWorkerJob);
					}
					break;
				}
#endif // ! DPNBUILD_LIBINTERFACE or ! DPNBUILD_ONLYONESP
			case WORKER_JOB_SEND_NAMETABLE_OPERATION:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_SEND_NAMETABLE_OPERATION");
					DNWTSendNameTableOperation(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_SEND_NAMETABLE_OPERATION_CLIENT:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_SEND_NAMETABLE_OPERATION_CLIENT");
					DNWTSendNameTableOperationClient(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_SEND_NAMETABLE_VERSION:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_SEND_NAMETABLE_VERSION");
					DNWTSendNameTableVersion(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_TERMINATE_SESSION:
				{
					DPFX(DPFPREP, 5,"Job: WORKER_JOB_TERMINATE_SESSION");
					DNWTTerminateSession(pdnObject,pWorkerJob);
					break;
				}
			case WORKER_JOB_UNKNOWN:
			default:
				{
					DPFERR("Unknown Job !");
					DNASSERT(FALSE);
					break;
				}
		}

		//
		//	Return this job to the pool (clean up is automatic)
		//
		if (pWorkerJob != NULL)
		{
			pWorkerJob->ReturnSelfToPool();
			pWorkerJob = NULL;
		}

#ifndef DPNBUILD_NONSEQUENTIALWORKERQUEUE
	}
	while (TRUE);

	//
	//	If we delayed any jobs, requeue them all now.
	//
	if (! blDelayedJobs.IsEmpty())
	{
		HRESULT		hResultCode;
		DWORD		dwMinRequeueCount;
		DWORD		dwRequeueCount;

		dwMinRequeueCount = 1000;	// wait at most 1 second
		DNEnterCriticalSection(&pdnObject->csWorkerQueue);
		do
		{
			pWorkerJob = CONTAINING_OBJECT(blDelayedJobs.GetNext(),CWorkerJob,m_bilinkWorkerJobs);
			pWorkerJob->m_bilinkWorkerJobs.RemoveFromList();
			dwRequeueCount = pWorkerJob->IncRequeueCount();
			if (dwRequeueCount < dwMinRequeueCount)
			{
				dwMinRequeueCount = dwRequeueCount;
			}
			pWorkerJob->m_bilinkWorkerJobs.InsertBefore(&pdnObject->m_bilinkWorkerJobs);
		}
		while (! blDelayedJobs.IsEmpty());
		DNLeaveCriticalSection(&pdnObject->csWorkerQueue);

		//
		// If we have an item we've never requeued yet, submit a work item to handle it
		// immediately.  Otherwise set a timer to handle the jobs in the future to prevent
		// us from picking up the same item over and over again because we're waiting on
		// the connection to drop, for example.
		//
#pragma TODO(vanceo, "Possibly select a CPU?")
		if (dwMinRequeueCount == 0)
		{
			DPFX(DPFPREP, 7,"Queuing work item to handle jobs");
			hResultCode = IDirectPlay8ThreadPoolWork_QueueWorkItem(pdnObject->pIDPThreadPoolWork,	// interface
																	-1,								// use any CPU
																	DNGenericWorkerCallback,		// callback
																	pdnObject,						// context
																	0);								// flags
		}
		else
		{
			PVOID	pvTimerDataIgnored;
			UINT	uiTimerUniqueIgnored;

			DPFX(DPFPREP, 7,"Scheduling timer to handle jobs");
			hResultCode = IDirectPlay8ThreadPoolWork_ScheduleTimer(pdnObject->pIDPThreadPoolWork,	// interface
																	-1,								// use any CPU
																	dwMinRequeueCount,				// use the requeue count as the delay in ms
																	DNGenericWorkerCallback,		// callback
																	pdnObject,						// context
																	&pvTimerDataIgnored,			// timer handle
																	&uiTimerUniqueIgnored,			// timer uniqueness
																	0);								// flags

			//
			// Ignore the timer handle since we will never attempt to cancel it.
			// Treat it as a really slow queued work item.
			//
		}
		DNASSERT(hResultCode == DPN_OK);

		//
		// Keep the threadpool object reference for the new work item/timer.
		//
	}
	else
	{
		//
		//	Release the threadpool object reference.
		//
		DNThreadPoolRelease(pdnObject);
	}
#endif // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE
}


//	DNQueueWorkerJob
//
//	Add a worker thread job to the end of the job queue, and signal the queue to run

#undef DPF_MODNAME
#define DPF_MODNAME "DNQueueWorkerJob"

void DNQueueWorkerJob(DIRECTNETOBJECT *const pdnObject,
					  CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;
	PVOID		pvContext;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p] (type=0x%x)",pWorkerJob,pWorkerJob->GetJobType());

	DNASSERT(pdnObject != NULL);
	DNASSERT(pWorkerJob != NULL);

#ifdef DPNBUILD_NONSEQUENTIALWORKERQUEUE
	pvContext = pWorkerJob;
#else // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE
	DNEnterCriticalSection(&pdnObject->csWorkerQueue);
	pWorkerJob->m_bilinkWorkerJobs.InsertBefore(&pdnObject->m_bilinkWorkerJobs);
	DNLeaveCriticalSection(&pdnObject->csWorkerQueue);

	pvContext = pdnObject;

	//
	//	Add a reference on the DNet object to prevent the threadpool from unloading
	//	before all of the scheduled work items have executed.
	//
	DNThreadPoolAddRef(pdnObject);
#endif // ! DPNBUILD_NONSEQUENTIALWORKERQUEUE


#pragma TODO(vanceo, "Possibly select a CPU?")

	hResultCode = IDirectPlay8ThreadPoolWork_QueueWorkItem(pdnObject->pIDPThreadPoolWork,	// interface
															-1,								// use any CPU
															DNGenericWorkerCallback,		// callback
															pvContext,						// context
															0);								// flags
	DNASSERT(hResultCode == DPN_OK);

	DPFX(DPFPREP, 6,"Returning");
}



//	DNWTSendInternal
//
//	Send an internal message. This will copy the message buffer and place the operation on the
//	working thread job queue

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTSendInternal"

HRESULT DNWTSendInternal(DIRECTNETOBJECT *const pdnObject,
						 CAsyncOp *const pAsyncOp)
{
	HRESULT				hResultCode;
	DWORD				dw;
	DWORD				dwSize;
	CWorkerJob			*pWorkerJob;
	CRefCountBuffer		*pRefCountBuffer;
	DN_SEND_OP_DATA		*pSendOpData;

	DPFX(DPFPREP, 6,"Parameters: pAsyncOp [0x%p]",pAsyncOp);

	pRefCountBuffer = NULL;
	pWorkerJob = NULL;
	pSendOpData = NULL;

	//
	//	Create local buffer
	//
	if (pAsyncOp->IsUseParentOpData())
	{
		if (pAsyncOp->IsChild() && pAsyncOp->GetParent())
		{
			pSendOpData = pAsyncOp->GetParent()->GetLocalSendOpData();
		}
	}
	else
	{
		pSendOpData = pAsyncOp->GetLocalSendOpData();
	}
	if (pSendOpData == NULL)
	{
		hResultCode = DPNERR_GENERIC;
		goto Failure;
	}

	dwSize = 0;
	for ( dw = 0 ; dw < pSendOpData->dwNumBuffers ; dw++ )
	{
		dwSize += pSendOpData->BufferDesc[dw].dwBufferSize;
	}

	DPFX(DPFPREP, 7,"Allocating RefCount Buffer of [%ld] bytes",dwSize);
	if ((hResultCode = RefCountBufferNew(pdnObject,dwSize,MemoryBlockAlloc,MemoryBlockFree,&pRefCountBuffer)) != DPN_OK)
	{
		DPFERR("Could not allocate space for local buffer");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}

	//
	//	Copy from scatter-gather buffers
	//
	dwSize = 0;
	for ( dw = 0 ; dw < pSendOpData->dwNumBuffers ; dw++ )
	{
		memcpy(	pRefCountBuffer->GetBufferAddress() + dwSize,
				pSendOpData->BufferDesc[dw].pBufferData,
				pSendOpData->BufferDesc[dw].dwBufferSize );
		dwSize += pSendOpData->BufferDesc[dw].dwBufferSize;
	}

	//	TODO - user context value of send ?
	DPFX(DPFPREP, 7,"Adding Internal Send to Job Queue");
	if ((hResultCode = WorkerJobNew(pdnObject,&pWorkerJob)) != DPN_OK)
	{
		DPFERR("Could not create worker job");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}

	pWorkerJob->SetJobType( WORKER_JOB_INTERNAL_SEND );
	pWorkerJob->SetInternalSendFlags( pAsyncOp->GetOpFlags() );
	pWorkerJob->SetRefCountBuffer( pRefCountBuffer );

	DNQueueWorkerJob(pdnObject,pWorkerJob);
	pWorkerJob = NULL;

	//
	//	Invoke send completion handler
	//
	DNPICompleteSend(	pdnObject,
						static_cast<void*>(pAsyncOp),
						DPN_OK,
						0,
						0);
	
	hResultCode = DPNERR_PENDING;

	pRefCountBuffer->Release();
	pRefCountBuffer = NULL;

Exit:
	DPFX(DPFPREP, 6,"Returning: [0x%lx]",hResultCode);
	return(hResultCode);

Failure:
	if (pRefCountBuffer)
	{
		pRefCountBuffer->Release();
		pRefCountBuffer = NULL;
	}
	goto Exit;
}


//	DNWTProcessSend
//
//	Process an internal send message.  This will indicate a received user message, or process a received internal one.

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTProcessSend"

HRESULT DNWTProcessSend(DIRECTNETOBJECT *const pdnObject,
						CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;
	void		*pvData;
	DWORD		dwDataSize;
	void		*pvInternalData;
	DWORD		dwInternalDataSize;
	DWORD		*pdwMsgId;
	CNameTableEntry	*pLocalPlayer;
	CConnection		*pConnection;
	CRefCountBuffer	*pRefCountBuffer;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	pLocalPlayer = NULL;
	pConnection = NULL;
	pRefCountBuffer = NULL;

	//
	//	Extract and clear RefCountBuffer from job
	//
	DNASSERT(pWorkerJob->GetRefCountBuffer() != NULL);
	pRefCountBuffer = pWorkerJob->GetRefCountBuffer();
	pWorkerJob->SetRefCountBuffer( NULL );

	//
	//	Get local player's connection (if still valid)
	//
	if ((hResultCode = pdnObject->NameTable.GetLocalPlayerRef( &pLocalPlayer )) != DPN_OK)
	{
		DPFERR("Local player not in NameTable (shutting down ?)");
		DisplayDNError(0,hResultCode);
		hResultCode = DPN_OK;
		goto Failure;
	}
	if ((hResultCode = pLocalPlayer->GetConnectionRef( &pConnection )) != DPN_OK)
	{
		DPFERR("Local player connection is not valid (shutting down?)");
		DisplayDNError(0,hResultCode);
		hResultCode = DPN_OK;
		goto Failure;
	}
	pLocalPlayer->Release();
	pLocalPlayer = NULL;

	DNASSERT(pRefCountBuffer->GetBufferAddress() != NULL);
	pvData = pRefCountBuffer->GetBufferAddress();
	dwDataSize = pRefCountBuffer->GetBufferSize();

	if ((pWorkerJob->GetInternalSendFlags() & DN_SENDFLAGS_SET_USER_FLAG)
			&& !(pWorkerJob->GetInternalSendFlags() & DN_SENDFLAGS_SET_USER_FLAG_TWO))
	{
		//
		//	Internal message
		//
		DPFX(DPFPREP, 7,"Received INTERNAL message");

		// Extract internal message
		DNASSERT(dwDataSize >= sizeof(DWORD));
		pdwMsgId = static_cast<DWORD*>(pvData);
		dwInternalDataSize = dwDataSize - sizeof(DWORD);
		if (dwInternalDataSize > 0)
		{
			pvInternalData = static_cast<void*>(static_cast<BYTE*>(pvData) + sizeof(DWORD));
		}
		else
		{
			pvInternalData = NULL;
		}

		// Process internal message
		hResultCode = DNProcessInternalOperation(	pdnObject,
													*pdwMsgId,
													pvInternalData,
													dwInternalDataSize,
													pConnection,
													NULL,
													pRefCountBuffer);
	}
	else
	{
		//
		//	User or voice message
		//
		DPFX(DPFPREP, 7,"Received USER or Voice message");

		hResultCode = DNReceiveUserData(pdnObject,
										pConnection,
										static_cast<BYTE*>(pvData),
										dwDataSize,
										NULL,
										pRefCountBuffer,
										0,
										pWorkerJob->GetInternalSendFlags());
	}

	//
	//	Clean up
	//
	pRefCountBuffer->Release();
	pRefCountBuffer = NULL;

	pConnection->Release();
	pConnection = NULL;

Exit:
	DPFX(DPFPREP, 6,"Returning: [0x%lx]",hResultCode);
	return(hResultCode);

Failure:
	if (pLocalPlayer)
	{
		pLocalPlayer->Release();
		pLocalPlayer = NULL;
	}
	if (pConnection)
	{
		pConnection->Release();
		pConnection = NULL;
	}
	if (pRefCountBuffer)
	{
		pRefCountBuffer->Release();
		pRefCountBuffer = NULL;
	}
	goto Exit;
}


//	DNWTTerminateSession
//
//	Disconnect the Local player from the session

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTTerminateSession"

HRESULT	DNWTTerminateSession(DIRECTNETOBJECT *const pdnObject,
							 CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	// Terminate session
	hResultCode = DNTerminateSession(pdnObject,pWorkerJob->GetTerminateSessionReason());

	DPFX(DPFPREP, 6,"Returning: [0x%lx]",hResultCode);
	return(hResultCode);
}


//	DNWTSendNameTableVersion
//
//	Send NAMETABLE_VERSION message to the host player

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTSendNameTableVersion"

HRESULT DNWTSendNameTableVersion(DIRECTNETOBJECT *const pdnObject,
								 CWorkerJob *const pWorkerJob)
{
	HRESULT			hResultCode;
	CRefCountBuffer	*pRefCountBuffer;
	CNameTableEntry	*pHostPlayer;
	CConnection		*pConnection;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	pRefCountBuffer = NULL;
	pHostPlayer = NULL;
	pConnection = NULL;

	//
	//	Extract and clear RefCountBuffer from job
	//
	DNASSERT(pWorkerJob->GetRefCountBuffer() != NULL);
	pRefCountBuffer = pWorkerJob->GetRefCountBuffer();
	pWorkerJob->SetRefCountBuffer( NULL );

	// Send message to host player
	if ((hResultCode = pdnObject->NameTable.GetHostPlayerRef( &pHostPlayer )) != DPN_OK)
	{
		DPFERR("Could not find Host player");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}
	if ((hResultCode = pHostPlayer->GetConnectionRef( &pConnection )) != DPN_OK)
	{
		DPFERR("Could not get Connection reference");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}
	hResultCode = DNSendMessage(pdnObject,
								pConnection,
								DN_MSG_INTERNAL_NAMETABLE_VERSION,
								pHostPlayer->GetDPNID(),
								pRefCountBuffer->BufferDescAddress(),
								1,
								pRefCountBuffer,
								0,
								DN_SENDFLAGS_RELIABLE,
								NULL,
								NULL);
	if (hResultCode != DPNERR_PENDING)
	{
		DPFERR("Could not send message to Host player");
		DisplayDNError(0,hResultCode);
		DNASSERT(hResultCode != DPN_OK);	// it was sent guaranteed, it should not return immediately
		goto Failure;
	}
	pConnection->Release();
	pConnection = NULL;
	pHostPlayer->Release();
	pHostPlayer = NULL;
	pRefCountBuffer->Release();
	pRefCountBuffer = NULL;

Exit:
	DPFX(DPFPREP, 6,"Returning: [0x%lx]",hResultCode);
	return(hResultCode);

Failure:
	if (pRefCountBuffer)
	{
		pRefCountBuffer->Release();
		pRefCountBuffer = NULL;
	}
	if (pHostPlayer)
	{
		pHostPlayer->Release();
		pHostPlayer = NULL;
	}
	if (pConnection)
	{
		pConnection->Release();
		pConnection = NULL;
	}
	goto Exit;
}


#if ((! defined(DPNBUILD_LIBINTERFACE)) || (! defined(DPNBUILD_ONLYONESP)))

//	DNWTRemoveServiceProvider
//
//	Remove a ServiceProvider from the Protocol

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTRemoveServiceProvider"

HRESULT DNWTRemoveServiceProvider(DIRECTNETOBJECT *const pdnObject,
								  CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	DNASSERT(pWorkerJob->GetRemoveServiceProviderHandle() != NULL);

	hResultCode = DNPRemoveServiceProvider(pdnObject->pdnProtocolData,pWorkerJob->GetRemoveServiceProviderHandle());

	DNProtocolRelease(pdnObject);

	DPFX(DPFPREP, 6,"Returning: [0x%lx]",hResultCode);
	return(hResultCode);
}

#endif // ! DPNBUILD_LIBINTERFACE or ! DPNBUILD_ONLYONESP


//	DNWTSendNameTableOperation
//
//	Send a NameTable operation to each connected player
//	This is based on the version number supplied and an excluded DPNID

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTSendNameTableOperation"

void DNWTSendNameTableOperation(DIRECTNETOBJECT *const pdnObject,
								CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;
	CAsyncOp	*pParent;
	CBilink		*pBilink;
	CNameTableEntry	*pNTEntry;
	DWORD		dwMsgId;
	DWORD		dwVersion;
	DPNID		dpnidExclude;
	DWORD		dwCount;
	DWORD		dwActual;
	DWORD		dw;
	CConnection **TargetList;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	DNASSERT(pdnObject != NULL);
	DNASSERT(pWorkerJob != NULL);

	pParent = NULL;
	pNTEntry = NULL;
	TargetList = NULL;

	dwMsgId = pWorkerJob->GetSendNameTableOperationMsgId();
	dwVersion = pWorkerJob->GetSendNameTableOperationVersion();
	dpnidExclude = pWorkerJob->GetSendNameTableOperationDPNIDExclude();

	hResultCode = DNCreateSendParent(	pdnObject,
										dwMsgId,
										pWorkerJob->GetRefCountBuffer()->BufferDescAddress(),
										1,
										DN_SENDFLAGS_RELIABLE | DN_SENDFLAGS_SET_USER_FLAG | DN_SENDFLAGS_COALESCE,
										&pParent);
	if (hResultCode != DPN_OK)
	{
		DPFERR("Could not create AsyncOp");
		DisplayDNError(0,hResultCode);
		DNASSERT(FALSE);
		goto Failure;
	}
	pParent->SetRefCountBuffer(pWorkerJob->GetRefCountBuffer());

	//
	//	Lock NameTable
	//
	pdnObject->NameTable.ReadLock();

	//
	//	Determine recipient list
	//
	dwCount = 0;
	dwActual = 0;
	pBilink = pdnObject->NameTable.m_bilinkPlayers.GetNext();
	while (pBilink != &pdnObject->NameTable.m_bilinkPlayers)
	{
		pNTEntry = CONTAINING_OBJECT(pBilink,CNameTableEntry,m_bilinkEntries);
		if (	   !pNTEntry->IsDisconnecting()
				&& !pNTEntry->IsLocal()
				&& ((dwVersion == 0) || (pNTEntry->GetVersion() < dwVersion))
				&& ((dpnidExclude == 0) || (pNTEntry->GetDPNID() != dpnidExclude))
			)
		{
			dwCount++;
		}
		pBilink = pBilink->GetNext();
	}
	DPFX(DPFPREP, 7,"Number of targets [%ld]",dwCount);

	//
	//	Create target list
	//
	if (dwCount > 0)
	{
		if ((TargetList = reinterpret_cast<CConnection**>(MemoryBlockAlloc(pdnObject,dwCount * sizeof(CConnection*)))) == NULL)
		{
			DPFERR("Could not create target list");
			DNASSERT(FALSE);
			goto Failure;
		}

		pBilink = pdnObject->NameTable.m_bilinkPlayers.GetNext();
		while (pBilink != &pdnObject->NameTable.m_bilinkPlayers)
		{
			pNTEntry = CONTAINING_OBJECT(pBilink,CNameTableEntry,m_bilinkEntries);
			if (	   !pNTEntry->IsDisconnecting()
					&& !pNTEntry->IsLocal()
					&& ((dwVersion == 0) || (pNTEntry->GetVersion() < dwVersion))
					&& ((dpnidExclude == 0) || (pNTEntry->GetDPNID() != dpnidExclude))
				)
			{
				DNASSERT(dwActual < dwCount);
				if ((hResultCode = pNTEntry->GetConnectionRef( &(TargetList[dwActual]) )) == DPN_OK)
				{
					dwActual++;
				}
			}
			pBilink = pBilink->GetNext();
		}
		DPFX(DPFPREP, 7,"Actual number of targets [%ld]",dwActual);
	}

	//
	//	Unlock NameTable
	//
	pdnObject->NameTable.Unlock();

	//
	//	Send to target list
	//
	if (TargetList)
	{
		for (dw = 0 ; dw < dwActual ; dw++)
		{
			DNASSERT(TargetList[dw] != NULL);

			hResultCode = DNPerformChildSend(	pdnObject,
												pParent,
												TargetList[dw],
												0,
												NULL,
												TRUE);
			if (hResultCode != DPNERR_PENDING)
			{
				DPFERR("Could not perform part of group send - ignore and continue");
				DisplayDNError(0,hResultCode);
				DNASSERT(hResultCode != DPN_OK);	// it was sent guaranteed, it should not return immediately
			}
			TargetList[dw]->Release();
			TargetList[dw] = NULL;
		}

		MemoryBlockFree(pdnObject,TargetList);
		TargetList = NULL;
	}

	pParent->Release();
	pParent = NULL;

Exit:
	DPFX(DPFPREP, 6,"Returning");
	return;

Failure:
	if (pParent)
	{
		pParent->Release();
		pParent = NULL;
	}
	if (TargetList)
	{
		MemoryBlockFree(pdnObject,TargetList);
		TargetList = NULL;
	}
	goto Exit;
}

//	DNWTSendNameTableOperationClient
//
//	Send a NameTable operation to a single client
//	This is based on the version number supplied and a DPNID

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTSendNameTableOperationClient"

void DNWTSendNameTableOperationClient(DIRECTNETOBJECT *const pdnObject,
									  CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;
	CAsyncOp	*pParent;
	CNameTableEntry	*pNTEntry;
	DWORD		dwMsgId;
	DWORD		dwVersion;
	DPNID		dpnid;
	CConnection *pConnection;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	DNASSERT(pdnObject != NULL);
	DNASSERT(pWorkerJob != NULL);

	pParent = NULL;
	pNTEntry = NULL;
	pConnection = NULL;

	dwMsgId = pWorkerJob->GetSendNameTableOperationMsgId();
	dwVersion = pWorkerJob->GetSendNameTableOperationVersion();
	dpnid = pWorkerJob->GetSendNameTableOperationDPNIDExclude();

	hResultCode = DNCreateSendParent(	pdnObject,
										dwMsgId,
										pWorkerJob->GetRefCountBuffer()->BufferDescAddress(),
										1,
										DN_SENDFLAGS_RELIABLE | DN_SENDFLAGS_SET_USER_FLAG | DN_SENDFLAGS_COALESCE,
										&pParent);
	if (hResultCode != DPN_OK)
	{
		DPFERR("Could not create AsyncOp");
		DisplayDNError(0,hResultCode);
		DNASSERT(FALSE);
		goto Failure;
	}
	pParent->SetRefCountBuffer(pWorkerJob->GetRefCountBuffer());

	//
	//	Lookup player
	//
	if ((hResultCode = pdnObject->NameTable.FindEntry(dpnid,&pNTEntry)) != DPN_OK)
	{
		DPFERR("Could not find player");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}

	if ((hResultCode = pNTEntry->GetConnectionRef( &pConnection )) != DPN_OK)
	{
		DPFERR("Could not get connection for player");
		DisplayDNError(0,hResultCode);
		goto Failure;
	}
	pNTEntry->Release();
	pNTEntry = NULL;

	hResultCode = DNPerformChildSend(	pdnObject,
										pParent,
										pConnection,
										0,
										NULL,
										TRUE);
	if (hResultCode != DPNERR_PENDING)
	{
		DPFERR("Could not perform send - ignore and continue");
		DisplayDNError(0,hResultCode);
		DNASSERT(hResultCode != DPN_OK);	// it was sent guaranteed, it should not return immediately
	}
	pConnection->Release();
	pConnection = NULL;

	pParent->Release();
	pParent = NULL;

Exit:
	DPFX(DPFPREP, 6,"Returning");
	return;

Failure:
	if (pParent)
	{
		pParent->Release();
		pParent = NULL;
	}
	if (pConnection)
	{
		pConnection->Release();
		pConnection = NULL;
	}
	if (pNTEntry)
	{
		pNTEntry->Release();
		pNTEntry = NULL;
	}
	goto Exit;
}

//	DNWTInstallNameTable
//
//	Install the Host sent NameTable and ApplicationDescription.  We will also be cracking open
//	LISTENs if required.  This has to be done on the worker thread due to the async->sync
//	behaviour of LISTEN.  We can't perform it on the SP's threads.

#undef DPF_MODNAME
#define DPF_MODNAME "DNWTInstallNameTable"

void DNWTInstallNameTable(DIRECTNETOBJECT *const pdnObject,
						  CWorkerJob *const pWorkerJob)
{
	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	DNASSERT(pdnObject != NULL);
	DNASSERT(pWorkerJob != NULL);

	DNASSERT(pWorkerJob->GetConnection() != NULL);
	DNASSERT(pWorkerJob->GetRefCountBuffer() != NULL);

	DNConnectToHost2(	pdnObject,
						pWorkerJob->GetRefCountBuffer()->GetBufferAddress(),
						pWorkerJob->GetConnection() );
}


#undef DPF_MODNAME
#define DPF_MODNAME "DNWTPerformListen"

void DNWTPerformListen(DIRECTNETOBJECT *const pdnObject,
					   CWorkerJob *const pWorkerJob)
{
	HRESULT		hResultCode;

	DPFX(DPFPREP, 6,"Parameters: pWorkerJob [0x%p]",pWorkerJob);

	DNASSERT(pdnObject != NULL);
	DNASSERT(pWorkerJob != NULL);

	DNASSERT(pWorkerJob->GetAddress() != NULL);
	DNASSERT(pWorkerJob->GetAsyncOp() != NULL);

	hResultCode = DNPerformListen(pdnObject,pWorkerJob->GetAddress(),pWorkerJob->GetAsyncOp());
}
