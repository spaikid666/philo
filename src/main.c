#include "../include/philo.h"

int	main (int argc, char** argv)
{
	t_rules		rules;
	t_philo		*philos;
	pthread_t	monitor;
	int			i;

	//Parsing and data formating
	if (argc < 5)
		return (instructions(), 1);
	if (parse_args(argv, &rules) == 1)
		return (write(2, "Error: Arguments must be possitive numbers.\n", 45));
	else if (parse_args(argv, &rules) == 2)
		return (write(2, "Error: Memory assigment failed for forks.\n", 43));
	
	philos = philo_init(&rules);
	pthread_mutex_lock(&philos->rules->start_mutex);
	i = 0;
	while (i < rules.n_philo)
	{
		pthread_create(&philos[i].thrd, NULL, routine, &philos[i]);
		i++;
	}
	pthread_create(&monitor, NULL, death_checker, philos);
	rules.start_time = get_time();
	pthread_mutex_unlock(&rules.start_mutex);
	pthread_join(monitor, NULL);
	i = 0;
	while (i < rules.n_philo)
	{
		pthread_join(philos[i].thrd, NULL);
	}
	i = 0;
	while (i < rules.n_philo)
	{
		pthread_mutex_destroy(&rules.forks[i].fork_mutex);
		pthread_mutex_destroy(&philos[i].meal_mutex);
	}
	cleanup(&rules, philos);
	return 0;
}
