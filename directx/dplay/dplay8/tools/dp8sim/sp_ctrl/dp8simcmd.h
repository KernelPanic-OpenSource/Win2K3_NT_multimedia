/***************************************************************************
 *
 *  Copyright (C) 2001 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dp8simcmd.h
 *
 *  Content:	Header for command object class.
 *
 *  History:
 *   Date      By        Reason
 *  ========  ========  =========
 *  04/23/01  VanceO    Created.
 *
 ***************************************************************************/




//=============================================================================
// Defines
//=============================================================================
#define CMDTYPE_SENDDATA_IMMEDIATE	1	// command represents a send transmitted right away
#define CMDTYPE_SENDDATA_DELAYED	2	// command represents a send that was artificially delayed
#define CMDTYPE_CONNECT				3	// command represents a connect
#define CMDTYPE_DISCONNECT			4	// command represents a disconnect
#define CMDTYPE_LISTEN				5	// command represents a listen
#define CMDTYPE_ENUMQUERY			6	// command represents an enum query
#define CMDTYPE_ENUMRESPOND			7	// command represents an enum respond




//=============================================================================
// Structures
//=============================================================================
typedef struct _DP8SIMCOMMAND_FPMCONTEXT
{
	DWORD	dwType;			// type of command
	PVOID	pvUserContext;	// user context for command
} DP8SIMCOMMAND_FPMCONTEXT, * PDP8SIMCOMMAND_FPMCONTEXT;






//=============================================================================
// Send object class
//=============================================================================
class CDP8SimCommand
{
	public:

		inline BOOL IsValidObject(void)
		{
			if ((this == NULL) || (IsBadWritePtr(this, sizeof(CDP8SimCommand))))
			{
				return FALSE;
			}

			if (*((DWORD*) (&this->m_Sig)) != 0x434d4953)	// 0x43 0x4d 0x49 0x53 = 'CMIS' = 'SIMC' in Intel order
			{
				return FALSE;
			}

			return TRUE;
		};




		static BOOL FPMAlloc(void* pvItem, void* pvContext)
		{
			CDP8SimCommand *	pDP8SimCommand = (CDP8SimCommand*) pvItem;


			pDP8SimCommand->m_Sig[0] = 'S';
			pDP8SimCommand->m_Sig[1] = 'I';
			pDP8SimCommand->m_Sig[2] = 'M';
			pDP8SimCommand->m_Sig[3] = 'c';	// start with lower case so we can tell when it's in the pool or not

			pDP8SimCommand->m_lRefCount				= 0;
			pDP8SimCommand->m_dwType				= 0;
			pDP8SimCommand->m_pvUserContext			= NULL;
			pDP8SimCommand->m_hCommand				= NULL;
			pDP8SimCommand->m_dwCommandDescriptor	= 0;
			ZeroMemory(&pDP8SimCommand->m_CommandSpecificData, sizeof(pDP8SimCommand->m_CommandSpecificData));

			return TRUE;
		}


#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::FPMInitialize"
		static void FPMInitialize(void* pvItem, void* pvContext)
		{
			CDP8SimCommand *			pDP8SimCommand = (CDP8SimCommand*) pvItem;
			DP8SIMCOMMAND_FPMCONTEXT *	pContext = (DP8SIMCOMMAND_FPMCONTEXT*) pvContext;


			pDP8SimCommand->m_lRefCount++;	// somebody is getting a pointer to this object
			DNASSERT(pDP8SimCommand->m_lRefCount == 1);


			pDP8SimCommand->m_dwType			= pContext->dwType;
			pDP8SimCommand->m_pvUserContext		= pContext->pvUserContext;

			
			//
			// Change the signature before handing it out.
			//
			pDP8SimCommand->m_Sig[3]	= 'C';
		}


#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::FPMRelease"
		static void FPMRelease(void* pvItem)
		{
			CDP8SimCommand *	pDP8SimCommand = (CDP8SimCommand*) pvItem;


			DNASSERT(pDP8SimCommand->m_lRefCount == 0);
			DNASSERT(pDP8SimCommand->m_CommandSpecificData.m_pDP8SimEndpointListen == NULL);


			//
			// Change the signature before putting the object back in the pool.
			//
			pDP8SimCommand->m_Sig[3]	= 'c';
		}


#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::FPMDealloc"
		static void FPMDealloc(void* pvItem)
		{
			const CDP8SimCommand *	pDP8SimCommand = (CDP8SimCommand*) pvItem;


			DNASSERT(pDP8SimCommand->m_lRefCount == 0);
			DNASSERT(pDP8SimCommand->m_CommandSpecificData.m_pDP8SimEndpointListen == NULL);
		}




#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::AddRef"
		inline void AddRef(void)
		{
			LONG	lResult;


			lResult = InterlockedIncrement(&this->m_lRefCount);
			DNASSERT(lResult > 0);
			DPFX(DPFPREP, 9, "Command 0x%p refcount = %u.", this, lResult);
		};

#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::Release"
		inline void Release(void)
		{
			LONG	lResult;


			lResult = InterlockedDecrement(&this->m_lRefCount);
			DNASSERT(lResult >= 0);
			if (lResult == 0)
			{
				DPFX(DPFPREP, 9, "Command 0x%p refcount = 0, returning to pool.", this);

				//
				// Time to return this object to the pool.
				//
				g_FPOOLCommand.Release(this);
			}
			else
			{
				DPFX(DPFPREP, 9, "Command 0x%p refcount = %u.", this, lResult);
			}
		};


