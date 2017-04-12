#include	"unp.h"

int
main(int argc, char **argv)
{
	int		sockfd;

	if (argc != 4)
		err_quit("usage: tcpcli <hostname> <service> <file>");

	sockfd = Tcp_connect(argv[1], argv[2]);

//	FILE* fp = Fopen( argv[3], "r+");
	
	str_cli(argv[3], sockfd);		/* do it all */

	exit(0);
}
