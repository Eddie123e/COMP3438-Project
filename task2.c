#include <assert.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
	assert(argc == 2);
	int fd;
	int count;
	char buf[256];
	memset(buf, 0, 256);

	char cur_state = '0';
	int i = 0;
	char input = argv[1][i];
	while(input != '\0'){
		switch(cur_state){
			case '0':
				switch(input){
					case 'a':
					case 'b':
						cur_state = '0';
						break;
					case 'c':
						cur_state = '1';
						break;
					default:
						cur_state = 'f';
				}
				break;
			case '1':
				if(input == 'c') cur_state = '1';
				else cur_state = 'f';
				break;
			case 'f':
				cur_state = 'f';
				break;
			default:
				fprintf(stderr, "BUG\n");
				exit(-1);
		}
		i++;
		input = argv[1][i];
		
	}
	if((cur_state == '0') || (cur_state == '1')) printf("Pattern matched!\n");
	else{
		printf("Pattern failed to match.\n");
		return 0;
	}
	fd = open("/dev/echoer", O_RDWR);
	if(fd == -1){
		printf("Fail to open device echoer.\n");
		return -1;
	}
	strcpy(buf, argv[1]);
	write(fd, buf, strlen(buf));
	count = read(fd, buf, 255);
	printf("Echoed from kernel: %s.\n", buf);
	close(fd);
	return 0;
}	
	


