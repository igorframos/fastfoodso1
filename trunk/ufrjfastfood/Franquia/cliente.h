#ifndef _CLIENTE_H_
#define _CLIENTE_H_


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/posix_opt.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>

#include "msg_t.h"

using namespace std;

#include "pedido.h"
#include "global.h"

void* cliente (void*);

#endif
