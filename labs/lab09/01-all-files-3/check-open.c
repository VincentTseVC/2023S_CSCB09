#include <fcntl.h>
#include <stdio.h>

int main(void) {
  int r = 0;
  // Checking FDs from 3 to 20 ought to be enough. (Famous last words.)
  for (int fd = 3; fd < 20; fd++) {
    // fcntl(fd, F_GETFL) returns flags such as read-only, write-only, etc.
    // That is, if fd is open at all.
    //
    // So I expect it to fail and return -1.
    if (fcntl(fd, F_GETFL) != -1) {
      printf("%d not closed\n", fd);
      r = 1;
    }
  }
  if (r == 0) {
    printf("AOK\n");
  }
  return r;
}
