#include <unistd.h>
#include <stdio.h>

int main()
{
	int ret;
	char buf[2];

	buf[1] = 0;
	ret = read(0, buf, 1);
	printf("%d\n", ret);
	return (0);
}
