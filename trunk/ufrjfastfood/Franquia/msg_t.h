#ifndef _MSG_TYPE_H_
#define _MSG_TYPE_H_

#define MAX_MSG_SIZE (1<<8)

struct msg_t
{
	long int mtype;
	char mtext[MAX_MSG_SIZE];
};

#endif

