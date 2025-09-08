#include "../include/philo.h"

t_philo *philo_init(t_rules *rules)
{
	t_philo *philos;
	int      i;

	philos = malloc(sizeof(t_philo) * rules->n_philo);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < rules->n_philo)
	{
		philos[i].id = i + 1;
		philos[i].times_eaten = 0;
		philos[i].last_meal = 0;
		philos[i].left_fork = &rules->forks[i];
		philos[i].right_fork = &rules->forks[(i + 1) % rules->n_philo];
		pthread_mutex_init(&philos[i].meal_mutex, NULL);
		philos[i].rules = rules;
		i++;
	}
	return (philos);
}