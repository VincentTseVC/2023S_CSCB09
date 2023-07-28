#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// Takes sockaddr but actually expects sockaddr_in
void print_inet4_addr(const struct sockaddr *a) {
  if (a->sa_family != AF_INET) {
    fprintf(stderr, "non-IPv4 address\n");
    exit(1);
  }
  const struct sockaddr_in *a4 = (const struct sockaddr_in *)a;
  char buf[INET_ADDRSTRLEN];
  inet_ntop(AF_INET, & a4->sin_addr, buf, INET_ADDRSTRLEN);
  printf("%s\n", buf);
}

int main(int argc, char *argv[]) {
  struct addrinfo hint;
  struct addrinfo *head;

  // Use getaddrinfo() to find the IPv4 addresses of domain/host name argv[1].
  // For simplicity, you may assume argc>=2 so argv[1] exists.
  //
  // Set the service parameter to the string "80" or "http", meaning port number
  // is 80.
  //
  // Set up the hint parameter to request IPv4-only and stream-only.
  //
  // For each IPv4 address provided by getaddrinfo() (traverse the whole linked list),
  // use print_inet4_addr() above to print the IPv4 address in dot notation.
  //
  // Lastly, use freeaddrinfo() to free the linked list.
  //
  // Optional: If getaddrinfo() returns a non-zero number, use gai_strerror()
  // for printing out the error message to stderr, then exit.

  char *host = argv[1];

  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_flags = 0;

  getaddrinfo(host, "80", &hint, &head);

  for (struct addrinfo *cur = head; cur != NULL; cur=cur->ai_next) {
      print_inet4_addr(cur->ai_addr);
  }

  freeaddrinfo(head);

  return 0;
}
