#include	"unpthread.h"

void	*interact_with_server(void *);

 int	sockfd;
 FILE	*fp;
 int	done;

void
str_cli(char* filename, int sockfd_cli)
{
	sockfd = sockfd_cli;
	char		sendline[MAXLINE], recvline[MAXLINE];
	fp = Fopen(filename, "r+");
	pthread_t	tid;

	Writen( sockfd, "ACK\n", 4);

	Writen( sockfd, filename, strlen(filename));
//	printf("client sending: %s\n", filename);

	Readline(sockfd, recvline, MAXLINE);
//	printf("client's ACK from server: %s\n", recvline);
	bzero(recvline, MAXLINE);

	Pthread_create(&tid, NULL, &interact_with_server, NULL);
	Pthread_join(tid, NULL); 

	if (done == 0)
		err_quit("server terminated prematurely");
}

void *
interact_with_server(void *arg)
{
	char		sendline[MAXLINE], recvline[MAXLINE];
	bzero(sendline, MAXLINE);
	bzero(recvline, MAXLINE);
	int ttt = 0;

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

//		printf("client sending: %s\n", sendline);
		Writen( sockfd, sendline, strlen(sendline));

		//create a checksum of the sendline string
		unsigned short original_checksum = csum( sendline, strlen(sendline));
//		printf("client's csum: %x\n", original_checksum);
		bzero(sendline, MAXLINE);

		Readline( sockfd, recvline, MAXLINE);
		unsigned short server_checksum = recvline[ttt++] & 0xff;
		server_checksum = server_checksum << 8;
		server_checksum = server_checksum | (recvline[ttt] & 0xff);
//		printf("recvline: %p%p\n", recvline[0] & 0xff, recvline[1] & 0xff);
//		printf("client's recv from serv csum: %x\n", server_checksum);
		if( (unsigned short) (server_checksum ^ original_checksum) != 0) {
			perror("server error");
			Close(fp);
			pthread_exit(NULL);
		
		}
		bzero(recvline, MAXLINE);
	}

	Shutdown(sockfd, SHUT_WR);	/* EOF on stdin, send FIN */

	done = 1;
	return(NULL);
	/* return (i.e., thread terminates) when end-of-file on stdin */
}
