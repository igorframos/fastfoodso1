#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

class Cliente
{
	public:
	int socketID;
	socklen_t size;
	pthread_t thread;
	sockaddr_in addr;
	
	Cliente();	
};
