#include "../include/philo.h"

int	ft_atoi(char *str)
{
	int	i;
	int	sign;
	int	n;

	i = 0;
	n = 0;
	sign = 1;
	while (str[i] != '\0')
	{
		if (str[i] < '0' || str[i] > '9')
			return (-1);
		i++;
	}
	i = 0;
	while (str[i] >= '0' && str[i] <= '9')
	{
		n = n * 10 + (str[i] - '0');
		i++;
	}
	return (n * sign);
}

long	get_time(void)
{
	struct	timeval current_time;
	long	time;
	
	gettimeofday(&current_time, NULL);
	time = current_time.tv_sec * 1000 + current_time.tv_usec / 1000;
	return (time);
}

void ft_usleep_ms(t_rules *rules, long ms)
{
    long start = get_time(); // get_time_ms debe devolver ms
    while (1)
    {
        // Si stop se ha activado, salimos pronto
        pthread_mutex_lock(&rules->stop_mutex);
        int stop = rules->stop;
        pthread_mutex_unlock(&rules->stop_mutex);
        if (stop)
            return;

        if ((get_time() - start) >= ms)
            break;
        usleep(500); // duerme 500 microsegundos (0.5 ms)
    }
}

void safe_print(t_rules *rules, int philo_id, const char *msg)
{
    pthread_mutex_lock(&rules->print_mutex);
    // Revisamos stop para no imprimir mensaje después del died final
    pthread_mutex_lock(&rules->stop_mutex);
    int stop = rules->stop;
    pthread_mutex_unlock(&rules->stop_mutex);

    if (!stop)
    {
        long timestamp = get_time() - rules->start_time;
        printf("%ld %d %s\n", timestamp, philo_id, msg);
    }
    pthread_mutex_unlock(&rules->print_mutex);
}

void cleanup(t_rules *rules, t_philo *philos)
{
    for (int i = 0; i < rules->n_philo; ++i)
    {
        pthread_mutex_destroy(&rules->forks[i].fork_mutex);
        pthread_mutex_destroy(&philos[i].meal_mutex);
    }
    pthread_mutex_destroy(&rules->print_mutex);
    pthread_mutex_destroy(&rules->stop_mutex);
    pthread_mutex_destroy(&rules->start_mutex);
    free(rules->forks);
    free(philos);
}