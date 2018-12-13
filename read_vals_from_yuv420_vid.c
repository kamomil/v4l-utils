#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	if(argc < 8) {
		printf("uage: <video file name> <width> <height> <plane [l|r|b]> <frame_num> <r> <c>\n");
		_exit(1);
	}
	int fd = open(argv[1], O_RDONLY, 0);
	if(fd<1) {
		printf("could not open flie\n");
		_exit(1);
	}
	unsigned int w = atoi(argv[2]);
	unsigned int h = atoi(argv[3]);

	char plane = argv[4][0];

	char frame_num = atoi(argv[5]);
	unsigned int r = atoi(argv[6]);
	unsigned int c = atoi(argv[7]);

	unsigned int frame_size = w*h*3/2;
	unsigned char* frame = malloc(frame_size);

	for(int i=0;i<frame_num;i++) {
		int ret = read(fd,frame,frame_size);
		if(ret != frame_size) {
			printf("error reading frame\n");
			return -1;
		}
	}
	if(plane != 'l') {
		int ret = read(fd,frame,w*h);
		if(ret != w*h) {
			printf("error reading luma\n");
			return -1;
		}
	}
	if(plane == 'r'){
		int ret = read(fd,frame,w*h/4);
		if(ret != w*h/4) {
			printf("error reading cb\n");
			return -1;
		}
	}

	if(plane != 'l') {
		w /= 2;
		h /= 2;
	}


	for(int i = 0;i<r; i++) {
		int ret = read(fd,frame,w);
		if(ret != w) {
			printf("error reading raw\n");
			return -1;
		}
	}
	int ret = read(fd, frame, c+1);
	if(ret != c+1) {
		printf("error reading final raw\n");
		return -1;
	}

	printf("the value in plane '%c', frame %u, raw %u col %u is %u (0x%02x)\n",plane, frame_num, r,c, frame[c], frame[c]);

	return 0;
}
