// Server program.

#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

# define MAX_BACKLOG 10

typedef struct client_struct {
    int fd; // client_fd

    char buffer[1024];
    char username[9];
    char filename[11];
    size_t filesize;
    char *file_content; // malloc(filesize);

    struct client_struct *next;
} client;

// cmdline reminder: portnum, helper
int main(int argc, char *argv[]) {
  if (argc < 2) {
    fprintf(stderr, "missing port number\n");
    return 1;
  }

  unsigned short port = (unsigned short) atoi(argv[1]);

  int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (sock_fd == -1) {
    fprintf(stderr, "Could not create socket\n");
    return 1;
  }

  struct sockaddr_in server;
  server.sin_addr.s_addr = INADDR_ANY;
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  if (bind(sock_fd, (struct sockaddr *)&server, sizeof(server)) < 0) {
    fprintf(stderr, "Binding to port failed\n");
    return 1;
  }

  if (listen(sock_fd, MAX_BACKLOG) < 0) {
    fprintf(stderr, "failed to listen\n");
    close(sock_fd);
    return 1;
  }

  int serial_number = 0;

  client *clients;

  fd_set all_fds, listen_fds;
  FD_ZERO(&all_fds);
  FD_SET(sock_fd, &all_fds);
  int max_fd = sock_fd;

  while (1) {
    listen_fds = all_fds;

    if (select(max_fd + 1, &listen_fds, NULL, NULL, NULL) < 0) continue;

    if (FD_ISSET(sock_fd, &listen_fds)) {
      // new connection
      int client_fd = accept(sock_fd, NULL, NULL);

      if (client_fd < 0) {
        fprintf(stderr, "Error on accept\n");
        return 1;
      }

      FD_SET(client_fd, &all_fds);
      // create new client and add to linked list
      client *new_client = malloc(sizeof(client));
      new_client->fd = client_fd;
      // TODO: memset the flied

      // add to head..
      new_client->next = clients;
      clients = new_client;
    }

    for (client *cur = clients; cur != NULL; cur=cur->next) {
      if (FD_ISSET(cur->fd, &listen_fds)) {
        // TODO: 有點難
        //  1. read whatever is in the fd (a random chunk) [vi, ncent\n00-, readme.tx, t\n1026\n, ....]
        //  2. find the first '\n'
        //                buffer -> [vi
        //                buffer -> [vincent\n00-               strncpy(buffer, username, xxxx);
        //                buffer -> [00-
        //                buffer -> [00-readme.tx
        //                buffer -> [00-readme.txt\n1026\n      strncpy(buffer, filename, xxxx);
        //                buffer -> [1026\n                     filesize = ......;
        //                buffer -> [
      }
    }
  }

  return 0;
}

