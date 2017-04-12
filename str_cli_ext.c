#include	"unp.h"

void
str_cli(char *filename, int sockfd)
{

	FILE* fp = Fopen( filename, "r+");

	char	sendline[MAXLINE], recvline[MAXLINE];

	//write filename to socket
	printf("client write: %s\n", filename);
	fflush(stdout);
	Writen( sockfd, filename, strlen(filename));

	Readline(sockfd, recvline, MAXLINE);
	printf("server ack: %s\n", recvline);
	bzero(recvline, MAXLINE);

	while (Fgets(sendline, MAXLINE, fp) != NULL) {

		//send a line from the file
		printf("client sending: %s\n", sendline);
		Writen(sockfd, sendline, strlen(sendline));
		
		//create a checksum of the sendline string
		unsigned short original_checksum = csum( sendline, strlen(sendline));
		printf("client's checksum: %x\n",(unsigned short) original_checksum);
		unsigned short server_checksum = 0;
		bzero(sendline, MAXLINE);

		Readline(sockfd, recvline, MAXLINE);
		printf("client's recv from serv checksum: %x\n",(unsigned short) recvline[0]);

		//get message integrity by comparing server's checksum to the original
		
		bzero(recvline, MAXLINE);
	}
}
