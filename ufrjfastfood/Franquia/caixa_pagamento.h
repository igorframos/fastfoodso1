#ifndef _CAIXA_PAGAMENTO_H_ 
#define _CAIXA_PAGAMENTO_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>
#include <math.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <errno.h>
#include <bits/posix_opt.h>
#include <map>
#include <vector>
#include <string>
#include <set>
#include <algorithm>
#include <queue>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/types.h>

#include "msg_t.h"

using namespace std;

#include "pedido.h"
#include "global.h"

void *caixa_pagamento (void*);

#endif
