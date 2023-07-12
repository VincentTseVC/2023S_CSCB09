#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>

/* Basically cmd 2> file */

int main(int argc, char **argv)
{
  pid_t p;
  int fd_err;

  if (argc < 3) {
    fprintf(stderr, "Need at least filename and program.\n");
    return 1;
  }

  if ((fd_err = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1) {
    perror("cannot open");
    return 1;
  }
  if ((p = fork()) == -1) {
    perror("cannot fork");
    return 1;
  } else if (p != 0) {
    close(fd_err);

    int ws;
    wait(&ws);
    printf("wait status: %.4x\n", (unsigned)ws); // 4-digit hexadecimal

    if (WIFEXITED(ws)) {
        printf("exit status: %d\n", WEXITSTATUS(ws));
    } else if (WIFSIGNALED(ws)) {
        printf("signal: %d\n", WTERMSIG(ws));
    }

    return 0;
  } else {

    int backup_stderr = dup(2);

    if (dup2(fd_err, 2) == -1) {
        perror("cannot redirect stderr");
        return 1;
    }

    close(fd_err);

    // close backup_stderr on exec
    fcntl(backup_stderr, F_SETFD, FD_CLOEXEC);

    execvp(argv[2], &argv[2]);
    int exec_errno = errno;

    // Restore the original stderr.
    dup2(backup_stderr, 2);

    errno = exec_errno;
    perror("cannot exec");
    return exec_errno == ENOENT ? 127 : 126; // ENOENT = file not found
  }
}
