#ifndef MESSAGE_QUEUE_H
#define MESSAGE_QUEUE_H

#define MPLAYER_NULL			0
#define MPLAYER_CMD				1

#ifdef __cplusplus
extern "C" {
#endif

int demo_msgq_init(void);

int demo_msgq_RecvMsg (void *arg);

int demo_msgq_SendMsg(const char *msgCmd);

char * demo_msgq_get_cmd(void);

#ifdef __cplusplus
}
#endif

#endif /* MESSAGE_QUEUE_H */
