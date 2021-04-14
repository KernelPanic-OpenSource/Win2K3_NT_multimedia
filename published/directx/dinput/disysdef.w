#ifndef __DISYSDEF_H__
#define __DISYSDEF_H__

// IOCTLs defined here are only for NT builds to work with DINPUT.SYS

#ifdef WINNT

#undef IOCTL_FIRST
#define IOCTL_FIRST 0x0800

#undef IOCTL_GETVERSION
#define IOCTL_GETVERSION 0x0000

#undef IOCTL_INPUTLOST
#define IOCTL_INPUTLOST           CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_DESTROYINSTANCE
#define IOCTL_DESTROYINSTANCE     CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+1, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_SETDATAFORMAT
#define IOCTL_SETDATAFORMAT       CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+2, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_ACQUIREINSTANCE
#define IOCTL_ACQUIREINSTANCE     CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+3, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_UNACQUIREINSTANCE
#define IOCTL_UNACQUIREINSTANCE   CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+4, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_SETNOTIFYHANDLE
#define IOCTL_SETNOTIFYHANDLE     CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+5, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_SETBUFFERSIZE
#define IOCTL_SETBUFFERSIZE       CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+6, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Keyboard class IOCTLs
#undef IOCTL_KBD_CREATEINSTANCE
#define IOCTL_KBD_CREATEINSTANCE   CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+7, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_KBD_INITKEYS
#define IOCTL_KBD_INITKEYS         CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+8, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Mouse class IOCTLs
#undef IOCTL_MOUSE_CREATEINSTANCE
#define IOCTL_MOUSE_CREATEINSTANCE CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+9, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_MOUSE_INITBUTTONS
#define IOCTL_MOUSE_INITBUTTONS    CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+10, METHOD_BUFFERED, FILE_ANY_ACCESS)
// Joystick class IOCTLs
#undef IOCTL_JOY_CREATEINSTANCE
#define IOCTL_JOY_CREATEINSTANCE   CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+11, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_JOY_PING
#define IOCTL_JOY_PING             CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+12, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_JOY_GETINITPARMS
#define IOCTL_JOY_GETINITPARMS     CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+13, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_JOY_FFIO
#define IOCTL_JOY_FFIO             CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+14, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_GETSEQUENCEPTR
#define IOCTL_GETSEQUENCEPTR       CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+15, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_JOY_GETAXES
#define IOCTL_JOY_GETAXES          CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+16, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_MOUSE_GETWHEEL
#define IOCTL_MOUSE_GETWHEEL       CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+17, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_JOY_CONFIGCHANGED
#define IOCTL_JOY_CONFIGCHANGED    CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+18, METHOD_BUFFERED, FILE_ANY_ACCESS)
#undef IOCTL_MAX
#define IOCTL_MAX                  CTL_CODE(FILE_DEVICE_KEYBOARD, IOCTL_FIRST+19, METHOD_BUFFERED, FILE_ANY_ACCESS)

#endif  // WINNT

#endif  // __DISYSDEF_H__