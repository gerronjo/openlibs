#ifndef _MZF_COMMON_HEADER_
#define _MZF_COMMON_HEADER_


#define IOCTL_BASE          0x800
#define MY_CTL_CODE(i)        \
    CTL_CODE                  \
    (                         \
	FILE_DEVICE_DISK_FILE_SYSTEM,  \
	IOCTL_BASE + i,       \
	METHOD_BUFFERED,      \
	FILE_ANY_ACCESS       \
)

#define IOCTL_FILEMON_SETEVENT				MY_CTL_CODE(1)
#define IOCTL_FILEMON_GETINFO				MY_CTL_CODE(2)
#define IOCTRL_STARTMON						    MY_CTL_CODE(3) 
#define IOCTRL_STOPMON						    MY_CTL_CODE(4)

//
// TODO: Add your IOCTL define here
//


#endif //_MZF_COMMON_HEADER_