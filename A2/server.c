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
#define PORT 8080
#define SA struct sockaddr

typedef struct entry
{
  struct sockaddr_in addr;
  char nickname[255];
  struct entry *next;
} entry;

void diep(char *s)
{
  perror(s);
  exit(1);
}

void func(int sockfd)
{
  static entry *entries = NULL;
  for (;;)
  {
    struct sockaddr_in client;
    // Accept the data packet from client and verification
    int size = sizeof(client);
    int connfd = accept(sockfd, (SA *)&client, &size);
    if (connfd < 0)
    {
      printf("server acccept failed...\n");
      diep("huh!?");
      exit(0);
    }
    else
      printf("server acccept the client...\n");

    char buffer[255];
    int b = read(connfd, buffer, 255);

    if (buffer[0] == 'r')
    {
      printf("Registering client %s\n", buffer + 1);
      entry *e = malloc(sizeof(entry));
      e->addr = client;
      strcpy(e->nickname, buffer + 1);
      e->next = NULL;
      printf("Registered: %s\n", e->nickname);
      if (entries == NULL)
      {
        entries = e;
      }
      else
      {
        entry *curr = entries;
        while (curr->next != NULL)
        {
          curr = curr->next;
        }
        curr->next = e;
      }
    }
    else
    {
      printf("Querrying for client %s\n", buffer + 1);
      entry *curr = entries;
      while (curr != NULL && strcmp(curr->nickname, buffer + 1))
      {
        printf("Not entry: %s != %s\n", curr->nickname, buffer + 1);
        curr = curr->next;
      }
      if (curr == NULL)
      {
        printf("Not found\n");
        continue;
      }
      else
      {
        char *ip = inet_ntoa(curr->addr.sin_addr);
        printf("Returning address for %s %s\n", curr->nickname, ip);
        write(connfd, &curr->addr, sizeof(curr->addr));
      }
    }
  }
}

// Driver function
int main()
{
  int sockfd, len;
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
  servaddr.sin_port = htons(PORT);

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

  // Function for chatting between client and server
  func(sockfd);

  // After chatting close the socket
  close(sockfd);
}