		inline DWORD GetType(void) const							{ return this->m_dwType; };
		inline PVOID GetUserContext(void)					{ return this->m_pvUserContext; };
		inline HANDLE GetRealSPCommand(void) const				{ return this->m_hCommand; };
		inline DWORD GetRealSPCommandDescriptor(void) const		{ return this->m_dwCommandDescriptor; };

#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::GetListenEndpoint"
		inline CDP8SimEndpoint * GetListenEndpoint(void)
		{
			DNASSERT(this->m_dwType == CMDTYPE_LISTEN);
			return this->m_CommandSpecificData.m_pDP8SimEndpointListen;
		};

#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::GetMessageSize"
		inline DWORD GetMessageSize(void)
		{
			DNASSERT(this->m_dwType == CMDTYPE_SENDDATA_IMMEDIATE);
			return this->m_CommandSpecificData.m_dwMessageSize;
		};



		inline void SetRealSPCommand(HANDLE hCommand, DWORD dwCommandDescriptor)
		{
			this->m_hCommand				= hCommand;
			this->m_dwCommandDescriptor		= dwCommandDescriptor;
		};

#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::SetListenEndpoint"
		inline void SetListenEndpoint(CDP8SimEndpoint * const pDP8SimEndpoint)
		{
			DNASSERT(this->m_dwType == CMDTYPE_LISTEN);
			DNASSERT((this->m_CommandSpecificData.m_pDP8SimEndpointListen == NULL) || (pDP8SimEndpoint == NULL));

			//
			// Note this only sets the pointer, it is the caller's
			// responsibility to add or remove the reference as necessary.
			//
			this->m_CommandSpecificData.m_pDP8SimEndpointListen = pDP8SimEndpoint;
		};

#undef DPF_MODNAME
#define DPF_MODNAME "CDP8SimCommand::SetMessageSize"
		inline void SetMessageSize(const DWORD dwMessageSize)
		{
			DNASSERT(this->m_dwType == CMDTYPE_SENDDATA_IMMEDIATE);
			DNASSERT((this->m_CommandSpecificData.m_dwMessageSize == 0) || (dwMessageSize == 0));
			this->m_CommandSpecificData.m_dwMessageSize = dwMessageSize;
		};


	
	private:
		BYTE				m_Sig[4];						// debugging signature ('SIMC')
		LONG				m_lRefCount;					// number of references for this object
		DWORD				m_dwType;						// type of command
		PVOID				m_pvUserContext;				// user's context for command
		HANDLE				m_hCommand;						// real SP command handle
		DWORD				m_dwCommandDescriptor;			// real SP descriptor for command
		union
		{
			CDP8SimEndpoint *	m_pDP8SimEndpointListen;		// pointer to listen endpoint, if this is a CMDTYPE_LISTEN command
			DWORD				m_dwMessageSize;				// size of message, if this is a CMDTYPE_SENDDATA_IMMEDIATE command
		} m_CommandSpecificData;
};

