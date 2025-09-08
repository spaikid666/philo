#ifndef PHILO_H

# define PHILO_H

# include <stdlib.h>
# include <unistd.h>
# include <stdio.h>
# include <fcntl.h>
# include <limits.h>
# include <sys/wait.h>
# include <sys/resource.h>
# include <sys/time.h>
# include <pthread.h>

/************ Instructions ************/
void	instructions (void);

/************** Structs ***************/
/* Fork Struct */
typedef struct	s_fork
{
	int				id;
	int				in_use;
	pthread_mutex_t	fork_mutex;
}	t_fork;

/* Rules Struct */
typedef struct	s_rules
{
	int				n_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	long			start_time;
	int				stop;
	pthread_mutex_t	print_mutex; // Mutex to serialize printing to not corrupt the printing
	pthread_mutex_t start_mutex;
	pthread_mutex_t	stop_mutex;
	t_fork			*forks; // Array of fork mutexes (size = n_philo)
}	t_rules;

/* Philosopher Struct */
typedef struct	s_philo
{
	int				id;
	int				times_eaten;
	long			last_meal;
	pthread_t		thrd;
	pthread_mutex_t	meal_mutex;
	t_fork			*left_fork;
	t_fork			*right_fork;
	t_rules			*rules;
}	t_philo;

/************** Functions *************/
/* Philo */
void	*death_checker(void *args);
void	*routine(void *args);

/* Utils Data */
int		parse_args(char** argv, t_rules *rules);

/* Utils Functions */
int		ft_atoi(char *str);
long	get_time(void);
void	ft_usleep_ms(t_rules *rules, long ms);
void	safe_print(t_rules *rules, int philo_id, const char *msg);
void	cleanup(t_rules *rules, t_philo *philos);

/* Utils Philo */
t_philo *philo_init(t_rules *rules);
void	free_philo(t_philo *philo);

#endif