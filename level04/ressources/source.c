#define X86_ORIG_EAX 44
#define X86_EXECVE_SYSNO 11

#include <stdlib.h>
#include <stdio.h>
#include <sys/ptrace.h>
#include <sys/prctl.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <linux/fcntl.h>

int main(int argc, char **argv, char **envp)
{
    int stat_loc;
    char buffer[128];
    int child_eax;
    pid_t pid;

    pid = fork();
    memset(buffer, 0, sizeof(buffer));
    child_eax = 0;
    stat_loc = 0;

    if (pid)
    {
        /* Parent */

        do
        {
            wait(&stat_loc);
            if (WIFEXITED(stat_loc) || WIFSIGNALED(stat_loc))
            {
                puts("child is exiting...");
                return 0;
            }
            child_eax = ptrace(PTRACE_PEEKUSER, pid, X86_ORIG_EAX, 0);
        } while (child_eax != X86_EXECVE_SYSNO);
        puts("no exec() for you");
        kill(pid, SIGKILL);
    }
    else
    {
        /* Child */

        prctl(PR_SET_PDEATHSIG, SIGHUP);
        ptrace(PTRACE_TRACEME, 0, 0, 0);
        puts("Give me some shellcode, k");
        gets(buffer);
    }

    return 0;
}