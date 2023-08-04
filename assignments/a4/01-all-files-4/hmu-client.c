// Client program.

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

# define BUFF_SIZE 1024


// cmdline reminder: IP-address, portnum, username, filename
int main(int argc, char *argv[]) {
  if (argc != 5) {
    fprintf(stderr, "Invalid number of arguments\n");
    return 1;
  }

  char *ip = argv[1];
  int port = atoi(argv[2]); // TODO: change to short?
  char *username = argv[3];
  char *filename = argv[4];

  // Initialize socket
  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == -1) {
    fprintf(stderr, "Could not create socket\n");
    return 1;
  }

  struct sockaddr_in server;
  server.sin_addr.s_addr = inet_addr(ip);
  server.sin_family = AF_INET;
  server.sin_port = htons(port);

  // connect to the server
  if (connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) {
    fprintf(stderr, "Connection failed\n");
    return 1;
  }

  // Open file
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    fprintf(stderr, "Unable to open file\n");
    return 1;
  }

  // Determine the file size
  fseek(file, 0, SEEK_END);
  long filesize = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Send username
  write(sock, username, strlen(username)); // 不會有\0
  write(sock, "\n", 1);

  // Send filename
  write(sock, filename, strlen(filename)); // 不會有\0
  write(sock, "\n", 1);

  // Send filesize
  char size_str[11];
  sprintf(size_str, "%ld", filesize);
  write(sock, size_str, strlen(size_str)); // 不會有\0
  write(sock, "\n", 1);

  // Send file content
  char buff[BUFF_SIZE];
  size_t n;  // num_read
  while ((n = fread(buff, 1, BUFF_SIZE, file)) > 0) {
    if (write(sock, buff, n) < n) {
      fprintf(stderr, "Failed to send the file content\n");
      return 1;
    }
  }

  fclose(file);

  // Receive the Serial number
  char serial_number[11];
  memset(serial_number, 0, 11);
  n = 0;

  while (n < 11) {
    //                            &serial_number[n]
    ssize_t num_read = read(sock, serial_number + n, 1); // Read one btype at a time
    if (num_read < 0) {
      fprintf(stderr, "Error while receiving the serial number\n");
      return 1;
    } else if (num_read == 0) {
      fprintf(stderr, "Connection closed before the full serial number was received\n");
      return 1;
    }

    if (serial_number[n] == '\n') {
      break;
    }

    // Check for non-digit characters in the received serial number
    // Server may send: "HDERR\n"
    if (serial_number[n] < '0' || serial_number[n] > '9') {
      fprintf(stderr, "Received non-digit characters in the serial number\n");
      return 1;
    }

    n++;
  }

  // Check for server bug
  if (n > 10 || serial_number[n] != '\n') {
    fprintf(stderr, "Received invalid serial number\n");
    return 1;
  }

  // Output the received serial number
  serial_number[n] = '\0'; // Replace the newline with null character
  printf("%s\n", serial_number);

  close(sock);

  return 0;
}
