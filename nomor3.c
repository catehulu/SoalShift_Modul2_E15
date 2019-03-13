#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(){
	pid_t child_id;
	int status_touch;
	child_id=fork();

	if(child_id==0){
	int status_unzip;

	pid_t child_id2;

	child_id2=fork();

	if(child_id2==0){
		char *param_unzip[]={"unzip", "campur2.zip", NULL};
		execv("/usr/bin/unzip", param_unzip);
	}
	else{
		while( (wait(&status_unzip)) > 0);

			char *param_touch[]={"touch",  "daftar.txt", NULL};
			execv("/usr/bin/touch", param_touch);
		}
	}
	else{
		while( (wait(&status_touch)) > 0);

		FILE *rd, *out;
		int MAX_CHAR = 50;
		char *result = malloc( 51 * sizeof(char) );
		out= fopen("daftar.txt", "w");
		rd = popen("ls campur2/*.txt | awk 'BEGIN{FS=\"/\"}{print $2}'", "r"); // Pipe C dengan command di kernel

		while(fgets(result, MAX_CHAR, rd)){
			fprintf(out, "%s", result);
		}

		fclose(out);
		pclose(rd);

		char *param_chmod[]={"chmod", "666", "daftar.txt", NULL};
		execv("/bin/chmod", param_chmod);
	}
}
