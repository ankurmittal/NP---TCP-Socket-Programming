#include "unp.h"
#include <stdio.h>

static struct hostent* gethostinfo(char *target, struct sockaddr_in servaddr);

static void process_service(int choice, struct hostent* hostp)
{	

	pid_t childpid;
	int status;
	int read_write_s;
	int pfd[2];
	char buf[MAXLINE];
	char fd[10], *ip;
	struct in_addr ** addr_list;
	if (pipe(pfd) == -1)
	{
		perror("Pipe failed");
		exit(1);
	}
	addr_list = (struct in_addr **) hostp->h_addr_list;
	ip = inet_ntoa(*addr_list[0]);
	sprintf(fd, "%d", pfd[1]);
	childpid = fork();
	if(childpid >= 0)
	{
		if(childpid == 0) //Child Process
		{
			close(pfd[0]);
			//read_write_s = write(pfd[1], "message from child", 19);
			if(choice == 2) {
				read_write_s = write(pfd[1], "Starting day time client....", 29);
				execlp("xterm", "xterm", "-e", "./time_cli", ip, fd, (char *) 0);
			} else if(choice == 1){
				read_write_s = write(pfd[1], "Starting echo client....", 29);
				execlp("xterm", "xterm", "-e", "./echo_cli", ip, fd, (char *) 0);
			}
			close(pfd[1]);
			exit(0);
		}
		else // Parent Process 
		{
			close(pfd[1]);
			while ((read_write_s = read(pfd[0], buf, MAXLINE))
					!= 0)
				printf("Status Message: %s\n", buf);
			close(pfd[0]);
			waitpid(childpid, &status, 0);
			if (WIFEXITED(status) == 0)
				printf("Client Crashed");
			if ( (status = WEXITSTATUS(status)) != 0)
				printf("Client exited with error code: %d", status);
			else
				printf("Client Terminated");
		}

	}
	else
	{
		perror("Forking failed!!!. Please try again.");
	}
}


int main(int argc, char **argv)
{
	int sockfd, choice;
	socklen_t len;
	struct sockaddr_in servaddr;
	struct hostent *hostp;
	char term;

	if (argc != 2)
		err_quit("usage: client <IP Address or hostname>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	//servaddr.sin_port   = htons(13);        /* daytime server */
	hostp = gethostinfo(argv[1], servaddr);
	close(sockfd);
	if(hostp == NULL) 
	{
		exit(-1);
	}

	do {
		printf("\n1. Echo\n");
		printf("2. Time\n");
		printf("3. Exit\n");
		printf("Please choose a option: ");
		if(scanf("%d%c", &choice, &term) != 2 || term != '\n')
		{
			choice = 0;
			do {
				term = fgetc(stdin);
			} while (term != '\n' && term != EOF);
			printf("Invalid Input. Please try again.");
		}
		else
		{
			switch(choice)
			{
				case 1: 
				case 2: process_service(choice, hostp);
						break;
				case 3: break;
				default: printf("Invalid Input. Please try again.");
			}
		}	
	} while(choice != 3);
}



struct hostent* gethostinfo(char *target, struct sockaddr_in servaddr)
{
	struct hostent *hostp;
	struct in_addr ** addr_list;

	if (inet_pton(AF_INET, target, &servaddr.sin_addr) <= 0) 
	{
		hostp = gethostbyname(target);
		if(hostp == (struct hostent *)NULL)
		{
			printf("HOST or IP not valid.\n");
			return NULL;
		}
		else 
		{
			addr_list = (struct in_addr **) hostp->h_addr_list;
			printf("IP Address: %s\n", inet_ntoa(*addr_list[0]));
		}
	}
	else
	{
		hostp = gethostbyaddr(&servaddr.sin_addr, sizeof(servaddr.sin_addr), AF_INET);
		if(hostp == (struct hostent *)NULL)
		{
			printf("IP not valid.\n");
			return NULL;
		}
		printf("Hostname is: %s\n", hostp->h_name);
	}
	return hostp;

}
