#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    // Cari PID nya executable file dari nomor 5a
    FILE *pid_of_process;
    pid_of_process=popen("pidof nomor5a", "r");

    int pid_of_jlog;
    fscanf(pid_of_process, "%d", &pid_of_jlog);

    kill(pid_of_jlog,SIGKILL);

    // langsung nama
    // char *param_pkill={"pkill", "jlog", NULL};
    // execv("/usr/bin/pkill", param_pkill);
}
