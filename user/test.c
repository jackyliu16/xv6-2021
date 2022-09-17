#include "stddef.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void print_array(int array[], int length){ 
    printf("\n ===== PRINT ===== \n");
    for ( int i = 0 ; i < length ; i++ ) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

void recursion();

int main(int argc, char** argv) {
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[0]);
        for ( int i = 2 ; i < 36 ; i++ ) {
            write(fd[1], &i, sizeof(int));
        }
        close(fd[1]);
    }
    else {
        wait(NULL);
        recursion(fd[0]);
    }
    exit(0);
}

void recursion(int in) {
/*
    1. read information
    2. write information
    3. come back
    int fd[2];
    pipe(fd);
*/
    
    int buf[34];
    int i = 0;
    // while (cnt++ < sizeof(buf) / sizeof(int)) buf[cnt] = 0;
    while ( i < sizeof(buf) / sizeof(int)) {
        buf[i] = 0;
        i++;
    }
    read(in, buf, sizeof(buf));
    close(in);                      // Close to prevent overflow

    /* count the avaliable number( useless is 0 ) */
    int cnt = 0;
    while ( buf[++cnt] != 0 ) ;    
    // printf("cnt: %d\n", cnt);
    // print_array(buf, sizeof(buf) / sizeof(int));
    printf("prime %d\n", buf[0]);
    if ( cnt < 2 ) return ;

    // print_array(buf, cnt);

    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        close(fd[0]);
        for ( int i = 1 ; i < cnt ; i++ ) {
            if ( buf[i] % buf[0] != 0 ) {
                int t = buf[i];
                write(fd[1], &t, sizeof(int));
            }
        }
        close(fd[1]);
        exit(0);
    }
    else {
        wait(NULL);
        recursion(fd[0]);
        exit(0);
    }
}