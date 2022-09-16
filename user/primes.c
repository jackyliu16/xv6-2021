#include "stddef.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// int fork():              :create a process; if child return 0, else return getpid()
// int exit(int status)     :Terminate current process, status report to wait();
// int exec(char * file, char* argv[])
//                          :Load a file and execute it with arguments; will only return when error
// int open(char *file, int flags)
//                          :Open a file, flags indicate read/write; return an fd(file descriptor)
// int read(int fd, char *buf, int n)
//                          :Read n byte into buf; reutrn number read; or 0 if end of file;
// int write(int fd, char *buf, int n)
//                          :Write n bytes from buf to file descriptor fd; return n;
// int close(int fd)        :Release open file fd
// int dup(int fd)          :return a new file descriptor ref to same file as fd
// int pipe(int p[])        :Create a pipe, put read/write file descriptors in p[0] and p[1];

// function define
void recursion();

int a2b[2];

void print_array(int array[], int length){ 
    printf("\n ===== PRINT ===== \n");
    for ( int i = 0 ; i < length ; i++ ) {
        printf("%d ", array[i]);
    }
    printf("\n");
}

// 还是有点不太理解pipe的作用机理，到底是将我们传入的那个数组作为指针指向了一个目标吗？
// 还是说这个数组起到了其他的用途
int main(int argc, char** argv){
    int buf[35];
    pipe(a2b);

    if (fork() == 0) {
        close(a2b[0]);
        for ( int i = 1 ; i < 36 ; i++ ) {
            write(a2b[1], &i, sizeof(int));
        }
        close(a2b[1]);
        exit(0);
    }
    else {
        wait(NULL);
        printf("fs: 1:%d, 2:%d", a2b[1], a2b[2]);
        recursion();
        printf("back to main");
        close(a2b[1]);
        read(a2b[0], buf, sizeof(buf));
        print_array(buf, sizeof(buf) / sizeof(int));
    }
    exit(0);
}

// 因为文件操作符是全局的，我感觉可以正常运行？如果说同时只有两个程序使用的话。
void recursion() {
    printf("fs: 1:%d, 2:%d", a2b[1], a2b[2]);
    int buf[35];

    close(a2b[1]);
    read(a2b[0], buf, sizeof(buf));
    close(a2b[0]);
    int cnt = 0;
    while ( buf[cnt] != 0 ) cnt++;
    // only leaf one number
    if ( cnt == 1 || cnt == 2 ) return;

    printf("cnt: %d\n", cnt);
    printf("turn: %d:", buf[0]);

    print_array(buf, sizeof(buf) / sizeof(int));

    if ( fork() == 0 ) {
        printf("into child\n");
        // child process
        close(a2b[0]);
        int i = buf[0] + 1;
        write(a2b[1], &i, sizeof(int));
        for ( int i = 1 ; i < cnt ; i++ ) {
            if ( buf[i] % buf[0] != 0 ) {
                // exact division
                printf("divide: %d \n", buf[i]);
                write(a2b[1], &buf[i], sizeof(int));
            }
        }
        close(a2b[1]);

        // try to read information
        // int buf2[35];
        // read(a2b[0], buf2, sizeof(buf2));
        // print_array(buf2, sizeof(buf2) / sizeof(int));

        // recursion();
        exit(0);
    }
    else {
        wait(NULL);
        printf("into parent");
    }
}

// int main(int argc, char **argv) {
//     int parent_fd[2];
//     pipe(parent_fd);
//     if (fork() == 0) {
//         close(parent_fd[0]);
//         for ( int i = 2 ; i < 36 ; i++ ) {
//             write(parent_fd[1], &i, sizeof(int));
//         }
//         close(parent_fd[1]);
//     }
//     else {
//         close(parent_fd[1]);
//         prime(parent_fd[0]);
//     }
//     wait(NULL);
//     exit(0);
// }

// void prime(int input_fd) {
//     int base;
//     // if last child
//     if (read(input_fd, &base, sizeof(int)) == 0) {
//         exit(0);
//     }
//     printf("prime %d\n", base);

//     // create new chile if not last
//     int p[2];
//     pipe(p);
//     if (fork() == 0) {
//         close(p[1]);
//         prime(p[0]);
//     }
//     else {
//         close(p[0]);
//         int n;
//         int eof;
//         do {
//             eof = read(input_fd, &n, sizeof(int));
//             if (n % base != 0) {
//                 write(p[1], &n, sizeof(int));
//             }
//         } while (eof);
//         close(p[1]);
//     }
//     wait(NULL);
//     exit(0);
// }