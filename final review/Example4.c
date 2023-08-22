#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>

void do_left(int fd_read, int fd_write) {
  char buf[512];

  int num_read;

  int max_fd = fd_read;
  fd_set all_fds, listen_fds;
  FD_ZERO(&all_fds);
  FD_SET(0, &all_fds);
  FD_SET(fd_read, &all_fds);

//  fd_set fds;

  while (1) {
//    FD_ZERO(&fds);
//    FD_SET(0, &fds);
//    FD_SET(fd_read, &fds);
//    select(rd_read+1, &fds, NULL, NULL, NULL);
    listen_fds = all_fds;
    select(max_fd+1, &listen_fds, NULL, NULL, NULL);

    if (FD_ISSET(0, &listen_fds)) {
      if ((num_read = read(0, buf, 512)) <= 0) // EOF
        exit(0);
      else {
        for (int i = 0; i < num_read; i++)
          buf[i] = toupper(buf[i]);

        write(fd_write, buf, num_read);
      }
    }

    if (FD_ISSET(fd_read, &listen_fds)) {
      if ((num_read = read(fd_read, buf, 512)) <= 0)
        exit(0);
      else
        write(1, buf, num_read);
    }
  }
}

void leftright(void) {
  // parent begins
  int ltr [2];
  int rtl [2];

  pipe(ltr);
  pipe(rtl);

  pid_t left = fork ();
  if (left == 0) {
    // left child code , part (a)
    close(rtl[1]);
    close(ltr[0]);

    do_left(rtl[0], ltr[1]);

    close(rtl[0]);
    close(ltr[1]);

  } else {
    pid_t right = fork ();
    if (right == 0) {
      // right child code , part (b)
      close(ltr[1]);
      close(rtl[0]);

      dup2(ltr[0], 0);
      dup2(rtl[1], 1);

      close(ltr[0]);
      close(rtl[1]);

      execlp (" right" , " right" , (char *) NULL);
      exit (1);
    } else {
      // parent code after both forks , part (c)
      close(ltr[0]);
      close(ltr[1]);

      close(rtl[0]);
      close(rtl[1]);

      wait(NULL);
      wait(NULL);
    }
  }
}