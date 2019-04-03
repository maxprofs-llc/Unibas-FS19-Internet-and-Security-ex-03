#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#define SRV_IP "127.0.0.1"
/* diep(), #includes and #defines like in the server */
#define BUFLEN 512
#define NPACK 10
#define PORT 9930

void diep(char *s)
{
  perror(s);
  exit(1);
}

int s, slen;
struct sockaddr_in si_other;

//// https://www.abc.se/~m6695/udp.html
int main(void)
{
  int i;
  slen = sizeof(si_other);
  char buf[BUFLEN];

  if ((s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    diep("socket");

  memset((char *)&si_other, 0, sizeof(si_other));
  si_other.sin_family = AF_INET;
  si_other.sin_port = htons(PORT);
  if (inet_aton(SRV_IP, &si_other.sin_addr) == 0)
  {
    fprintf(stderr, "inet_aton() failed\n");
    exit(1);
  }

  send_chat("Test123");

  close(s);
  return 0;
}

void send_chat(char *c)
{
  if (sendto(s, c, strlen(c) + 1, 0, &si_other, slen) == -1)
  {
    diep("sendto()");
  }
}