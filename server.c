#include	"unpthread.h"

void reverse_string( char*); 
static void	*interact_with_client(void *);		/* each thread executes this function */

int
main(int argc, char **argv)
{
	int				listenfd = 0, connfd = 0;
	pthread_t		tid;
	socklen_t		addrlen, len;
	struct sockaddr	*cliaddr;

	if (argc == 2)
		listenfd = Tcp_listen(NULL, argv[1], &addrlen);
	else if (argc == 3)
		listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
	else
		err_quit("usage: tcpserv01 [ <host> ] <service or port>");

	cliaddr = Malloc(addrlen);

	for ( ; ; ) {
		len = addrlen;
		connfd = Accept(listenfd, cliaddr, &len);
		Pthread_create(&tid, NULL, &interact_with_client, (void *) connfd);
	}
}

static void *
interact_with_client(void *arg)
{
	Pthread_detach(pthread_self());

	int sock = (int) arg;
	int flag = 1;
    int result = setsockopt(sock,            /* socket affected */                                     
                            IPPROTO_TCP,     /* set option at TCP level */                               
                            TCP_NODELAY,     /* name of option */                                        
                            (char *) &flag,  /* the cast is historical                                   
                                                   cruft */                                             
                            sizeof(int));    /* length of option value */  
	unsigned short name_size = 64, server_name_size = 128;
	char temp[name_size], new_name[server_name_size];

	bzero(temp, name_size);
	bzero(new_name, server_name_size);
	
	//server ACK
	Read( sock, temp, 4);	
//	printf(" %s \n", temp);
	
	// read from the sock the client's filename
	Read( sock, temp, name_size);

	//find a name that's not in conflict with another file
	if( access( temp, F_OK) != -1) {
		do {
			//find a new filename
			time_t now = time(NULL);
			strcpy( new_name, temp);
			strcat( new_name, ctime(&now));
			char *d = new_name, *s = new_name;
			do while(isspace(*s)) s++; while(*d++ = *s++);
		} while( access( new_name, F_OK) != -1);

	} else {
		//name isn't in conflict with another file
		strcpy( new_name, temp);
	}

//	printf(" server new name: %s\n", new_name);
	
	//send ack to client
//	printf("server name: %s\n", new_name);
	FILE* fp = Fopen(new_name, "w+");
	Writen( sock, new_name, MAXLINE);

	char recvline[MAXLINE], sendline[MAXLINE];
	ssize_t t = 0;
	do {

		//check if the client has closed the connection
		if ( recv(sock, recvline, MAXLINE, MSG_PEEK | MSG_DONTWAIT) == 0) {
			Fclose(fp);
			pthread_exit(NULL);
		}

		//read the client's file message 	
		Readline( sock, recvline, MAXLINE);
//		printf("server rcvd: X%sX\n", recvline);

		unsigned short recv_checksum =  csum( recvline, strlen(recvline));

		reverse_string(recvline);
		Fputs( recvline, fp);
		bzero(recvline, MAXLINE);

//		printf(" server recv csum: %x\n", recv_checksum);
		sendline[0] = (recv_checksum >> 8) & 0xff;
		sendline[1] = recv_checksum & 0xff;

//		printf("sendline[0]: %x\n",(unsigned char) sendline[0]);
//		printf("sendline[1]: %x\n",(unsigned char) sendline[1]);

		//send the server's checksum of the recvd message back to the client
		Writen( sock, sendline, MAXLINE); 
		bzero(sendline, MAXLINE);

	} while ( t >= 0 && errno != EINTR);

	Close((int) arg);		/* done with connected socket */
	return(NULL);
}


void reverse_string(char *str) {                                                                              
                                                                                                              
    /* skip null */                                                                                           
    if (str == 0)                                                                                             
    {                                                                                                         
        return;                                                                                               
    }                                                                                                         
                                                                                                              
    /* skip empty string */                                                                                   
    if (*str == 0)                                                                                            
    {                                                                                                         
        return;                                                                                               
    }                                                                                                         
                                                                                                              
    /* get range */                                                                                           
    char *start = str;                                                                                        
    char *end = start + strlen(str) - 2; /* buff is length 100 */                                             
    char temp;                                                                                                
                                                                                                              
    /* reverse */                                                                                             
    while (end > start)                                                                                       
    {                                                                                                         
        /* swap */                                                                                            
        temp = *start;                                                                                        
        *start = *end;                                                                                        
        *end = temp;                                                                                          
                                                                                                              
        /* move */                                                                                            
        ++start;                                                                                              
        --end;                                                                                                
    }                                                                                                         
}













