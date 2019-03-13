#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <locale.h>

char *daemon_path={"/home/johnforjc/log/"};
int increment_file = 30;
char buf[100], path[100];
time_t t;
struct tm *timeptr,result;

int main() {
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
    exit(EXIT_FAILURE);
    }

    if (pid > 0) {
    exit(EXIT_SUCCESS);
    }
    // Daemon untuk mencatat log
    umask(0);

    // Mendapat SID dari kernel
    sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/johnforjc/log/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1) {
        if(increment_file==30){
            chdir(daemon_path);
            // Atur waktu
            setlocale(LC_ALL,"");           // LC_ALL 	Specifies all categories associated with the program's locale.
            t = time(NULL);
            timeptr = localtime(&t);

            // Nama folder
            strftime(buf,sizeof(buf), "%m:%d:%Y-%H:%M:%S", timeptr);
            strcpy(path, buf);

            // Buat folder
            mkdir(path, ACCESSPERMS);
            increment_file=1;
            chdir(path);
        }

        // Make a file
        pid_t touch_process;

        touch_process=fork();

        if(touch_process==0){
            // Membuat path dari file yang akan dibuat
            char name_of_file[100]={"log"};
            char number_of_file[2];

            // Membuat angka belakang log name file
            if(increment_file/10){
                number_of_file[0]='0' + (increment_file/10);
                number_of_file[1]='0' + (increment_file%10);
            }
            else{
                number_of_file[0]='0' + increment_file;
            }
            strcat(name_of_file, number_of_file);
            strcat(name_of_file, ".log");

            // Membuat parameter touch
            char *param_cp[]={"cp", "/var/log/syslog", name_of_file, NULL};
            execv("/bin/cp", param_cp);
        }

        increment_file++;

        sleep(60);
    }

    exit(EXIT_SUCCESS);
}
