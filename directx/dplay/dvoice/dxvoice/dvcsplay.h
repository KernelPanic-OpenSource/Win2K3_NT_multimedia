/*==========================================================================;
 *
 *  Copyright (C) 1999 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dvcsplay.h
 *  Content:    Declaration of CDVCSPlayer class
 *  History:
 *	Date   By  Reason
 *	============
 *	07/22/99	rodtoll		created
 *  10/29/99	rodtoll		Bug #113726 - Integrate Voxware Codecs, updating to use new
 *							pluggable codec architecture.  
 *  01/14/2000	rodtoll		Updated to support multiple targets
 *  03/27/2000	rodtoll		Updated to use new base class for player record
 *  03/28/2000  rodtoll     Updated to use new player class as base
 *              rodtoll     Moved a bunch of logic out of server into this class
 * 03/29/2000	rodtoll Bug #30753 - Added volatile to the class definition
 * 11/16/2000	rodtoll	Bug #40587 - DPVOICE: Mixing server needs to use multi-processors  
 * 09/05/2001  	simonpow	Bug #463972. Added constuct/destruct methods to enable 
 *							allocations and de-allocations via CFixedPool objects
 * 02/28/2002	rodtoll		WINBUG #549959 - SECURITY: DPVOICE: Voice server trusts client's target list
 *							- When server controlled targetting is enabled, use server's copy of client
 *							  target list instead of list specified in incoming packet.
 ***************************************************************************/

#ifndef __DVCSPLAYER_H
#define __DVCSPLAYER_H

#define DPV_TARGETBUFFER_REALLOC_SIZE		10

#undef DPF_SUBCOMP
#define DPF_SUBCOMP DN_SUBCOMP_VOICE

class CDVCSPlayer;

struct CBilinkPlusObject
{
	CDVCSPlayer* m_pPlayer;
	CBilink	     m_bl;
};

