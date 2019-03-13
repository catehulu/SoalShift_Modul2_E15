#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <locale.h>

int file=1;

int main(){
    pid_t pid, sid;

    pid = fork();

    if (pid < 0) {
        exit(EXIT_FAILURE);
    }

    if (pid > 0) {
        exit(EXIT_SUCCESS);
    }

    umask(0);

    sid = setsid();

    if (sid < 0) {
        exit(EXIT_FAILURE);
    }

    if ((chdir("/home/johnforjc/Documents/makanan/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    while(1){
        FILE *pointer=popen("stat -c %x makan_enak.txt", "r");

        int minute_now, minute_last_seen, hour_now, hour_last_seen, second_now, second_last_seen;
        int year, day, month;
        int to_second_last_seen, to_second_now;

        fscanf(pointer, "%d-%d-%d %d:%d:%d", &year, &day, &month, &hour_last_seen, &minute_last_seen, &second_last_seen);

        time_t t;
        struct tm *timeptr,result;
        setlocale(LC_ALL,"");           // LC_ALL 	Specifies all categories associated with the program's locale.
        t = time(NULL);
        timeptr = localtime(&t);

        hour_now = timeptr->tm_hour;
        minute_now = timeptr->tm_min;
        second_now = timeptr->tm_sec;

        if(hour_last_seen==0) hour_last_seen=24;
        if(hour_now==0) hour_now=24;

        to_second_last_seen=3600*hour_last_seen+60*minute_last_seen+second_last_seen;
        to_second_now=3600*hour_now+60*minute_now+second_now;

        if(to_second_now- to_second_last_seen<=30){
            pid_t process_touch;
            process_touch = fork();

            if(process_touch==0){
                char file_name[100];
                sprintf(file_name, "makan_sehat%d.txt", file);
                char *param_touch[] = {"touch", file_name, NULL};
                execv("/usr/bin/touch", param_touch);
            }else file++;
        }

        sleep(5);
    }
}
