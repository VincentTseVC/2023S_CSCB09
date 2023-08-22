#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/select.h>

void worker1(const char *buf, size_t n);
void worker2(const char *buf, size_t n);

int main() {
  int s1, s2;
  // some code here.....

  char buf[1600];
  size_t num_read;

  int max_fd = s2;
  fd_set all_fds, listen_fds;
  FD_ZERO(&all_fds);
  FD_SET(s1, &all_fds);
  FD_SET(s2, &all_fds);

  while (FD_ISSET(s1, &all_fds) || FD_ISSET(s2, &all_fds)) {
    listen_fds = all_fds;
    select(max_fd+1, &listen_fds, NULL, NULL, NULL);

    if (FD_ISSET(s1, &listen_fds)) {
      if ((num_read = read(s1, buf, 1600)) <= 0) {
        close(s1);
        FD_CLR(s1, &all_fds);
      } else worker1(buf, num_read);
    }

    if (FD_ISSET(s2, &listen_fds)) {
      if ((num_read = read(s2, buf, 1600)) <= 0) {
        close(s2);
        FD_CLR(s2, &all_fds);
        max_fd = s1;
      } else worker2(buf, num_read);
    }
  }

}