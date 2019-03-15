#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

int main() {
  char file[] = "elen.ku";
  char name[] = "www-data";
  char mode[] = "0777";
  int i;
  i = strtol(mode, 0, 8);
  struct stat per;
  struct passwd *user;
  struct group *group;
  
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

  if ((chdir("/home/catehulu/Desktop/sisop/SoalShift_modul2_E15/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    if(stat(file,&per) == 0){
        user = getpwuid(per.st_uid);
        group = getgrgid(per.st_gid);
        // printf("%s %s\n", user->pw_name, group->gr_name);
        if(strcmp(user->pw_name,group->gr_name) == 0 && strcmp(group->gr_name,name) == 0){
            // printf("right");
            // printf("%d\n",i);
            chmod(file,i);
            remove(file);
        }
    }
    sleep(3);
  }
  
  exit(EXIT_SUCCESS);
}