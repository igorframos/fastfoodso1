#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "clienteApp.h"

Cliente::Cliente (void)
{
	size = sizeof(addr);
}

