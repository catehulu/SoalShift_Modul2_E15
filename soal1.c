#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#include <string.h>

int main() {
  pid_t pid, sid;
  DIR *directory;
  char *folder = malloc( 200 * sizeof(char) );
  char *filename;
  char *tmp;
  char *newname = malloc( 100 * sizeof(char) ); 
  int lenght;
  char ext[] = ".png";
  struct dirent *dir;

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
    directory = opendir(".");
    if (directory) {
        while ((dir = readdir(directory)) != NULL) {
            filename = dir->d_name;
            tmp = strrchr(filename,'.');
            if (!tmp || tmp == filename) {
                continue;
            }
            if(strcmp(strrchr(filename,'.'),ext) == 0) {
                strcpy(folder,"/home/catehulu/modul2/gambar/");
                lenght = strlen(filename) - 4;
                // printf("fname %s\nlen %d\n", filename,lenght);
                strncpy(newname, filename, lenght);
                // printf("new %s\n", newname);
                strcat(newname, "_grey.png");
                // printf("new grey %s\n", newname);
                strcat(folder, newname);
                // printf("dir %s\n", folder);
                rename(filename,folder);
                memset(newname, '\0', sizeof(newname));
                memset(folder, '\0', sizeof(folder));
            }
        }
        closedir(directory);
    }
    sleep(5);
  }
  exit(EXIT_SUCCESS);
}