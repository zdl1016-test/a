#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

//void getFileCreationTime(char *path) {
    //struct stat attr;
    //stat(path, &attr);
    //printf("Last modified time: %s", ctime(&attr.st_mtime));
//}

int main()
{
    struct stat st;
    memset(&st, 0, sizeof(st));
    stat("1.txt", &st);

    printf("create time : %u, modify time : %u\n", st.st_ctime, st.st_mtime);

    return 0;

}
