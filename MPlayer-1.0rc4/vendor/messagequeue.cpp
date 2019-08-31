/*
 * This file is part of MPlayer.
 *
 * MPlayer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * MPlayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with MPlayer; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

/// \file
/// \ingroup Properties Command2Property OSDMsgStack

#include "config.h"

#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <pthread.h>

extern "C" {
#include "mp_msg.h"
#include "mp_core.h"
}

#include "C_CmdMsg.h"

#include "messagequeue.h"

static C_CmdMsg* s_pCmdMsgObj;
static int volatile s_demo_msgq_index;
static char s_demo_msgq_cmd[4096];

int demo_msgq_index;
char * demo_msgq_cmd;
pthread_mutex_t g_mutex;
static pthread_t s_th;

static void * demo_msgq_thread(void *arg)
{
	for (;;)
	{
		demo_msgq_index = demo_msgq_RecvMsg(NULL);
		if ( demo_msgq_index == MPLAYER_CMD )
		{
			demo_msgq_cmd = demo_msgq_get_cmd();
			for (;;)
			{
				pthread_mutex_lock(&g_mutex);
				if ( demo_msgq_index == MPLAYER_NULL )
				{
					pthread_mutex_unlock(&g_mutex);
					break;
				}
				else
				{
					pthread_mutex_unlock(&g_mutex);
					usleep(20000);
				}
			}
		}
	}

	return NULL;
}

int demo_msgq_init(void)
{
#if 0
	//### Init CmdMsgObj MsgType
  C_CmdMsg* pCmdMsgObj;
  pCmdMsgObj = C_CmdMsg::GetObject();
  pCmdMsgObj->m_curSyncMsgType = FL_CMD_SYNC_TYPE_2N;
  pCmdMsgObj->m_curAsyncMsgType = FL_CMD_ASYNC_TYPE_2N;
#endif

	s_pCmdMsgObj = C_CmdMsg::GetObject();

	s_pCmdMsgObj->m_curSyncMsgType = FL_CMD_SYNC_TYPE_2S;
    s_pCmdMsgObj->m_curAsyncMsgType = FL_CMD_ASYNC_TYPE_2S;

	s_demo_msgq_index = 0;

	while ( s_pCmdMsgObj->RecvAsyncMsg() );

	demo_msgq_index = MPLAYER_NULL;
	pthread_mutex_init(&g_mutex, NULL);
	if (pthread_create(&s_th,NULL,demo_msgq_thread,NULL)!=0)
		exit_player(EXIT_NONE);

	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### Message Queue Initialized. ###\n" );

  return 0;
}

void __sendGenEventMsg(C_CmdMsg* pCmdMsgObj, long targetMsgType, const char *msgCmd)
{
           S_FL_CMD_CODE_GENERIC_MSG_IMPL genericMsg;
           genericMsg.command = FL_CMD_CODE_GENERIC_EVENT;
           sprintf (genericMsg.msgData, "%s", msgCmd);
           genericMsg.msgData[sizeof (genericMsg.msgData) - 1] = '\0';
           pCmdMsgObj->Send (targetMsgType, (const unsigned char *) &genericMsg, sizeof (genericMsg.command) + strlen (genericMsg.msgData) + 1);  
}

static void __asyncMessageHandler(C_CmdMsg* pCmdMsgObj)
{
//  TraceLog("ASYNC/receive: cmd=0x%x\n",pCmdMsgObj->m_cmdCode);                        
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### __asyncMessageHandler: ASYNC/receive: cmd=0x%x ###\n", pCmdMsgObj->m_cmdCode );
  unsigned long keyCode;     
///  S_FL_CMD_CODE_NOTIFY_EVENT* pEvent; 
           long replyMsgType = FL_CMD_ASYNC_TYPE_2F;
           if( pCmdMsgObj->m_fromMsgType > 0 )
                     replyMsgType = pCmdMsgObj->m_fromMsgType;

  switch(pCmdMsgObj->m_cmdCode) {

           case FL_CMD_CODE_GENERIC_EVENT: {
                     S_FL_CMD_CODE_GENERIC_MSG *pGenericMsg = (S_FL_CMD_CODE_GENERIC_MSG *) pCmdMsgObj->m_sbuf.mtext;
					 // Based on your protocol to tokenize the message string.
					 char *cmd = strtok (pGenericMsg->msgData, " ,");
					 int i;
					 if (strcmp (cmd, "MPLAYER_CMD") == 0) {
						i = 0;
						while ( cmd != NULL )
						{
							if ( i == 0 )
							{
								mp_msg( MSGT_CPLAYER, MSGL_INFO, "### GOT %s ###\n", cmd );
								strcpy( (char *)&s_demo_msgq_cmd[0], cmd );
								cmd = strtok( NULL, " ," );
								i++;
							}
							else if ( i == 1 )
							{
								mp_msg( MSGT_CPLAYER, MSGL_INFO, "### GOT %s ###\n", cmd );
								strcpy( (char *)&s_demo_msgq_cmd[0], cmd );
								cmd = strtok( NULL, " ," );
								i++;
							}
							else if ( i == 2 )
							{
								strcat( (char *)&s_demo_msgq_cmd[0], " " );
								strcat( (char *)&s_demo_msgq_cmd[0], cmd );
								mp_msg( MSGT_CPLAYER, MSGL_INFO, "### GOT %s ###\n", (char *)&s_demo_msgq_cmd[0] );
								cmd = strtok( NULL, " ," );
								i++;
							}
						}
                        // Reply message, it's optional for FL_CMD_CODE_GENERIC_EVENT.
///						mp_msg( MSGT_CPLAYER, MSGL_INFO, "### __asyncMessageHandler: replyMsgType = 0x%x ###\n", replyMsgType );
                        __sendGenEventMsg(pCmdMsgObj,replyMsgType,"### __asyncMessageHandler: MPLAYER_CMD SUCCESS ###");
						mp_msg( MSGT_CPLAYER, MSGL_INFO, "### __asyncMessageHandler: MPLAYER_CMD SUCCESS ###\n" );
						s_demo_msgq_index = MPLAYER_CMD;
                     }
					 else
					 {
						s_demo_msgq_index = MPLAYER_NULL;
					 }
                     break;
           }

           default:
///                     TraceLog("*** ASYNC/cmd code:%x unrecognized ***\n", pCmdMsgObj->m_cmdCode);           
				mp_msg( MSGT_CPLAYER, MSGL_INFO, "### default: ASYNC/cmd code:%x unrecognized ###\n", pCmdMsgObj->m_cmdCode );
				s_demo_msgq_index = MPLAYER_NULL;
				break;
   }
}


int demo_msgq_RecvMsg (void *arg)
{
	s_demo_msgq_index = 0;
           int delayTime;
///           unsigned int count=0;
///           C_CmdMsg* pCmdMsgObj;

///           TraceLog("Start Recv Message\n ");
///	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### Start Recv Message ###\n" );
///         pCmdMsgObj = C_CmdMsg::GetObject();
                   
///           pCmdMsgObj->m_curSyncMsgType = FL_CMD_SYNC_TYPE_2S;
///           pCmdMsgObj->m_curAsyncMsgType = FL_CMD_ASYNC_TYPE_2S;
           
///         while(1)
         {
                   //### process SYNC type, skip

                   //### process ASYNC type
                   if(s_pCmdMsgObj->RecvAsyncMsg ())
                   {
//                            TraceLog("ASYNC cmd_code=0x%x\n",pCmdMsgObj->m_cmdCode);            
	mp_msg( MSGT_CPLAYER, MSGL_INFO, "### RecvMsg: ASYNC cmd_code=0x%x ###\n", s_pCmdMsgObj->m_cmdCode );
                              __asyncMessageHandler(s_pCmdMsgObj);
							  return s_demo_msgq_index;
                              delayTime = 20000;
                   } else delayTime = 30000;
                     usleep (delayTime);      
           }
	return MPLAYER_NULL;
}

int demo_msgq_SendMsg(const char *msgCmd)
{
	long replyMsgType = FL_CMD_SYNC_TYPE_2F;
    if( s_pCmdMsgObj->m_fromMsgType > 0 )
		replyMsgType = s_pCmdMsgObj->m_fromMsgType;
    S_FL_CMD_CODE_GENERIC_MSG_IMPL genericMsg;
    genericMsg.command = FL_CMD_CODE_GENERIC_EVENT;
    sprintf (genericMsg.msgData, "%s", msgCmd);
    genericMsg.msgData[sizeof (genericMsg.msgData) - 1] = '\0';
    s_pCmdMsgObj->Send (replyMsgType, (const unsigned char *) &genericMsg, sizeof (genericMsg.command) + strlen (genericMsg.msgData) + 1);
}

char * demo_msgq_get_cmd(void)
{
	if ( s_demo_msgq_index == MPLAYER_CMD )
	{
		return s_demo_msgq_cmd;
	}
}
