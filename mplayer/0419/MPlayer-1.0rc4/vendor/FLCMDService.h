/****************************************************************
*																*
* Copyright (c) Nuvoton Technology Corp. All rights reserved.	*
*																*
*****************************************************************/

#ifndef __FL_CMD_SERVICE_H__
#define __FL_CMD_SERVICE_H__

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>

// ### MSG Queue Definition
// Nuvoton CCLi8 (2009.11.18)
// Enlarge message length limit for generic message passing.
// Nuvoton Chris 2010.12.14	to enlarge MSG length
//#define FL_CMD_MSGSZ   		1024
#define FL_CMD_MSGSZ   		(1024 + 32)
#define FL_SYNC_CMD_KEY		12345
#define FL_ASYNC_CMD_KEY	(FL_SYNC_CMD_KEY + 10000)

#define FL_CMD_MSGGET_FLAG 	( 0666 | IPC_CREAT )
#define FL_CMD_FLAG_NOWAIT 	IPC_NOWAIT
#define FL_CMD_FLAG_WAIT 	0
#define FL_CMD_SYNC_TYPE_F2S	1		//### Sync queue: Flash to System Manager
#define FL_CMD_SYNC_TYPE_2S		FL_CMD_SYNC_TYPE_F2S	//### Sync queue: To System Manager
#define FL_CMD_SYNC_TYPE_S2F	2		//### Sync queue: System Manager to Flash content
#define FL_CMD_SYNC_TYPE_2F		FL_CMD_SYNC_TYPE_S2F		//### Sync queue: To Flash content
#define FL_CMD_ASYNC_TYPE_F2S	3		//### ASync queue: Flash to System Manager
#define FL_CMD_ASYNC_TYPE_2S	FL_CMD_ASYNC_TYPE_F2S	//### ASync queue: To System Manager
#define FL_CMD_ASYNC_TYPE_S2F	4		//### ASync queue: System Manager to Flash content
#define FL_CMD_ASYNC_TYPE_2F	FL_CMD_ASYNC_TYPE_S2F		//### ASync queue: To Flash content
//### 2009/06/09 Chris: To support OSD relative event from system manager
#define FL_CMD_ASYNC_TYPE_S2P	5		//### ASync queue: System Manager to Flash Player
#define FL_CMD_ASYNC_TYPE_2P	FL_CMD_ASYNC_TYPE_S2P		//### ASync queue: To Flash Player
#define FL_CMD_SYNC_TYPE_2P		6		//### Sync queue: To Flash Player
//### 2010/04/08 Chris: To support NAtive AP
#define FL_CMD_SYNC_TYPE_2N		7						//### Sync queue: To Native AP
#define FL_CMD_ASYNC_TYPE_2N	8						//### ASync queue: To Native AP

//### Power Source Type
#define FL_CMD_POWER_SOURCE_BATT 	0x00
#define FL_CMD_POWER_SOURCE_EXT 	0x01


//### Command Code ##############
	//### Sync
#define FL_CMD_CODE_REPLY				0xEE000000
#define FL_CMD_CODE_SET_POWER_STATE		0x00000100
#define FL_CMD_CODE_SET_POWER_STATE_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_SET_POWER_STATE )
#define FL_CMD_CODE_GET_POWER_STATE		0x00000200
#define FL_CMD_CODE_GET_POWER_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_POWER_STATE )
#define FL_CMD_CODE_GET_POWER_SOURCE	0x00000210
#define FL_CMD_CODE_GET_POWER_SOURCE_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_POWER_SOURCE )
#define FL_CMD_CODE_SET_SCREEN_STATE	0x00000300
#define FL_CMD_CODE_SET_SCREEN_STATE_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_SET_SCREEN_STATE )
#define FL_CMD_CODE_GET_SCREEN_STATE	0x00000400
#define FL_CMD_CODE_GET_SCREEN_STATE_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_SCREEN_STATE )
#define FL_CMD_CODE_SET_CPU_STATE		0x00000500
#define FL_CMD_CODE_SET_CPU_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_SET_CPU_STATE )
#define FL_CMD_CODE_GET_CPU_STATE		0x00000600
#define FL_CMD_CODE_GET_CPU_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_CPU_STATE )
  //### 2009/06/10 Chris: Support LCM control
#define FL_CMD_CODE_SET_LCM_LEVEL		0x00000700
#define FL_CMD_CODE_SET_LCM_LEVEL_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_SET_LCM_LEVEL )
#define FL_CMD_CODE_GET_LCM_LEVEL		0x00000800
#define FL_CMD_CODE_GET_LCM_LEVEL_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_LCM_LEVEL )
//## 2009/06/22 ken: support Hinge state
#define FL_CMD_CODE_GET_HINGE_STATE		0x00000900
#define FL_CMD_CODE_GET_HINGE_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_HINGE_STATE)
//## 2009/08/19 Chris: support USB state
#define FL_CMD_CODE_SET_USB_STATE		0x00000A00
#define FL_CMD_CODE_SET_USB_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_SET_USB_STATE)
//## 2009/08/28 Chris: support Get Power On Setting state
#define FL_CMD_CODE_GET_PWR_SETTING_STATE		0x00000B00
#define FL_CMD_CODE_GET_PWR_SETTING_STATE_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GET_PWR_SETTING_STATE)
// Nuvoton CCLi8 (2009.11.18)
// Synchronous generic message.
#define FL_CMD_CODE_GENERIC			0x00000F00
#define FL_CMD_CODE_GENERIC_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_GENERIC)

  //### Async
