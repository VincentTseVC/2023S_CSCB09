#include <errno.h>
#include <netdb.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

void parse_inaddr(struct addrinfo *ai, const char *hostname, const char *port) {
  struct addrinfo hint;
  struct addrinfo *head;

  memset(&hint, 0, sizeof(struct addrinfo));
  hint.ai_family = AF_INET;
  hint.ai_socktype = SOCK_STREAM;
  hint.ai_protocol = 0;
  hint.ai_flags = AI_NUMERICSERV;

  int r = getaddrinfo(hostname, port, &hint, &head);
  if (r != 0) {
    if (r == EAI_SYSTEM) {
      perror("getaddrinfo");
    } else {
      fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(r));
    }
    exit(1);
  } else {
    *ai = *head;
    ai->ai_next = NULL;
    freeaddrinfo(head);
  }
}

int main(int argc, char **argv)
{
  if (argc < 3) {
    fprintf(stderr, "Need IPv4 address and port.\n");
    return 1;
  }

  struct addrinfo ai;
  parse_inaddr(&ai, argv[1], argv[2]);

  int s = socket(ai.ai_family, ai.ai_socktype, ai.ai_protocol);
  if (-1 == connect(s, ai.ai_addr, ai.ai_addrlen)) {
    perror("connect");
    return 1;
  }

  // TODO: A select() loop to copy stdin to socket (s), copy socket to stdout.
  // Do not exit until both stdin and socket are EOF.
  // Do not monitor an FD for reading if it reached EOF in the past, lest you
  // would cause busy-polling. (Why?)

  fd_set all_fds, listen_fds;
  FD_ZERO(&all_fds);
  FD_SET(STDIN_FILENO, &all_fds);
  FD_SET(s, &all_fds);
  int max_fd = s;



  while (1) {
    listen_fds = all_fds;

    if (select(max_fd + 1, &listen_fds, NULL, NULL, NULL) < 0) {
      perror("select");
      return 1;
    }

    if (FD_ISSET(STDIN_FILENO, &listen_fds)) {
      char buff[1024];
      int len = read(STDIN_FILENO, buff, sizeof(buff));

      if (len < 0) {
        perror("read");
        return 1;
      } else if (len == 0) {
        // EOF, remove stdin from all_fds set
        FD_CLR(STDIN_FILENO, &all_fds);
      } else {
        if (write(s, buff, len) < 0) {
          perror("write");
          return 1;
        }
      }
    }

    if (FD_ISSET(s, &listen_fds)) {
      char buff[1024];
      int len = read(s, buff, sizeof(buff));

      if (len < 0) {
        perror("read");
        return 1;
      } else if (len == 0) {
        // EOF, remove s from all_fds set
        FD_CLR(s, &all_fds);
      } else {
        if (write(STDOUT_FILENO, buff, len) < 0) {
          perror("write");
          return 1;
        }
      }
    }

    if (!FD_ISSET(STDIN_FILENO, &all_fds) && !FD_ISSET(s, &all_fds))
      break;
  }

  close(s);
  return 0;
}
