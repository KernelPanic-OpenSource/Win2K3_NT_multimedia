/* Copyright (c) 1998 - 1999  Microsoft Corporation.  All Rights Reserved. */
#define INITGUID
#include <guiddef.h>

#include <uuids.h>

// control.odl should really be converted to control.idl and the generated
// control_i.c can then be added to sources.  Instead, we just manually
// dump the DEFINE_GUIDs here because we can't include control.h under INITGUID
// without dragging in all of the windows type information
//
DEFINE_GUID(LIBID_QuartzTypeLib,0x56A868B0L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IAMCollection,0x56A868B9L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IMediaControl,0x56A868B1L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IMediaEvent,0x56A868B6L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IMediaEventEx,0x56A868C0L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IMediaPosition,0x56A868B2L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IBasicAudio,0x56A868B3L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IVideoWindow,0x56A868B4L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IBasicVideo,0x56A868B5L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IBasicVideo2,0x329BB360L,0xF6EA,0x11D1,0x90,0x38,0x00,0xA0,0xC9,0x69,0x72,0x98);
DEFINE_GUID(IID_IDeferredCommand,0x56A868B8L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IQueueCommand,0x56A868B7L,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(CLSID_FilgraphManager,0xE436EBB3L,0x524F,0x11CE,0x9F,0x53,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IFilterInfo,0x56A868BAL,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IRegFilterInfo,0x56A868BBL,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IMediaTypeInfo,0x56A868BCL,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IPinInfo,0x56A868BDL,0x0AD4,0x11CE,0xB0,0x3A,0x00,0x20,0xAF,0x0B,0xA7,0x70);
DEFINE_GUID(IID_IAMStats,0xBC9BCF80L,0xDCD2,0x11D2,0xAB,0xF6,0x00,0xA0,0xC9,0x05,0xF3,0x75);

#include <amstream_i.c>
#include <austream_i.c>
#include <ddstream_i.c>
#include <mmstream_i.c>
#include <qedit_i.c>
#include <strmif_i.c>
#include <regbag_i.c>
#include <tuner_i.c>
#ifndef TUNING_MODEL_ONLY
#include <segment_i.c>
#endif
#include <msvidctl_i.c>
#include <bdaiface_i.c>
#include <bdatif_i.c>
#include <sbe_i.c>
#include <videoacc_i.c>
#include <tvratings_i.c>
#include <encdec_i.c>
#include <mixerocx_i.c>