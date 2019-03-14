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

void refresh_time(){
    FILE *file;
    int position;
    file = fopen ("makan_enak.txt","a");
    if (file!=NULL)
    {
        fputs ("a",file);
    }
    fseeko(file,-1,SEEK_END);
    position = ftello(file);
    ftruncate(fileno(file), position);
}

int main(){
    pid_t pid, sid;

    //char *touch_refresh[] = {"touch", "haha.txt", NULL};
    
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

    if ((chdir("/home/catehulu/Documents/makanan/")) < 0) {
        exit(EXIT_FAILURE);
    }

    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    refresh_time();
    while(1){
       
        //FILE *pointer=popen("stat -c %x makan_enak.txt", "r");

        int minute_now, minute_last_seen, hour_now, hour_last_seen, second_now, second_last_seen;
        int year, day, month;
        int to_second_last_seen, to_second_now;
        int difference;

        //fscanf(pointer, "%d-%d-%d %d:%d:%d", &year, &day, &month, &hour_last_seen, &minute_last_seen, &second_last_seen);

        struct stat check;
        if (stat("makan_enak.txt", &check) == -1) {
            printf("salah!");
        }

        time_t t1;
        time_t t2;
        struct tm *timeptr;

        t1 = time(NULL);
        t2 = check.st_atime;
        
        timeptr = localtime(&t1);

        hour_now = timeptr->tm_hour;
        minute_now = timeptr->tm_min;
        second_now = timeptr->tm_sec;

        timeptr = localtime(&t2);

        hour_last_seen = timeptr->tm_hour;
        minute_last_seen = timeptr->tm_min;
        second_last_seen = timeptr->tm_sec;

        if(hour_last_seen==0) hour_last_seen=24;
        if(hour_now==0) hour_now=24;
        // printf("last seen %d:%d:%d\n",hour_last_seen,minute_last_seen,second_last_seen);
        // printf("now %d:%d:%d\n",hour_now,minute_now,second_now);
        to_second_last_seen=3600*hour_last_seen+60*minute_last_seen+second_last_seen;
        to_second_now=3600*hour_now+60*minute_now+second_now;
        // printf("last %d now %d\n",to_second_last_seen,to_second_now);
        difference = to_second_now - to_second_last_seen;

        if(difference<=30 && difference>=0){
            //printf("difference %d\n",difference);
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
        refresh_time();
    }
}
