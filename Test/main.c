#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#define BUF_LEN 80


int main()
{
	int time;
	float dist;
	int file_desc;
	//int ret_val;
	char tmp[BUF_LEN];
	char tm1[BUF_LEN];

	/* Open dummy file. */
	file_desc = open("/dev/Driver", O_RDWR);

	if (file_desc < 0)
	{
		printf("Error, 'dummy' not opened\n");
		return -1;
	}

	printf("Unesti 1 za pokretanje \n");
	scanf("%c", &tm1[0]);
	fflush(stdin);

	float max_speed = 100, speed;

	while (1)
	{
		write(file_desc, tm1, BUF_LEN);

		read(file_desc, tmp, BUF_LEN);

		printf("\n");

		time = atoi(tmp);
		dist = time * 34000 / 2000000.0;

		if ((dist > 20) && (dist < 100))
		{
			speed = max_speed * (1 - (20 / dist));	// linear reduce speed
		}
		if (dist < 20)
		{
			speed = 0;
		}

		printf("brzina  je: %f\n", speed);
		printf("vreme: %d\n", time);
		printf("udaljenost od objekta: %f\n", dist);
		sleep(1);
	}
	/* Close dummy file. */

	tmp[0] = '0';

	close(file_desc);

	return 0;
}
