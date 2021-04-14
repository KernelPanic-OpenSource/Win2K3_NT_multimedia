//
//  Microsoft Windows Media Technologies
//  Copyright (C) Microsoft Corporation, 1999 - 2001. All rights reserved.
//

// MSHDSP.DLL is a sample WMDM Service Provider(SP) that enumerates fixed drives.
// This sample shows you how to implement an SP according to the WMDM documentation.
// This sample uses fixed drives on your PC to emulate portable media, and 
// shows the relationship between different interfaces and objects. Each hard disk
// volume is enumerated as a device and directories and files are enumerated as 
// Storage objects under respective devices. You can copy non-SDMI compliant content
// to any device that this SP enumerates. To copy an SDMI compliant content to a 
// device, the device must be able to report a hardware embedded serial number. 
// Hard disks do not have such serial numbers.
//
// To build this SP, you are recommended to use the MSHDSP.DSP file under Microsoft
// Visual C++ 6.0 and run REGSVR32.EXE to register the resulting MSHDSP.DLL. You can
// then build the sample application from the WMDMAPP directory to see how it gets 
// loaded by the application. However, you need to obtain a certificate from 
// Microsoft to actually run this SP. This certificate would be in the KEY.C file 
// under the INCLUDE directory for one level up. 


// MDSPDevice.h : Declaration of the CMDSPDevice

#ifndef __MDSPDEVICE_H_
#define __MDSPDEVICE_H_

/////////////////////////////////////////////////////////////////////////////
// CMDSPDevice
class ATL_NO_VTABLE CMDSPDevice : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CMDSPDevice, &CLSID_MDSPDevice>,
	public IMDSPDevice2, IMDSPDeviceControl,
    public ISpecifyPropertyPages
{
public:
	CMDSPDevice();
	~CMDSPDevice();


DECLARE_REGISTRY_RESOURCEID(IDR_MDSPDEVICE)

BEGIN_COM_MAP(CMDSPDevice)
	COM_INTERFACE_ENTRY(IMDSPDevice)
	COM_INTERFACE_ENTRY(IMDSPDevice2)
	COM_INTERFACE_ENTRY(IMDSPDeviceControl)
	COM_INTERFACE_ENTRY(ISpecifyPropertyPages)
END_COM_MAP()

// IMDSPDevice
public:
	HRESULT InitGlobalDeviceInfo();
	WCHAR m_wcsName[MAX_PATH];
	STDMETHOD(EnumStorage)(/*[out]*/ IMDSPEnumStorage **ppEnumStorage);
	STDMETHOD(GetFormatSupport)(_WAVEFORMATEX **pFormatEx,
                                UINT *pnFormatCount,
                                LPWSTR **pppwszMimeType,
                                UINT *pnMimeTypeCount);
	STDMETHOD(GetDeviceIcon)(/*[out]*/ ULONG *hIcon);
	STDMETHOD(GetStatus)(/*[out]*/ DWORD *pdwStatus);
	STDMETHOD(GetPowerSource)(/*[out]*/ DWORD *pdwPowerSource, /*[out]*/ DWORD *pdwPercentRemaining);
	STDMETHOD(GetSerialNumber)(/*[out]*/ PWMDMID pSerialNumber, /*[in, out]*/BYTE abMac[WMDM_MAC_LENGTH]);
	STDMETHOD(GetType)(/*[out]*/ DWORD *pdwType);
	STDMETHOD(GetVersion)(/*[out]*/ DWORD *pdwVersion);
	STDMETHOD(GetManufacturer)(/*[out,string,size_is(nMaxChars)]*/ LPWSTR pwszName, /*[in]*/ UINT nMaxChars);
	STDMETHOD(GetName)(/*[out,string,size_is(nMaxChars)]*/ LPWSTR pwszName, /*[in]*/ UINT nMaxChars);
    STDMETHOD(SendOpaqueCommand)(OPAQUECOMMAND *pCommand);
// IMDSPDevice2
	STDMETHOD(GetStorage)( LPCWSTR pszStorageName, IMDSPStorage** ppStorage );
 
    STDMETHOD(GetFormatSupport2)(   DWORD dwFlags,
                                    _WAVEFORMATEX **ppAudioFormatEx,
                                    UINT *pnAudioFormatCount,
			                        _VIDEOINFOHEADER **ppVideoFormatEx,
                                    UINT *pnVideoFormatCount,
                                    WMFILECAPABILITIES **ppFileType,
                                    UINT *pnFileTypeCount );

	STDMETHOD(GetSpecifyPropertyPages)( ISpecifyPropertyPages** ppSpecifyPropPages, 
									    IUnknown*** pppUnknowns, 
									    ULONG* pcUnks );

    STDMETHOD(GetPnPName)( LPWSTR pwszPnPName, UINT nMaxChars );


// IMDSPDeviceControl
	STDMETHOD(GetDCStatus)(/*[out]*/ DWORD *pdwStatus);
	STDMETHOD(GetCapabilities)(/*[out]*/ DWORD *pdwCapabilitiesMask);
	STDMETHOD(Play)();
	STDMETHOD(Record)(/*[in]*/ _WAVEFORMATEX *pFormat);
	STDMETHOD(Pause)();
	STDMETHOD(Resume)();
	STDMETHOD(Stop)();
	STDMETHOD(Seek)(/*[in]*/ UINT fuMode, /*[in]*/ int nOffset);

// ISpecifyPropertyPages
    STDMETHOD(GetPages)(CAUUID *pPages);

};

#endif //__MDSPDEVICE_H_
