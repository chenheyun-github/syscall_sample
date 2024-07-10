#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>


#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[])
{
        pid_t  child = 0;
        int    status = 0;
        int syscallID = 0;
        int returnValue = 0;
        int returnValue1 = 0;
        int flag = 0;


        child = fork();
        if(child == -1)
        {
                fprintf(stdout, "fork err\n");
        }
        
        if (child == 0)
        {
                /* 子进程 */
                return proc_child();
        }
        
        fprintf(stdout, "child pid = %d \n", child);

        
        while(1)
        {
                wait(&status); //等待并记录execve
        //if(WIFEXITED(status))
           // return 0;
        syscallID = ptrace(PTRACE_PEEKUSER, child, 8, &returnValue1);
        printf("Process executed system call ID = %ld/n", syscallID);
        ptrace(PTRACE_SYSCALL, child, NULL, NULL);
        while(1)
        {
            wait(&status); //等待信号
            //if(WIFEXITED(status)) //判断子进程是否退出
                //return 0;
            if(flag==0) //第一次(进入系统调用)，获取系统调用的参数
            {
                syscallID=ptrace(PTRACE_PEEKUSER, 8, NULL, &returnValue1);
                printf("Process executed system call ID = %ld ",syscallID);
                flag=1;
            }
            else //第二次(退出系统调用)，获取系统调用的返回值
            {
                returnValue=ptrace(PTRACE_PEEKUSER, 8, NULL, &returnValue1);
                printf("with return value= %ld/n", returnValue);
                flag=0;
            }
            ptrace(PTRACE_SYSCALL, child, 8, &returnValue1);
        }
    }
        return 0;
}

#define PTRACE_TEST_FILE "./ptrace.txt"
#define PTRACE_TEST_COUNT 10

int proc_child()
{
        int fd = -1;
        struct timeval tv;
        long long start_time = 0;
        int i = 0;

        gettimeofday(&tv, NULL);
        start_time = tv.tv_sec;
        start_time = start_time * 1000;
        start_time = start_time + (tv.tv_usec / 1000);
        
        printf("start_time: %lld\n", start_time);
        fd = open(PTRACE_TEST_FILE, O_RDWR | O_CLOEXEC | O_CREAT        );
        if (fd < 0) // 打开文件失败,则异常返回  
        {           
                printf("open file[%s] failed :%d\n", PTRACE_TEST_FILE, fd);
                return fd;         
        }          

        for (i = 0; i <PTRACE_TEST_COUNT; i++)
                write(fd , "aa", 2);

        close(fd);

        return 0;
}

