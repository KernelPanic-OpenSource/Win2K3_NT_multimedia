//@@@@AUTOBLOCK+============================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  File: mediadet.h
//
//  Copyright (c) Microsoft Corporation.  All Rights Reserved.
//
//@@@@AUTOBLOCK-============================================================;

#ifndef __MEDIADET_H__
#define __MEDIADET_H__

extern const AMOVIESETUP_FILTER sudMediaDetFilter;
//extern const AMOVIESETUP_FILTER sudBitBucketFilter;

typedef struct _MDCacheFile
{
    double StreamLength;
    double StreamRate;
    GUID StreamType;
} MDCacheFile;

typedef struct _MDCache
{
    long Version;
    FILETIME FileTime;
    long Count;
    MDCacheFile CacheFile[1];
} MDCache;

class CMediaDetPin
    : public CBaseInputPin
{
    friend class CMediaDetFilter;
    CMediaDetFilter * m_pFilter;
    CCritSec m_Lock;
    LONG m_cPinRef;          // Pin's reference count

protected:

    GUID m_mtAccepted;

    CMediaDetPin( CMediaDetFilter * pFilter, HRESULT *phr, LPCWSTR Name );

    STDMETHODIMP_(ULONG) NonDelegatingAddRef();
    STDMETHODIMP_(ULONG) NonDelegatingRelease();

    // CBaseInputPin necessary overrides
    HRESULT CheckMediaType( const CMediaType *pmtIn );

    // CBasePin overrides
    HRESULT GetMediaType( int Pos, CMediaType * pMediaType );
    HRESULT CompleteConnect( IPin * pReceivePin );
};

class CMediaDetFilter
    : public CBaseFilter
    , public IMediaDetFilter
{
    friend class CMediaDetPin;
    typedef CGenericList <CMediaDetPin> CInputList;
    CInputList m_PinList;
    CCritSec m_Lock;

    CMediaDetFilter( TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr );
    ~CMediaDetFilter( );

protected:

    long m_nPins;

public:

    // needed to define IUnknown methods
    DECLARE_IUNKNOWN;
    
    // Function needed for the class factory
    static CUnknown * WINAPI CreateInstance( LPUNKNOWN pUnk, HRESULT *phr );

    // CBaseFilter overrides
    STDMETHODIMP NonDelegatingQueryInterface( REFIID, void ** );

    // IMediaDetFilter
    STDMETHODIMP put_AcceptedMediaType( long PinNo, GUID * pMajorType );
    STDMETHODIMP get_Length( long PinNo, double * pVal );
    STDMETHODIMP put_AcceptedMediaTypeB( long PinNo, BSTR MajorType );
    STDMETHODIMP get_PinCount( long * pVal );

    // CBaseFilter overrides
    CBasePin * GetPin( int n );
    CMediaDetPin * GetPin2( int n );
    int GetPinCount( );

    // random pin stuff
    void InitInputPinsList( );
    CMediaDetPin * CreateNextInputPin( );
    void DeleteInputPin( CMediaDetPin * pPin );
    int GetNumFreePins();
};

class CAutoReleaseMutex
{
    HANDLE m_hMutex;

public:

    CAutoReleaseMutex( HANDLE hMutex )
    {
        m_hMutex = hMutex;
    }
    ~CAutoReleaseMutex( )
    {
        if( m_hMutex )
        {
            ReleaseMutex( m_hMutex );
        }
    }
};

class CMediaDet
    : public CUnknown
    , public IMediaDet
    , public IServiceProvider
    , public IObjectWithSite
{
    WCHAR * m_szFilename;
    CComPtr< IBaseFilter > m_pFilter;
    CComPtr< IGraphBuilder > m_pGraph;
    CComPtr< IBaseFilter > m_pMediaDet;
    CComPtr< IBaseFilter > m_pBitBucketFilter;
    CComPtr< IBaseFilter > m_pBitRenderer;

    // storage stuff
    //
    MDCache * m_pCache;
    HRESULT _ReadCacheFile( );
    void _WriteCacheFile( );
    void _FreeCacheMemory( );
    void _GetStorageFilename( WCHAR * In, WCHAR * Out );


    long m_nStream;
    long m_cStreams;
    bool m_bBitBucket;
    bool m_bAllowCached;

    // stuff for the frame grabbing
    //
    HDRAWDIB m_hDD;
    HDC m_hDC;
    HBITMAP m_hDib;
    HGDIOBJ m_hOld;
    char * m_pDibBits;
    long m_nDibWidth;
    long m_nDibHeight;
    double m_dLastSeekTime;

    HRESULT _SeekGraphToTime( double SeekTime );
    void _ClearOutEverything( ); // clear out filters, streams, and filename
    void _ClearGraphAndStreams( ); // clear out filters, plus streamcount info
    void _ClearGraph( ); // clear out any filters we've loaded
    HRESULT _Load( );
    HRESULT _InjectBitBuffer( );
    IPin * _GetNthStreamPin( long Stream );
    HRESULT _GetCacheDirectoryName( WCHAR * pPath );
    bool _IsLoaded( );

    CMediaDet( TCHAR *pName, LPUNKNOWN pUnk, HRESULT *phr );
    ~CMediaDet( );

public:

    // needed to define IUnknown methods
    DECLARE_IUNKNOWN;

    // CUnknown overrides
    STDMETHODIMP NonDelegatingQueryInterface( REFIID, void ** );

    // Function needed for the class factory
    static CUnknown * WINAPI CreateInstance( LPUNKNOWN pUnk, HRESULT *phr );

    STDMETHODIMP get_Filter( IUnknown* *pVal);
    STDMETHODIMP put_Filter( IUnknown* newVal);
    STDMETHODIMP get_Filename( BSTR *pVal);
    STDMETHODIMP put_Filename( BSTR newVal);
    STDMETHODIMP get_OutputStreams( long *pVal);
    STDMETHODIMP get_CurrentStream( long *pVal);
    STDMETHODIMP put_CurrentStream( long newVal);
    STDMETHODIMP get_StreamType( GUID *pVal);
    STDMETHODIMP get_StreamTypeB( BSTR *pVal);
    STDMETHODIMP get_StreamLength( double *pVal);
    STDMETHODIMP GetBitmapBits(double StreamTime, long * pBufferSize, char * pBuffer, long Width, long Height);
    STDMETHODIMP WriteBitmapBits(double StreamTime, long Width, long Height, BSTR Filename );
    STDMETHODIMP get_StreamMediaType(AM_MEDIA_TYPE *pVal);
    STDMETHODIMP GetSampleGrabber( ISampleGrabber ** ppVal );
    STDMETHODIMP get_FrameRate(double *pVal);
    STDMETHODIMP EnterBitmapGrabMode( double SeekTime );

    static void CALLBACK StaticLoader( BOOL bLoading, const CLSID * rclsid );
    static HANDLE m_ghMutex;
    static WCHAR m_gszCacheDirectoryName[_MAX_PATH];

    // --- IObjectWithSite methods
    // This interface is here so we can keep track of the context we're
    // living in.
    STDMETHODIMP    SetSite(IUnknown *pUnkSite);
    STDMETHODIMP    GetSite(REFIID riid, void **ppvSite);

    IUnknown *        m_punkSite;

    // IServiceProvider
    STDMETHODIMP QueryService(REFGUID guidService, REFIID riid, void **ppvObject);

};

#endif
