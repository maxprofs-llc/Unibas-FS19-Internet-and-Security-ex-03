// Write CPP code here
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#define PORT 8080
#define SA struct sockaddr
#define max(a, b) (((a) < (b)) ? (b) : (a))

void diep(char *s)
{
  perror(s);
  exit(1);
}

void register_nick(char *nick)
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
  char *command = malloc(strlen(nick) + 1);
  command[0] = 'r';
  strcpy(command + 1, nick);
  printf("register command: %s\n", command);
  write(sockfd, command, strlen(command));
  close(sockfd);
}

struct sockaddr_in query_nick(char *nick)
{
  int sockfd;
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
  char *command = malloc(strlen(nick) + 1);
  command[0] = 'q';
  strcpy(command + 1, nick);
  printf("Query command: %s\n", command);
  write(sockfd, command, strlen(nick));
  struct sockaddr_in input;
  int b = read(sockfd, &input, sizeof(input));
  if (b <= 0)
  {
    diep("Failed reading:");
  }

  close(sockfd);

  return input;
}

int main(int argc, char **argv)
{

  if (argc != 2)
  {
    diep("wrong args");
  }

  char *nick = argv[1];

  // register the current username
  register_nick(nick);

  char *otherName;

  printf("Other nickname: ");
  scanf("%s", otherName);

  /*struct sockaddr_in adr = */ query_nick(otherName);
  //char *ip = inet_ntoa(adr.sin_addr);
  //printf("Addr: %s\n", ip);
}
