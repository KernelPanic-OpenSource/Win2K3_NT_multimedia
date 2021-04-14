/*==========================================================================;
 *
 *  Copyright (C) 1995-1998 Microsoft Corporation.  All Rights Reserved.
 *
 *  File:       dsprv.h
 *  Content:    DirectSound include file
 *@@BEGIN_MSINTERNAL
 *  History:
 *   Date       By      Reason
 *   ====       ==      ======
 *  8/19/98     dereks  Created.
 *@@END_MSINTERNAL
 *
 **************************************************************************/

#ifndef __DSPRV_INCLUDED__
#define __DSPRV_INCLUDED__

#ifndef __DSOUND_INCLUDED__
#error dsound.h not included
#endif // __DSOUND_INCLUDED__

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

// DirectSound Private Component GUID {11AB3EC0-25EC-11d1-A4D8-00C04FC28ACA}
DEFINE_GUID(CLSID_DirectSoundPrivate, 0x11ab3ec0, 0x25ec, 0x11d1, 0xa4, 0xd8, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

// 
// DirectSound Mixer Properties {84624F80-25EC-11d1-A4D8-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundMixer, 0x84624f80, 0x25ec, 0x11d1, 0xa4, 0xd8, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum 
{
    DSPROPERTY_DIRECTSOUNDMIXER_SRCQUALITY,
    DSPROPERTY_DIRECTSOUNDMIXER_ACCELERATION,
} DSPROPERTY_DIRECTSOUNDMIXER;

typedef enum
{
    DIRECTSOUNDMIXER_SRCQUALITY_WORST,
    DIRECTSOUNDMIXER_SRCQUALITY_PC,
    DIRECTSOUNDMIXER_SRCQUALITY_BASIC,
    DIRECTSOUNDMIXER_SRCQUALITY_ADVANCED,
} DIRECTSOUNDMIXER_SRCQUALITY;

#define DIRECTSOUNDMIXER_SRCQUALITY_DEFAULT DIRECTSOUNDMIXER_SRCQUALITY_PC

typedef struct _DSPROPERTY_DIRECTSOUNDMIXER_SRCQUALITY_DATA
{
    GUID                        DeviceId;   // DirectSound device id
    DIRECTSOUNDMIXER_SRCQUALITY Quality;    // SRC quality
} DSPROPERTY_DIRECTSOUNDMIXER_SRCQUALITY_DATA, *PDSPROPERTY_DIRECTSOUNDMIXER_SRCQUALITY_DATA;

#define DIRECTSOUNDMIXER_ACCELERATIONF_NORING0MIX   0x00000001
#define DIRECTSOUNDMIXER_ACCELERATIONF_NOHWBUFFERS  0x00000002
#define DIRECTSOUNDMIXER_ACCELERATIONF_NOHW3D       0x00000004
#define DIRECTSOUNDMIXER_ACCELERATIONF_NOHWPROPSETS 0x00000008
                                                        
#define DIRECTSOUNDMIXER_ACCELERATIONF_FULL         0x00000000
#define DIRECTSOUNDMIXER_ACCELERATIONF_STANDARD     0x00000008
#define DIRECTSOUNDMIXER_ACCELERATIONF_NONE         0x0000000F

#define DIRECTSOUNDMIXER_ACCELERATIONF_DEFAULT      DIRECTSOUNDMIXER_ACCELERATIONF_STANDARD

typedef struct _DSPROPERTY_DIRECTSOUNDMIXER_ACCELERATION_DATA
{
    GUID    DeviceId;   // DirectSound device id
    ULONG   Flags;      // Acceleration flags
} DSPROPERTY_DIRECTSOUNDMIXER_ACCELERATION_DATA, *PDSPROPERTY_DIRECTSOUNDMIXER_ACCELERATION_DATA;

// 
// DirectSound Device Properties {84624F82-25EC-11d1-A4D8-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundDevice, 0x84624f82, 0x25ec, 0x11d1, 0xa4, 0xd8, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum
{
    DSPROPERTY_DIRECTSOUNDDEVICE_PRESENCE,
    DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A,
    DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1,
    DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1,
    DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W,
    DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A,
    DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W,
    DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A,
    DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W,
} DSPROPERTY_DIRECTSOUNDDEVICE;

#if DIRECTSOUND_VERSION >= 0x0700
#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A
#endif // UNICODE
#else // DIRECTSOUND_VERSION >= 0x0700
#define DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1
#endif // DIRECTSOUND_VERSION >= 0x0700

typedef enum
{
    DIRECTSOUNDDEVICE_TYPE_EMULATED,
    DIRECTSOUNDDEVICE_TYPE_VXD,
    DIRECTSOUNDDEVICE_TYPE_WDM
} DIRECTSOUNDDEVICE_TYPE;

typedef enum
{
    DIRECTSOUNDDEVICE_DATAFLOW_RENDER,
    DIRECTSOUNDDEVICE_DATAFLOW_CAPTURE
} DIRECTSOUNDDEVICE_DATAFLOW;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_PRESENCE_DATA
{
    GUID    DeviceId;   // DirectSound device id
    BOOL    Present;    // Presence switch
} DSPROPERTY_DIRECTSOUNDDEVICE_PRESENCE_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_PRESENCE_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A_DATA
{
    LPSTR                       DeviceName; // waveIn/waveOut device name
    DIRECTSOUNDDEVICE_DATAFLOW  DataFlow;   // Data flow (i.e. waveIn or waveOut)
    GUID                        DeviceId;   // DirectSound device id
} DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W_DATA
{
    LPWSTR                      DeviceName; // waveIn/waveOut device name
    DIRECTSOUNDDEVICE_DATAFLOW  DataFlow;   // Data flow (i.e. waveIn or waveOut)
    GUID                        DeviceId;   // DirectSound device id
} DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W_DATA;

#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_DATA DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_W_DATA
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_DATA DSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_WAVEDEVICEMAPPING_A_DATA
#endif // UNICODE

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA
{
    GUID                        DeviceId;               // DirectSound device id
    CHAR                        DescriptionA[0x100];    // Device description (ANSI)
    WCHAR                       DescriptionW[0x100];    // Device description (Unicode)
    CHAR                        ModuleA[MAX_PATH];      // Device driver module (ANSI)
    WCHAR                       ModuleW[MAX_PATH];      // Device driver module (Unicode)
    DIRECTSOUNDDEVICE_TYPE      Type;                   // Device type
    DIRECTSOUNDDEVICE_DATAFLOW  DataFlow;               // Device dataflow
    ULONG                       WaveDeviceId;           // Wave device id
    ULONG                       Devnode;                // Devnode (or DevInst)
} DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA
{
    DIRECTSOUNDDEVICE_TYPE      Type;           // Device type
    DIRECTSOUNDDEVICE_DATAFLOW  DataFlow;       // Device dataflow
    GUID                        DeviceId;       // DirectSound device id
    LPSTR                       Description;    // Device description
    LPSTR                       Module;         // Device driver module
    LPSTR                       Interface;      // Device interface
    ULONG                       WaveDeviceId;   // Wave device id
} DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA
{
    DIRECTSOUNDDEVICE_TYPE      Type;           // Device type
    DIRECTSOUNDDEVICE_DATAFLOW  DataFlow;       // Device dataflow
    GUID                        DeviceId;       // DirectSound device id
    LPWSTR                      Description;    // Device description
    LPWSTR                      Module;         // Device driver module
    LPWSTR                      Interface;      // Device interface
    ULONG                       WaveDeviceId;   // Wave device id
} DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA;

#if DIRECTSOUND_VERSION >= 0x0700
#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA
#endif // UNICODE
#else // DIRECTSOUND_VERSION >= 0x0700
#define DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA DSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA
#endif // DIRECTSOUND_VERSION >= 0x0700

typedef BOOL (CALLBACK *LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK1)(PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_1_DATA, LPVOID);
typedef BOOL (CALLBACK *LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA)(PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_A_DATA, LPVOID);
typedef BOOL (CALLBACK *LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKW)(PDSPROPERTY_DIRECTSOUNDDEVICE_DESCRIPTION_W_DATA, LPVOID);

#if DIRECTSOUND_VERSION >= 0x0700
#ifdef UNICODE
#define LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKW
#else // UNICODE
#define LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA
#endif // UNICODE
#else // DIRECTSOUND_VERSION >= 0x0700
#define LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK1
#endif // DIRECTSOUND_VERSION >= 0x0700

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1_DATA
{
    LPFNDIRECTSOUNDDEVICEENUMERATECALLBACK1 Callback;   // Callback function pointer
    LPVOID                                  Context;    // Callback function context argument
} DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA
{
    LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKA Callback;   // Callback function pointer
    LPVOID                                  Context;    // Callback function context argument
} DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA
{
    LPFNDIRECTSOUNDDEVICEENUMERATECALLBACKW Callback;   // Callback function pointer
    LPVOID                                  Context;    // Callback function context argument
} DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA, *PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA;

#if DIRECTSOUND_VERSION >= 0x0700
#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_W_DATA
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_A_DATA
#endif // UNICODE
#else // DIRECTSOUND_VERSION >= 0x0700
#define DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA DSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1_DATA
#define PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_DATA PDSPROPERTY_DIRECTSOUNDDEVICE_ENUMERATE_1_DATA
#endif // DIRECTSOUND_VERSION >= 0x0700

// 
// Basic DirectSound Acceleration Properties {1AEAA606-35F0-11D1-B161-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundBasicAcceleration, 0x1aeaa606, 0x35f0, 0x11d1, 0xb1, 0x61, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum
{
    DSPROPERTY_DIRECTSOUNDBASICACCELERATION_ACCELERATION,
} DSPROPERTY_DIRECTSOUNDBASICACCELERATION;

typedef enum
{
    DIRECTSOUNDBASICACCELERATION_NONE,
    DIRECTSOUNDBASICACCELERATION_SAFE,
    DIRECTSOUNDBASICACCELERATION_STANDARD,
    DIRECTSOUNDBASICACCELERATION_FULL,
} DIRECTSOUNDBASICACCELERATION_LEVEL;

#define DIRECTSOUNDBASICACCELERATION_DEFAULT    DIRECTSOUNDBASICACCELERATION_FULL

typedef struct _DSPROPERTY_DIRECTSOUNDBASICACCELERATION_ACCELERATION_DATA
{
    GUID                                DeviceId;   // DirectSound device id
    DIRECTSOUNDBASICACCELERATION_LEVEL  Level;      // Basic acceleration level
} DSPROPERTY_DIRECTSOUNDBASICACCELERATION_ACCELERATION_DATA, *PDSPROPERTY_DIRECTSOUNDBASICACCELERATION_ACCELERATION_DATA;

// 
// DirectSound Debug Properties {F2957840-260C-11d1-A4D8-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundDebug, 0xf2957840, 0x260c, 0x11d1, 0xa4, 0xd8, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum
{
    DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A,
    DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W,
} DSPROPERTY_DIRECTSOUNDDEBUG;

#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A
#endif // UNICODE

#define DIRECTSOUNDDEBUG_DPFINFOF_PRINTFUNCTIONNAME     0x00000001
#define DIRECTSOUNDDEBUG_DPFINFOF_PRINTPROCESSTHREADID  0x00000002
#define DIRECTSOUNDDEBUG_DPFINFOF_PRINTFILELINE         0x00000004

#define DIRECTSOUNDDEBUG_DPFINFOF_DEFAULT               DIRECTSOUNDDEBUG_DPFINFOF_PRINTFUNCTIONNAME

typedef struct _DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A_DATA
{
    ULONG   Flags;              // DPF flags
    ULONG   DpfLevel;           // DPF level
    ULONG   BreakLevel;         // Break level
    CHAR    LogFile[MAX_PATH];  // Log file
} DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A_DATA, *PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W_DATA
{
    ULONG   Flags;              // DPF flags
    ULONG   DpfLevel;           // DPF level
    ULONG   BreakLevel;         // Break level
    WCHAR   LogFile[MAX_PATH];  // Log file
} DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W_DATA, *PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W_DATA;

#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_DATA DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W_DATA
#define PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_DATA PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_W_DATA
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_DATA DSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A_DATA
#define PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_DATA PDSPROPERTY_DIRECTSOUNDDEBUG_DPFINFO_A_DATA
#endif // UNICODE

#define DIRECTSOUNDDEBUG_DPFLEVEL_DEFAULT   2
#define DIRECTSOUNDDEBUG_BREAKLEVEL_DEFAULT 0

// 
// DirectSound Persistent Data {1BE55C3E-36AB-11d1-B162-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundPersistentData, 0x1be55c3e, 0x36ab, 0x11d1, 0xb1, 0x62, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum
{
    DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A,
    DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W,
} DSPROPERTY_DIRECTSOUNDPERSISTENTDATA;

#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A
#endif // UNICODE

typedef struct _DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A_DATA
{
    GUID    DeviceId;           // DirectSound device id
    LPSTR   SubKeyName;         // Optional subkey name
    LPSTR   ValueName;          // Value name
    ULONG   RegistryDataType;   // Data type
    LPVOID  Data;               // Data pointer
    ULONG   DataSize;           // Size of data buffer, in bytes
} DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A_DATA, *PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A_DATA;

typedef struct _DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W_DATA
{
    GUID    DeviceId;           // DirectSound device id
    LPWSTR  SubKeyName;         // Optional subkey name
    LPWSTR  ValueName;          // Value name
    ULONG   RegistryDataType;   // Data type
    LPVOID  Data;               // Data pointer
    ULONG   DataSize;           // Size of data buffer, in bytes
} DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W_DATA, *PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W_DATA;

#ifdef UNICODE
#define DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_DATA DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W_DATA
#define PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_DATA PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_W_DATA
#else // UNICODE
#define DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_DATA DSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A_DATA
#define PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_DATA PDSPROPERTY_DIRECTSOUNDPERSISTENTDATA_PERSISTDATA_A_DATA
#endif // UNICODE

// 
// DirectSound Buffer Properties {50393DEA-51AD-11d2-91B2-00C04FC28ACA}
// 

DEFINE_GUID(DSPROPSETID_DirectSoundBuffer, 0x50393dea, 0x51ad, 0x11d2, 0x91, 0xb2, 0x0, 0xc0, 0x4f, 0xc2, 0x8a, 0xca);

typedef enum 
{
    DSPROPERTY_DIRECTSOUNDBUFFER_DEVICEID,
} DSPROPERTY_DIRECTSOUNDBUFFER;

typedef struct _DSPROPERTY_DIRECTSOUNDBUFFER_DEVICEID_DATA
{
    LPDIRECTSOUNDBUFFER Buffer;     // Buffer object pointer
    GUID                DeviceId;   // DirectSound device ID
} DSPROPERTY_DIRECTSOUNDBUFFER_DEVICEID_DATA, *PDSPROPERTY_DIRECTSOUNDBUFFER_DEVICEID_DATA;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif  // __DSPRV_INCLUDED__ 
