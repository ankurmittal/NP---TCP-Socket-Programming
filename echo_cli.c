#include "unp.h"
#include "common.h"

static void do_echo(FILE *fp, int sockfd)
{
	int maxfdp1, stdineof, n;
	fd_set rset;
	char buf[MAXLINE];
	stdineof = 0;
	FD_ZERO(&rset);
	int stdoutfd = fileno(stdout);
	for ( ; ; ) {
		if (stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		myselect(maxfdp1, &rset, NULL, NULL, NULL);
		if (FD_ISSET(sockfd, &rset)) { /* socket is readable */
			if ( (n = myread(sockfd, buf, MAXLINE)) == 0) {
				if (stdineof == 1)
					return; /* normal termination */
				else
					show_err_sys("Server terminated prematurely");
			}
			n = write(stdoutfd, buf, n);
		}
		if (FD_ISSET(fileno(fp), &rset)) { /* input is readable */
			if ( (n = myread(fileno(fp), buf, MAXLINE)) == 0) {
				stdineof = 1;
				Shutdown(sockfd, SHUT_WR); /* send FIN */
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			n = write(sockfd, buf, n);
		}
	}
}

	
int main(int argc, char **argv)
{
	int	 sockfd;
	struct sockaddr_in	servaddr;
	
	statusfd = fileno(stdout);

	if (argc < 2)
		show_err_sys("usage: echo_cli <IPaddress> [statusfd]");
	
	if(argc > 2) {
		statusfd = atoi(argv[2]);
	}

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		show_err_sys("socket error");


	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0) {
		fprintf(fdopen(statusfd, "w+"), "inet_pton error for %s\n", argv[1]);
		exit(0);
	}

	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		show_err_sys("connect error");

	do_echo(stdin, sockfd);
}
