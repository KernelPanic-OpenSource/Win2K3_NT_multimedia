/*==========================================================================
 *
 *  Copyright (C) 1999, 2000 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       fwdserver.cpp
 *  Content:	Implements the forwarding server portion of the server class
 *
 *  History:
 *   Date		By		Reason
 *   ====		==		======
 *	11/01/2000	rodtoll	Split out from dvsereng.cpp
 *  02/28/2002	rodtoll WINBUG #549959 - SECURITY: DPVOICE: Voice server trusts client's target list
 *						- Update receive path to use server's copy of client target list when server controlled targetting enabled
 *
 ***************************************************************************/

#include "dxvoicepch.h"


#undef DPF_MODNAME
#define DPF_MODNAME "CDirectVoiceServerEngine::StartupMulticast"
//
// StartupMulticast
//
// This function is called to initialize the multicast portion of the server
// object.  
//
// Called By:
// - StartSession
//
// Locks Required:
// - None
//
HRESULT CDirectVoiceServerEngine::StartupMulticast()
{
	return DV_OK;
}

#undef DPF_MODNAME
#define DPF_MODNAME "CDirectVoiceServerEngine::ShutdownMulticast"
//
// ShutdownMulticast
//
// This function is called to shutdown the multicast portion of the server
// object.  
//
// Called By:
// - StartSession
//
// Locks Required:
// - None
//
HRESULT CDirectVoiceServerEngine::ShutdownMulticast()
{

	return DV_OK;
}

HRESULT CDirectVoiceServerEngine::HandleForwardingReceive( CVoicePlayer *pTargetPlayer, PDVPROTOCOLMSG_SPEECHWITHTARGET pdvSpeechWithtarget, DWORD dwSpeechSize, PBYTE pbSpeechData )
{
	HRESULT hr;
    PDVTRANSPORT_BUFFERDESC pBufferDesc;
	PDVPROTOCOLMSG_SPEECHWITHFROM pdvSpeechWithFrom;
	PVOID pvSendContext;

	DWORD dwTransmitSize = sizeof( DVPROTOCOLMSG_SPEECHWITHFROM ) + dwSpeechSize;

	DNASSERT( dwTransmitSize <= m_lpdvfCompressionInfo->dwFrameLength + sizeof( DVPROTOCOLMSG_SPEECHWITHFROM ) );

	pBufferDesc = GetTransmitBuffer( dwTransmitSize, &pvSendContext );

	if( pBufferDesc == NULL )
	{
		DPFX(DPFPREP,  DVF_ERRORLEVEL, "Error allocating memory" );
		pTargetPlayer->Release();
		return FALSE;
	}

	pdvSpeechWithFrom = (PDVPROTOCOLMSG_SPEECHWITHFROM) pBufferDesc->pBufferData;

	pdvSpeechWithFrom->dvHeader.dwType = DVMSGID_SPEECHWITHFROM;
	pdvSpeechWithFrom->dvHeader.bMsgNum = pdvSpeechWithtarget->dvHeader.bMsgNum;
	pdvSpeechWithFrom->dvHeader.bSeqNum = pdvSpeechWithtarget->dvHeader.bSeqNum;
	pdvSpeechWithFrom->dvidFrom = pTargetPlayer->GetPlayerID();

	memcpy( &pdvSpeechWithFrom[1], pbSpeechData, dwSpeechSize );

	// With server controlled targetting, ignore client specified target list and send to the server's current 
	// view of the client's list.  (Which is also the most accurate).  Prevents client hacks from overriding 
	// target lists.  
	if( m_dvSessionDesc.dwFlags & DVSESSION_SERVERCONTROLTARGET )
	{
		// Lock player so that the target list doesn't change before it's copied.  
		pTargetPlayer->Lock();

		hr = m_lpSessionTransport->SendToIDS( (PDVID) pTargetPlayer->GetTargetList(), pTargetPlayer->GetNumTargets(), pBufferDesc, pvSendContext, 0 );

		pTargetPlayer->UnLock();
	}
	else
	{
		hr = m_lpSessionTransport->SendToIDS( (PDVID) &pdvSpeechWithtarget[1], pdvSpeechWithtarget->dwNumTargets, pBufferDesc, pvSendContext, 0 );
	}

	if( hr == DVERR_PENDING )
	{
	    hr = DV_OK;
	}
	else if( FAILED( hr ) )
	{
		DPFX(DPFPREP,  DVF_ERRORLEVEL, "Failed sending to ID hr=0x%x", hr );
	}	

	return hr;
}
