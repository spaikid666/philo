#include "philo.h"

int	main (int argc, char** argv)
{
	(void)argv;
	if (argc < 4)
	{
		instructions();
		return (1);
	}
	return 0;
}
