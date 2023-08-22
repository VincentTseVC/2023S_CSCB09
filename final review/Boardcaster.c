#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
  int n;
  char *filtername;
  char *filterargs[n];

  int fanout[n][2], fanin[n][2];

  // pipe creation
  for (int i = 0; i < n; i++) {
    pipe(fanout[i]);
    pipe(fanin[i]);
  }

  int broadcaster = fork();
  if (broadcaster == 0) {
    // broadcaster code

    // close all UN-NEEDED pipe ends
    for (int i = 0; i < n; i++) {
      close(fanout[i][0]);
      close(fanin[i][0]);
      close(fanin[i][1]);
    }

    char buff[4096];
    int num_read;
    while (1) {
      if ((num_read = read(0, buff, 4096)) <= 0) {
        // close all write-end before exit
        for (int i = 0; i < n; i++)
          close(fanout[i][1]);
        exit(0);
      }

      // copy and paste the all n filters
      for (int i = 0; i < n; i++)
        write(fanout[i][1], buff, num_read);
    }
  }

  int collector = fork();
  if (collector == 0) {
    // collector code

    // close all UN-NEEDED pipe ends
    for (int i = 0; i < n; i++) {
      close(fanin[i][1]);
      close(fanout[i][0]);
      close(fanout[i][1]);
    }

    double number, sum;
    while (1) {
      sum = 0;
      for (int i = 0; i < n; i++) {
        if (read(fanin[i][0], &number, sizeof(double)) <= 0) {
          // close all read-end before exit
          for (int j = 0; j < n; j++)
            close(fanin[j][0]);
          exit(0);
        }
        sum += number;
      }
      printf("%g\n", sum);
    }
  }

  int filter;
  for (int i = 0; i < n; i++) {
    filter = fork();

    if (filter == 0) {
      dup2(fanout[i][0], 0);
      dup2(fanin[i][1], 1);

      // close all pipes
      for (int j = 0; j < n; j++) {
        close(fanin[j][0]);
        close(fanin[j][1]);
        close(fanout[j][0]);
        close(fanout[j][1]);
      }

      execlp(filtername, filtername, filterargs[i], (char *) NULL);
    }
  }

  // Parent

  // close all pipes
  for (int j = 0; j < n; j++) {
    close(fanin[j][0]);
    close(fanin[j][1]);
    close(fanout[j][0]);
    close(fanout[j][1]);
  }

  // wait for all children (n+2)
  for (int i = 0; i < n+2; i++) {
    wait(NULL);
  }

  return 0;
}