#define FL_CMD_CODE_POWER_STATE_EVENT	0x00700500
#define FL_CMD_CODE_POWER_STATE_EVENT_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_POWER_STATE_EVENT )
#define FL_CMD_CODE_LOW_BATT_EVENT		0x00700600
#define FL_CMD_CODE_LOW_BATT_EVENT_R	( FL_CMD_CODE_REPLY | FL_CMD_CODE_LOW_BATT_EVENT )
#define FL_CMD_CODE_KEYPAD_EVENT		0x00700700
#define FL_CMD_CODE_KEYPAD_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_KEYPAD_EVENT )
#define FL_CMD_CODE_TOUCH_EVENT			0x00700800
#define FL_CMD_CODE_TOUCH_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_TOUCH_EVENT )
#define FL_CMD_CODE_AP_EVENT			0x00700900
#define FL_CMD_CODE_AP_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_AP_EVENT )
//### 2009/06/10 Chris: system manager notify shut down event
#define FL_CMD_CODE_SHUT_DOWM_EVENT			0x00700A00
#define FL_CMD_CODE_SHUT_DOWN_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_SHUT_DOWN_EVENT )
//### 2009/06/22 Chris: system manager notify hinge event
#define FL_CMD_CODE_HINGE_STATE_EVENT				0x00700B00
#define FL_CMD_CODE_HINGE_STATE_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_HINGE_STATE_EVENT )
//### 2009/07/02 Chris: Adjust Volume Event
#define FL_CMD_CODE_VOLUME_STATE_EVENT				0x00700C00
#define FL_CMD_CODE_VOLUME_STATE_EVENT_R		( FL_CMD_CODE_REPLY | FL_CMD_CODE_VOLUME_STATE_EVENT )
//### 2009/08/14 Chris: New change events as Battery, Lcm & USB connection
#define FL_CMD_CODE_BATT_STATE_EVENT				0x00700D00
#define FL_CMD_CODE_LCM_STATE_EVENT					0x00700E00
#define FL_CMD_CODE_USB_CONNECT_EVENT				0x00700F00
//### 2010/01/25 Chris: system manager Ask Player Control events
#define FL_CMD_CODE_PLAYER_PAUSE_EVENT				0x00701100
#define FL_CMD_CODE_PLAYER_RESUME_EVENT				0x00701200
#define FL_CMD_CODE_PLAYER_STOP_EVENT				0x00701300
#define FL_CMD_CODE_PLAYER_REWIND_EVENT				0x00701400
#define FL_CMD_CODE_PLAYER_SNAPSHOT_EVENT			0x00701500
#define FL_CMD_CODE_PLAYER_DISPLAY_SCALE_EVENT		0x00701600
#define FL_CMD_CODE_PLAYER_DISPLAY_SCREEN_EVENT		0x00701610
//### 2010/01/28 Chris: Error Message Event Code
#define FL_CMD_CODE_PLAYER_ERROR_EVENT				0x00701F00
//### 2009/11/05 Chris: Miscellaneous Events
#define FL_CMD_CODE_MISC_EVENT						0x00710000
// Nuvoton CCLi8 (2009.11.18)
// Asynchronous generic message.
#define FL_CMD_CODE_GENERIC_EVENT			0x00720000


//### Status Code
#define FL_CMD_STATUS_OK 	0x00
#define FL_CMD_STATUS_FAIL 	0x01

//### QWERTY Keyboard Marker
#define FL_CMD_QWERTY_MARKER 0xFE000000
#define FL_STATE_CODE_USB_CONNECT		1
#define FL_STATE_CODE_USB_DISCONNECT	-1


#ifdef __cplusplus
extern "C"
{
#endif


 //### Declare the message queue structure.
 
typedef struct msgbuf_tag {
    long    mtype;
    unsigned char    mtext[FL_CMD_MSGSZ];
}  __attribute__ ((__packed__)) message_buf;

 //### Declare command message set structure.
typedef struct {
    unsigned long    command;
    unsigned long    state;
} __attribute__ ((__packed__)) S_FL_CMD_CODE_SET_STATE;

typedef struct {
    unsigned long    command;
    unsigned short   status;

} __attribute__ ((__packed__)) S_FL_CMD_CODE_R;

typedef struct {
    unsigned long    command;
    unsigned short   status;
} __attribute__ ((__packed__)) S_FL_CMD_CODE_SET_STATE_R;

typedef struct {
    unsigned long    command;
} __attribute__ ((__packed__)) S_FL_CMD_CODE_GET_STATE;

typedef struct {
    unsigned long    command;
    unsigned short   status;
    unsigned long    state;
} __attribute__ ((__packed__)) S_FL_CMD_CODE_GET_STATE_R;

typedef struct {
    unsigned long    command;
    unsigned long    event;		//### lowbattery, KeyRelease, touch release
    unsigned long    data;		//### current state or keypad or touch
} __attribute__ ((__packed__)) S_FL_CMD_CODE_NOTIFY_EVENT;

// Nuvoton CCLi8 (2009.11.18)
// Generic message structure.
typedef struct {
	unsigned long command;
	char msgData[];
} __attribute__ ((__packed__)) S_FL_CMD_CODE_GENERIC_MSG;
typedef struct {
	unsigned long command;
// Nuvoton Chris 2010.12.14	to enlarge GEN MSG length
//	char msgData[512];
	char msgData[1024];
} __attribute__ ((__packed__)) S_FL_CMD_CODE_GENERIC_MSG_IMPL;


typedef struct {
	unsigned long command;
	unsigned long fromAddress;	
	unsigned long len;
// Nuvoton Chris 2010.12.14	to enlarge GEN MSG length	
//	unsigned char msgData[512];
	unsigned char msgData[1024];
} __attribute__ ((__packed__)) S_FL_CMD_CODE_AP_EVENT_MSG;

#ifdef __cplusplus
}
#endif

#endif  // __FL_CMD_SERVICE_H__