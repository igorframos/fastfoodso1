#ifndef _GERENCIA_H
#define _GERENCIA_H

#include <stdio.h>
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <vector>
#include <string>
#include <algorithm>
#include <queue>
#include <stack>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <bits/posix_opt.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "global.h"
#include "relatorio.h"

void *gerencia (void*);

#endif

