#include "../include/philo.h"

int	parse_args(char** argv, t_rules *rules)
{
	int	i;

	if (ft_atoi(argv[1]) == -1 || ft_atoi(argv[2]) == -1 || \
	ft_atoi(argv[3]) == -1 || ft_atoi(argv[4]) == -1)
		return (1);
	rules->n_philo = ft_atoi(argv[1]);
	rules->time_to_die = ft_atoi(argv[2]);
	rules->time_to_eat = ft_atoi(argv[3]);
	rules->time_to_sleep = ft_atoi(argv[4]);
	rules->start_time = get_time();
	pthread_mutex_init(&rules->print_mutex, NULL);
	pthread_mutex_init(&rules->stop_mutex, NULL);
	pthread_mutex_init(&rules->start_mutex, NULL);
	rules->forks = malloc(sizeof(t_fork) * rules->n_philo);
	if (!rules->forks)
		return (2);
	i = 0;
	while (i < rules->n_philo)
	{
   		rules->forks[i].id = i;
    	rules->forks[i].in_use = 0;
    	pthread_mutex_init(&rules->forks[i].fork_mutex, NULL);
	}

	return (0);
}
