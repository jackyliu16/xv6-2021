#include "kernel/types.h"
#include "user/user.h"
#include "stddef.h"

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

// 我有点完全没搞明白这个有关于child pid的判定是如何进行的


int main(int argc, char** argv){
    // 有点不太理解此处的文件描述符的意思，暂且先跟着xv6book来
    // 由于管道主要用于单方向通讯，因此需要存在两条管道才能实现双方通信
    int p2c[2];
    int c2p[2];

    pipe(p2c);
    pipe(c2p);

    char buf[20];

    if (fork() == 0) {
        // child process
        read(p2c[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
        write(c2p[1], "pong", strlen("pong"));
    } else {
        // parent process
        write(p2c[1], "ping", strlen("ping"));
        wait(NULL);
        read(c2p[0], buf, sizeof(buf));
        printf("%d: received %s\n", getpid(), buf);
    }
    exit(0);
}