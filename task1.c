#include <stdio.h>
#include <fcntl.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]){
	assert(argc == 2);
	int fd;
	int count;
	char buf[256];
	memset(buf, 0, 256);

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
