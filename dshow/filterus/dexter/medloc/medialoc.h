//@@@@AUTOBLOCK+============================================================;
//
//  THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
//  KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
//  PURPOSE.
//
//  File: medialoc.h
//
//  Copyright (c) Microsoft Corporation.  All Rights Reserved.
//
//@@@@AUTOBLOCK-============================================================;

// MediaLocator.h : Declaration of the CMediaLocator

#ifndef __MEDIALOCATOR_H_
#define __MEDIALOCATOR_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMediaLocator
class ATL_NO_VTABLE CMediaLocator : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMediaLocator, &CLSID_MediaLocator>,
	public IDispatchImpl<IMediaLocator, &IID_IMediaLocator, &LIBID_DexterLib>
{
    CCritSec m_Lock;
    BOOL m_bUseLocal;
    static TCHAR szShowWarnOriginal[_MAX_PATH];
    static TCHAR szShowWarnReplaced[_MAX_PATH];
    static INT_PTR CALLBACK DlgProc( HWND h, UINT i, WPARAM w, LPARAM l );
    static DWORD WINAPI ThreadProc( LPVOID lpParam );

public:
	CMediaLocator();

DECLARE_REGISTRY_RESOURCEID(IDR_MEDIALOCATOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CMediaLocator)
	COM_INTERFACE_ENTRY(IMediaLocator)
	COM_INTERFACE_ENTRY(IUnknown)
END_COM_MAP()

    void AddOneToDirectoryCache( HKEY h, int WhichDirectory );
    int  GetLeastUsedDirectory( HKEY h, int DirectoryCount );
    void ReplaceDirectoryPath( HKEY h, int WhichDirectory, TCHAR * Path );
    void ShowWarnReplace( TCHAR * pOriginal, TCHAR * pReplaced );

// IMediaLocator
public:
    STDMETHODIMP FindMediaFile( BSTR Input, BSTR FilterString, BSTR * pOutput, long Flags );
    STDMETHODIMP AddFoundLocation( BSTR DirectoryName );
};

#endif //__MEDIALOCATOR_H_
