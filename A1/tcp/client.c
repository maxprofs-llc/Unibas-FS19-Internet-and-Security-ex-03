// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
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
      if (read(sockfd, buf, sizeof(buf)))
      {
        return;
      }
      printf("recv %s\n", buf);
    }
  }
}

int main()
{
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  // socket create and varification
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
  servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  servaddr.sin_port = htons(PORT);

  // connect the client socket to server socket
  if (connect(sockfd, (SA *)&servaddr, sizeof(servaddr)) != 0)
  {
    printf("connection with the server failed...\n");
    exit(0);
  }
  else
    printf("connected to the server..\n");

  // function for chat
  func(sockfd);

  // close the socket
  close(sockfd);
}
