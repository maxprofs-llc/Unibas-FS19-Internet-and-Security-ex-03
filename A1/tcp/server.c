#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define MAX 80
#define PORT 8080
#define SA struct sockaddr
#define max(a, b) (((a) < (b)) ? (b) : (a))

void diep(char *s)
{
  perror(s);
  exit(1);
}

void func(int sockfd)
{
  while (1)
  {
    char buf[255];
    fd_set set;
    FD_ZERO(&set);
    FD_SET(0, &set); //stdin
    FD_SET(sockfd, &set);
    int nfds = max(sockfd, 0) + 1;
    int nr = select(nfds, &set, NULL, NULL, NULL);
    if (nr < 0)
    {
      diep("select() failed");
    }
    if (FD_ISSET(0, &set)) //New stdin
    {
      scanf("%s", buf);
      write(sockfd, buf, sizeof(buf));
    }
    if (FD_ISSET(sockfd, &set))
    {
      if (read(sockfd, buf, sizeof(buf)) < 0)
      {
        return;
      }
      printf("recv %s\n", buf);
    }
  }
}

// Driver function
int main(int argc, char *argv[])
{
	
	if(argc != 2){
		printf("Invalid Arguments\n");
		printf("Usage: port\n");
		exit(0);
	}
	int port = atoi(argv[1]);
	printf("port: %i\n",port);
	
  int sockfd, connfd, len;
  struct sockaddr_in servaddr, cli;

  // socket create and verification
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd == -1)
  {
    printf("socket creation failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully created..\n");
  bzero(&servaddr, sizeof(servaddr));

  // assign IP, PORT
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(port);

  // Binding newly created socket to given IP and verification
  if ((bind(sockfd, (SA *)&servaddr, sizeof(servaddr))) != 0)
  {
    printf("socket bind failed...\n");
    exit(0);
  }
  else
    printf("Socket successfully binded..\n");

  // Now server is ready to listen and verification
  if ((listen(sockfd, 5)) != 0)
  {
    printf("Listen failed...\n");
    exit(0);
  }
  else
    printf("Server listening..\n");
  len = sizeof(cli);

  // Accept the data packet from client and verification
  connfd = accept(sockfd, (SA *)&cli, &len);
  if (connfd < 0)
  {
    printf("server acccept failed...\n");
    exit(0);
  }
  else
    printf("server acccept the client...\n");

  // Function for chatting between client and server
  func(connfd);

  // After chatting close the socket
  close(sockfd);
}
