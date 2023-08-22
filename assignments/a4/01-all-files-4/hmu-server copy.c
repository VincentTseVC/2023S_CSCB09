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
#include <sys/wait.h>

# define MAX_BACKLOG 10

void handler(int sig) {
  int status;

  wait(&status);
  if (W)

}


void handle_client(int client_fd, int serial_number) {
  char buffer[1024];
  char username[9];
  char filename[101];
  char file_size_str[11];
  size_t filesize;
  size_t idx = 0;


  // Reset username, filename, and file_size-str
  memset(username, 0, sizeof(username));
  memset(filename, 0, sizeof(filename));
  memset(file_size_str, 0, sizeof(file_size_str));

  // Use Three while loops to handle username, filename, and filesize
  // Read from socket until we encounter a newline character.

  // Read username
  while (idx < sizeof(username) - 1) {
   if (read(client_fd, buffer, 1) <= 0) { // Reading byte by byte
     fprintf(stderr, "Failed to read username\n");
     close(client_fd);
     return;
   }

   if (buffer[0] == '\n') break;
   username[idx++] = buffer[0];
  }
  username[idx] = '\0'; // Null terminate the string

  // Validate username
  if (strlen(username) < 1 || strlen(username) > 8) {
    write(client_fd, "HDERR\n", 6);
    close(client_fd);
    return;
  }

  idx = 0;

  // Read filename
  while (idx < sizeof(filename) - 1) {
    if (read(client_fd, buffer, 1) <= 0) { // Reading byte by byte
      fprintf(stderr, "Failed to read username\n");
      close(client_fd);
      return;
    }

    if (buffer[0] == '\n') break;
    filename[idx++] = buffer[0];
  }
  filename[idx] = '\0'; // Null terminate the string

  // Validate filename
  if (strlen(filename) < 1 || strlen(filename) > 100) {
    write(client_fd, "HDERR\n", 6);
    close(client_fd);
    return;
  }

  idx = 0;

  // Read filesize
  while (idx < sizeof(file_size_str) - 1) {
    if (read(client_fd, buffer, 1) <= 0) { // Reading byte by byte
      fprintf(stderr, "Failed to read username\n");
      close(client_fd);
      return;
    }

    if (buffer[0] == '\n') break;
    file_size_str[idx++] = buffer[0];
  }

  // TODO: Validate file_size_str (all digit?)

  filesize = strtoul(file_size_str, NULL, 10);

  // Create file
  char file_path[120]; // Assuming max path length to be 120
  sprintf(file_path, "%s-%d-%s", username, serial_number, filename);

  FILE *file = fopen(file_path, "wb");
  if (!file) {
    fprintf(stderr, "failed to open file\n");
    return;
  }

  int bytes_received;
  // Read file content
  while (filesize > 0) {
    bytes_received = read(client_fd, buffer, sizeof(buffer));
    if (bytes_received <= 0) {
      fprintf(stderr, "failed to read file content\n");
      fclose(file);
      return;
    }

    fwrite(buffer, sizeof(char), bytes_received, file);
    filesize -= bytes_received;
  }
  fclose(file);

  // Send serial number back
  sprintf(buffer, "%d\n", serial_number);
  write(client_fd, buffer, strlen(buffer));

  close(client_fd);
}

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

  // Handle Zombie process: use SIGCHLD to prevent child process from becoming zombies
  signal(SIGCHLD, SIG_IGN);

  int serial_number = 0;

  while (1) {
    int client_fd = accept(sock_fd, NULL, NULL);

    if (client_fd < 0) {
      fprintf(stderr, "Error on accept\n");
      return 1;
    }

    serial_number += 1;

    int pid = fork();
    if (pid < 0) {
      fprintf(stderr, "Error on fork\n");
      close(sock_fd);
      continue;
    }

    if (pid == 0) {
      // This is the child process
      close(sock_fd);
      handle_client(client_fd, serial_number);
      close(client_fd);
      return 0;
    } else {
      // DEBUG:
      printf("new Connection [%d]\n", client_fd);
      // This is the parent process
      close(client_fd);
    }
  }

  return 0;
}

