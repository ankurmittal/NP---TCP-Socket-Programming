
#include "unpthread.h"
extern void str_echo(int);
extern void str_time(int);

static void *echo(void *); /* each thread executes this function*/
static void *daytime(void *); /* each thread executes this function*/

int main(int argc, char **argv)
{
	int echofd, timefd, connfd;
	int maxfdp1, *iptr;
	fd_set rset;
	socklen_t clilen;
	pthread_t tid;
	struct sockaddr_in cliaddr, servaddr;
	char buff[30];
	int on = 1;

	echofd = Socket (AF_INET, SOCK_STREAM, 0);
	timefd = Socket (AF_INET, SOCK_STREAM, 0);
	
	Setsockopt(echofd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));
	Setsockopt(timefd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(int));

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl (INADDR_ANY);
	servaddr.sin_port = htons (7161);

	Bind(echofd, (SA *) &servaddr, sizeof(servaddr));
	Listen(echofd, LISTENQ);
	
	servaddr.sin_port = htons (7162);

	Bind(timefd, (SA *) &servaddr, sizeof(servaddr));
	Listen(timefd, LISTENQ);

	for ( ; ; ) {
		FD_ZERO(&rset);

		FD_SET(echofd, &rset);
		FD_SET(timefd, &rset);
		maxfdp1 = max(echofd, timefd) + 1;

		Select(maxfdp1, &rset, NULL, NULL, NULL);

		if (FD_ISSET(echofd, &rset)) { /* Connection coming in */
			clilen = sizeof(cliaddr);
			iptr = Malloc(sizeof(int));
			*iptr = Accept(echofd, (SA *) &cliaddr, &clilen);
			printf("Connection to echo service from %s, port %d\n",
			   inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));
			Pthread_create(&tid, NULL, &echo, iptr);
		}

		if (FD_ISSET(timefd, &rset)) { /* Connection coming in */
			clilen = sizeof(cliaddr);
			iptr = Malloc(sizeof(int));
			*iptr = Accept(timefd, (SA *) &cliaddr, &clilen);
			printf("Connection to time service from %s, port %d\n",
			   inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)),
			   ntohs(cliaddr.sin_port));
			Pthread_create(&tid, NULL, &daytime, iptr);
		}
	}
}
static void * echo(void *arg)
{
	int connfd;
	connfd = *((int *) arg);
	free(arg);
	pthread_detach(pthread_self());
	str_echo(connfd);
	close(connfd); /* done with connected socket */
	return (NULL);
}


static void * daytime(void *arg)
{
	int connfd;
	connfd = *((int *) arg);
	free(arg);
	pthread_detach(pthread_self());
	str_time(connfd);
	close(connfd); /* done with connected socket */
	return (NULL);
}
