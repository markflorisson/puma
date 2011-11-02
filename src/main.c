#include <stdio.h>
#include "puma.h"

int map[NX][NY];

int 
main(void)
{
	int nx, ny, puma_errno;

	if (puma_errno = readmap(map, "small.dat", &nx, &ny)) {
		printf("Error reading file: %s\n", puma_strerror(puma_errno));
	}

	printf("%d %d %d\n", map[0][0], map[49][49], map[17][17]);
	return 0;
}
