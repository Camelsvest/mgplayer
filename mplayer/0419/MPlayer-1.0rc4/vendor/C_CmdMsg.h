#ifndef __C_CMDMSG_H__
#define	__C_CMDMSG_H__


#include "FLCMDService.h"

typedef enum{
	eSyncQ = 0,
	eAsyncQ = 1
}E_MsgQ_Type;	

class C_CmdMsg
{
public:
	~C_CmdMsg ();
	static C_CmdMsg* GetObject ();
	bool Send (long msgType, const unsigned char* buf, int length);
	bool Recv (long msgType);
	bool RecvNoWait (long msgType);	
    message_buf 	m_sbuf;
	unsigned int	m_cmdCode;
	
	bool SetPowerState(unsigned int state, int* pStatus);
  	bool SetLcmState(unsigned int state, int* pStatus);  
	bool SetScreenState(unsigned int state, int* pStatus);
	bool SetUsbState(unsigned int state, int* pStatus);
	bool GetPowerState(unsigned int* pState, int* pStatus);
  	bool GetLcmState(unsigned int* pState, int* pStatus);  
  	bool GetHingeState(unsigned int* pState, int* pStatus);  
	bool GetScreenState(unsigned int* pState, int* pStatus);
	bool GetPowerSource(unsigned int* pSource, int* pStatus);
	bool GetCommonState(unsigned long cmdCode, unsigned int* pState, int* pStatus);
	bool NotifyKeyPadEvent (unsigned int keyCode, unsigned int eventCode);
	bool NotifyTouchEvent (unsigned int xyData, unsigned int eventCode);
	bool NotifyAPEvent (unsigned int data, unsigned int eventCode);
	bool NotifyLowBattEvent (unsigned int state, unsigned int eventCode);
	bool NotifyEvent (int msgQType, unsigned long cmdCode, int dataCode, unsigned int eventCode);
	bool ProcessRecvMsg(long msgType);
	bool PostMsgToServer (char* msg);
	bool PostMsgToSelf(const char *msg);
	bool RecvSyncMsg();
	bool RecvAsyncMsg();
	bool RecvWaitSyncMsg();
	bool RecvWaitAsyncMsg();
	int  GetItemsOfMsgQ(E_MsgQ_Type queueType);
	int	 GetCapacityOfMsgQ(E_MsgQ_Type queueType);
	//### 2010/06/15 Manitain current & from message type
	long m_curSyncMsgType;
	long m_curAsyncMsgType;
	long m_curMsgType;
	long m_fromMsgType;
	// Nuvoton CCLi8 (2009.11.18)
	// Support a generic channel for message passing between content AS and the system manager.
	bool GetSyncGenericMsg (S_FL_CMD_CODE_GENERIC_MSG_IMPL &genericMsg);	
	
private:
	C_CmdMsg ();
  	C_CmdMsg (key_t syncKey,key_t asyncKey, int msgflg);	

  	int GetMsqId (long msgType);	
	bool GetSyncReplyMsg(unsigned int cmdCode, int* pStatus);	
	static C_CmdMsg* mInstance;
	key_t m_syncKey;
	key_t m_asyncKey;
	int m_msgflg;
	int m_syncMsqid;
	int m_asyncMsqid;
};

typedef struct msgwrapper_tag {
    long    wrapperCmdCode;
    long    fromMsgType;
}  __attribute__ ((__packed__)) S_FL_MSG_WRAPPER;

#endif // __C_CMDMSG_H__

























