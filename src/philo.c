#include "../include/philo.h"

void *death_checker(void *arg)
{
    t_philo *philos = (t_philo *)arg;
    t_rules *rules = philos[0].rules;

    while (1)
    {
        int i = 0;
        while (i < rules->n_philo)
        {
            pthread_mutex_lock(&philos[i].meal_mutex);
            long last = philos[i].last_meal;
            pthread_mutex_unlock(&philos[i].meal_mutex);

            long now = get_time();
            if ((now - last) > rules->time_to_die)
            {
                // Print once and set stop
                pthread_mutex_lock(&rules->print_mutex);
                long ts = now - rules->start_time;
                printf("%ld %d died\n", ts, philos[i].id);
                pthread_mutex_unlock(&rules->print_mutex);

                pthread_mutex_lock(&rules->stop_mutex);
                rules->stop = 1;
                pthread_mutex_unlock(&rules->stop_mutex);
                return NULL;
            }
            i++;
        }
        usleep(1000); // check 1ms
    }
}

void *routine(void *arg)
{
    t_philo *ph = (t_philo *)arg;
    t_rules *r = ph->rules;

    // BARRIER: wait until main unlocks start_mutex
    pthread_mutex_lock(&r->start_mutex);
    pthread_mutex_unlock(&r->start_mutex);

    while (1)
    {
        // check stop
        pthread_mutex_lock(&r->stop_mutex);
        if (r->stop)
        {
            pthread_mutex_unlock(&r->stop_mutex);
            break;
        }
        pthread_mutex_unlock(&r->stop_mutex);

        // take forks (order parity to avoid deadlock)
        if (ph->id % 2 == 0)
        {
            pthread_mutex_lock(&ph->right_fork->fork_mutex);
            safe_print(r, ph->id, "has taken a fork");
            pthread_mutex_lock(&ph->left_fork->fork_mutex);
            safe_print(r, ph->id, "has taken a fork");
        }
        else
        {
            usleep(500); // small stagger
            pthread_mutex_lock(&ph->left_fork->fork_mutex);
            safe_print(r, ph->id, "has taken a fork");
            pthread_mutex_lock(&ph->right_fork->fork_mutex);
            safe_print(r, ph->id, "has taken a fork");
        }

        // eating
        safe_print(r, ph->id, "is eating");
        pthread_mutex_lock(&ph->meal_mutex);
        ph->last_meal = get_time();
        ph->times_eaten++;
        pthread_mutex_unlock(&ph->meal_mutex);

        ft_usleep_ms(r, r->time_to_eat);

        // put down forks
        pthread_mutex_unlock(&ph->left_fork->fork_mutex);
        pthread_mutex_unlock(&ph->right_fork->fork_mutex);

        // sleep
        safe_print(r, ph->id, "is sleeping");
        ft_usleep_ms(r, r->time_to_sleep);

        // think
        safe_print(r, ph->id, "is thinking");
    }
    return NULL;
}