// CDVCSPlayer
//
// This class extends the CDVPlayer class for client/server servers.
//
volatile class CDVCSPlayer: public CVoicePlayer
{
public:

 		//called from CFixedPool class to to build/destroy CDVCSPlayer's
	static BOOL PoolAllocFunction(void * pvItem, void * pvContext);
	static void PoolDeallocFunction(void * pvItem);
	

    HRESULT Initialize( const DVID dvidPlayer, const DWORD dwHostOrder, DWORD dwFlags, 
                        PVOID pvContext, DWORD dwCompressedSize, DWORD dwUnCompressedSize,
						CFixedPool *pCSOwner, DWORD dwNumMixingThreads );
    HRESULT DeInitialize();

    HRESULT HandleMixingReceive( PDVPROTOCOLMSG_SPEECHHEADER pdvSpeechHeader, PBYTE pbData, DWORD dwSize, PDVID pdvidTargets, DWORD dwNumTargets, BOOL fServerTargetting );
	HRESULT CompressOutBound( PVOID pvInputBuffer, DWORD dwInputBufferSize, PVOID pvOutputBuffer, DWORD *pdwOutputSize );

	inline HRESULT ResizeIfRequired( DWORD dwThreadIndex, DWORD dwNewMaxSize )
	{
		if( dwNewMaxSize > m_pdwMaxCanHear[dwThreadIndex] )
		{
			m_pdwMaxCanHear[dwThreadIndex] = dwNewMaxSize+DPV_TARGETBUFFER_REALLOC_SIZE;

			if( m_pppCanHear[dwThreadIndex] )
			{
				delete [] (m_pppCanHear[dwThreadIndex]);
			}

			m_pppCanHear[dwThreadIndex] = new CDVCSPlayer*[m_pdwMaxCanHear[dwThreadIndex]];

			if( m_pppCanHear[dwThreadIndex] == NULL )
			{
				return DVERR_OUTOFMEMORY;
			}

		}

		return DV_OK;
	};


	#undef DPF_MODNAME
	#define DPF_MODNAME "CDVCSPlayer::CreateOutBoundConverter"
	inline HRESULT CreateOutBoundConverter( PWAVEFORMATEX pwfxSourceFormat, const GUID &guidCT  )
	{
		HRESULT hr;

		hr = DVCDB_CreateConverter( pwfxSourceFormat, guidCT, &m_lpOutBoundAudioConverter );

		if( FAILED( hr ) )
		{
			DPFX(DPFPREP,  0, "Error creating audio converter hr=0x%x" , hr );
			return hr;
		}

		return hr;
	}

	inline BOOL IsOutBoundConverterInitialized() const
	{
		return (m_lpOutBoundAudioConverter != NULL);
	}

	BOOL ComparePlayerMix( DWORD dwThreadIndex, const CDVCSPlayer *lpdvPlayer );

	void ResetForNextRun( DWORD dwThreadIndex, BOOL fDequeue );
	void CompleteRun( DWORD dwThreadIndex );

    inline void AddToMixingList( DWORD dwThreadIndex, CBilink *pblBilink )
    {
        m_pblMixingActivePlayers[dwThreadIndex].m_bl.InsertAfter( pblBilink );
    }

    inline void RemoveFromMixingList(DWORD dwThreadIndex)
    {
        m_pblMixingActivePlayers[dwThreadIndex].m_bl.RemoveFromList();
    }

    inline void AddToSpeakingList( DWORD dwThreadIndex, CBilink *pblBilink )
    {
        m_pblMixingSpeakingPlayers[dwThreadIndex].m_bl.InsertAfter( pblBilink );
    }

    inline void RemoveFromSpeakingList(DWORD dwThreadIndex)
    {
        m_pblMixingSpeakingPlayers[dwThreadIndex].m_bl.RemoveFromList();
    }


    inline void AddToHearingList( DWORD dwThreadIndex, CBilink *pblBilink )
    {
        m_pblMixingHearingPlayers[dwThreadIndex].m_bl.InsertAfter( pblBilink );
    }

    inline void RemoveFromHearingList(DWORD dwThreadIndex)
    {
        m_pblMixingHearingPlayers[dwThreadIndex].m_bl.RemoveFromList();
    }
	

public: // These variables are shared between mixing threads
	PDPVCOMPRESSOR		m_lpOutBoundAudioConverter;	

	BOOL				m_bLastSent;			// Was last frame sent to this user?
	BYTE				m_bMsgNum;				// Last msg # transmitted
	BYTE				m_bSeqNum;				// Last Sequence # transmitted
    DWORD				m_targetSize;			// Tmp to hold size of compressed data (bytes)	

	BOOL				m_lost;	// the queue detected that this frame was lost
	DWORD				m_dwNumMixingThreads;

public: // These variables are on a per/mixing thread basis

	CDVCSPlayer			**m_pReuseMixFromThisPlayer;
	BYTE				*m_pbMsgNumToSend;				// Last msg # transmitted
	BYTE				*m_pbSeqNumToSend;				// Last Sequence # transmitted
	CBilinkPlusObject	*m_pblMixingActivePlayers;	// Bilink of active players (per mixing thread)
	CBilinkPlusObject	*m_pblMixingSpeakingPlayers;	// Bilink of players speaking (per mixing thread)
	CBilinkPlusObject	*m_pblMixingHearingPlayers;		// Bilink of players hearing (per mixing thread)
    DWORD				*m_pdwHearCount;				// How many people can this user hear? (per mixing thread)
    BOOL				*m_pfDecompressed;			// Has the user's frame been decompressed (per mixing thread)
	BOOL				*m_pfSilence;				// Is the latest from from user silence? (per mixing thread)
	BOOL				*m_pfNeedsDecompression;	// Does this player need decompression (per mixing thread)
    CDVCSPlayer			***m_pppCanHear;			// Array of pointers to player records this player can hear (per mixing thread)
    DWORD				*m_pdwMaxCanHear;			// # of elements in the m_dwCanHead array (per mixing thread)
    CFrame				**m_pSourceFrame;			// Source frame (per mixing thread)
    BYTE				*m_sourceUnCompressed;		// Buffer to hold decompressed data (per mixing thread)
    BYTE				*m_targetCompressed;		// Has user's mix been created yet? (per mixing thread)
    BOOL				*m_pfMixed;					// Has the user's output been readied? (per mixing thread)
    DWORD				*m_pdwUnCompressedBufferOffset;
    												// Offset into uncompressed buffer for thread (per mixing thread)
    DWORD				*m_pdwCompressedBufferOffset;
    												// Offset into compressed buffer for thread (per mixing thread)
    DWORD				*m_pdwResultLength; 		// Size of compressed data (in bytes) (per mixing thread)
    BOOL				*m_pfMixToBeReused;			// Will this player's mix be re-used by another player (per mixing thread)

    CFixedPool			*m_pCSOwner;

protected:

		//protected to ensure alloc/de-alloc occurs through the 
		//fixed pool mechanism using construct/destruct calls
	CDVCSPlayer();
	~CDVCSPlayer();

};

typedef CDVCSPlayer *LPDVCSPLAYER;

#undef DPF_MODNAME

#endif
