#pragma once

#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/time.h>
#include <termios.h>

#define MKFLAG(which) \
static int io_tio_set_flag_##which(int fd, int val, int on, int *old) \
{ struct termios tio; \
    if (tcgetattr(fd,&tio)) return -1; \
    if (old) *old=(tio.which & (val)); \
    if (on) tio.which |= (val); \
    else tio.which &= ~(val); \
    if (tcsetattr(fd,TCSADRAIN,&tio)) return -1; \
    return 0; \
} \
static int io_tio_get_flag_##which(int fd, int bit, int *value) \
{ struct termios tio; \
    if (tcgetattr(fd,&tio)) return -1; \
    *value=(tio.which & (bit)); \
    return 0; \
}
MKFLAG(c_lflag)


// int kbhit(void)
// {
//   struct termios oldt, newt;
//   int ch;
//   int oldf;
 
//   tcgetattr(STDIN_FILENO, &oldt);
//   newt = oldt;
//   newt.c_lflag &= ~(ICANON | ECHO);
//   tcsetattr(STDIN_FILENO, TCSANOW, &newt);
//   oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
//   fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
 
//   ch = getchar();
 
//   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
//   fcntl(STDIN_FILENO, F_SETFL, oldf);
 
//   if(ch != EOF)
//   {
//     ungetc(ch, stdin);
//     return 1;
//   }
 
//   return 0;
// }

static int io_charavail(int fd)
{
	fd_set set;
	struct timeval zeit;
	int ret;
	int tty=1;	
	int old_ICANON;
	FD_ZERO(&set);
	FD_SET(fd,&set);
	zeit.tv_sec  = 0;
	zeit.tv_usec = 0;
	if (-1==io_tio_set_flag_c_lflag(fd,ICANON,0,&old_ICANON)) {
		if (errno==EINVAL || errno==ENOTTY) 
			tty=0;
		else {
			perror("ICANON");
			return -1;
		}
	}

		
	while (1) {
		ret=select(fd+1,&set,0,0,&zeit);
		if (ret>=0) break;
		if (errno==EINTR) continue;
		perror("select");
		break;
	}
	if (tty)
		io_tio_set_flag_c_lflag(fd,ICANON,old_ICANON,NULL);
	return ret;
}

static int kbhit(){
	return io_charavail(0);
}
