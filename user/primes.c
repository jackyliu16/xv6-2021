#include "stddef.h"
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

// DEBUG: 
// 感觉是由于在进入第一个子进程的迭代过程中出现了由于子进程没有完成

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

// 参考https://blog.csdn.net/weixin_48283247/article/details/120602005
// 文件描述符重定向(讲成映射比较好)
void
mapping(int n, int pd[])
{
  // 关闭文件描述符 n
  close(n);
  // 将管道的 读或写 端口复制到描述符 n 上
  // 即产生一个 n 到 pd[n] 的映射
  dup(pd[n]);
  // 关闭管道中的描述符
  close(pd[0]);
  close(pd[1]);
}

// 还是有点不太理解pipe的作用机理，到底是将我们传入的那个数组作为指针指向了一个目标吗？
// 还是说这个数组起到了其他的用途
// 
int main(int argc, char** argv){
    int fd[2];
    pipe(fd);
    if (fork() == 0) {
        mapping(1, fd);
        for ( int i = 2 ; i < 36 ; i++ ){ 
            write(1, &i, sizeof(int));
        }
    }
    else {
        wait(NULL);
        mapping(0, fd);
        recursion();
    }
    exit(0);
}

void recursion() {
    int buf[35], i = 0;
    while ( i < 35 ) {
        buf[i] = 0;
        i++;
    }

    int fd[2];
    if ( read(0, &buf, sizeof(buf))) {
        print_array(buf, sizeof(buf) / sizeof(int));
        int cnt = 0 ;
        while ( buf[cnt] != 0 && ++cnt < sizeof(buf) / sizeof(int) ) ;

        printf("cnt: %d\n", cnt);
    
        printf("primes %d\n", buf[0]);
        
        if ( cnt == 0 ) return;
        pipe(fd);
        if ( fork() == 0 ) {
            mapping(1, fd);
            for ( int i = 1 ; i < cnt ; i++ ) {
                if ( buf[i] % buf[0] != 0 ) {
                    int next = buf[i];
                    write(1, &next, sizeof(int));
                }
            }
        }
        else {
            wait(NULL);
            mapping(0, fd);
            recursion();
        }
    }
}

// 因为文件操作符是全局的，我感觉可以正常运行？如果说同时只有两个程序使用的话。
// void recursion() {
//     printf("pid: {%d}\n", getpid());

//     // read information
//     int buf[35], i = 0;

//     close(a2b[1]);
//     read(a2b[0], &buf, sizeof(buf));
//     print_array(buf, sizeof(buf) / sizeof(int));
//     // account the current number
//     int cnt = 0;    // the number right now
//     while ( buf[++cnt] != 0 ) ;
//     printf("cnt: %d\n", cnt);  

//     // if 1 => error ; 2 => less one
//     if ( cnt <= 2 ) return ;

//     if (fork() == 0) {
//         printf("into child process\n");
//         close(a2b[1]);
//         recursion();
//         printf("back child process\n");
//     }
//     else {
//         wait(NULL);
//         exit(0);
//     }
// }

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