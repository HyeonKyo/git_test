#include <stdio.h>
#include <unistd.h>

int main()
{
	int ret;
	char buf[2];
	char c;

	c = -1;
	ret = read(0, buf, 5);
	usleep(10);
	write(0, &c, 1);
	write(0, &c, 1);
	close(0);
	printf("ret : %d\n", ret);
	return 0;
}