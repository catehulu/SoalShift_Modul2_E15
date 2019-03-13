#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main(){
    char *param_pkill={"pkill", "jlog", NULL};
    execv("usr/bin/pkill", param_pkill);
}
