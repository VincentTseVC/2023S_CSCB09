#include <stdio.h>
// Please use the man pages of opendir etc to find out more #include's you need.
#include <dirent.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // If the user gives 1 cmdline argument:
    //   argc=2, argv[1] is the argument.
    // If the user gives 2 cmdline arguments:
    //   argc=3, argv[1] is the 1st argument, argv[2] the 2nd.

    DIR *dir;
    struct dirent *sub;

    if (argc < 2) {
        // perror("....");
        exit(1);
    }

    if ((dir = opendir(argv[1])) == NULL) {
        perror("failed to open\n");
        exit(1);
    }

    while ((sub = readdir(dir)) != NULL) {

        if (argc >= 3) {
            if (sub->d_name[0] != argv[2][0])
                printf("%s\n", sub->d_name);
        }
        else
            printf("%s\n", sub->d_name);
    }

    closedir(dir);
    return 0;
}
