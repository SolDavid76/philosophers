/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:18:59 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:33:55 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_philo
{
	int				id;
	pthread_t		th;
	unsigned long	die;
	unsigned long	eat;
	unsigned long	sleep;
	unsigned long	start;
	unsigned long	now;
	int				end;
	unsigned long	last_eat;
	int				must_eat;
	pthread_mutex_t	check_last_eat;
	pthread_mutex_t	check_must_eat;
	pthread_mutex_t	check_end;
	pthread_mutex_t	*print;
	pthread_mutex_t	*r_fork;
	pthread_mutex_t	*l_fork;
}					t_philo;

typedef struct s_rules
{
	unsigned long	start;
	int				nb_philo;
	int				die;
	int				eat;
	int				sleep;
	int				must_eat;
	int				end;
	pthread_mutex_t	print;
	pthread_mutex_t	*all_forks;
	t_philo			*philo;
}					t_rules;

/* actions.c */
void			unlock_fork(pthread_mutex_t *l_fork, pthread_mutex_t *r_fork);
int				lock_fork(t_philo *philo);
int				eating(t_philo *philo);
int				thinking(t_philo *philo);
void			*routine(void *arg);

/* checkers.c */
int				check_end(t_philo *philo);
int				check_must_eat(t_philo *philo);
unsigned long	check_last_eat(t_philo *philo);

/* monitoring.c */
int				print_action(t_philo *philo, char *msg);
void			start_threads(t_rules *rules);
int				someone_died(t_philo *philo, int nb_philo, unsigned long die);
int				evryone_eat(t_philo *philo, int nb_philo);
int				monitoring(t_philo *philo, int nb_philo, unsigned long die);

/* parsing.c */
int				mutex_init(t_rules *rules);
t_rules			*philo_init(t_rules *rules);
t_rules			*parsing(char **args);

/* utils.c */
unsigned long	get_time(void);
int				ft_strslen(char **strs);
int				ft_atoi(char *str);

/* main.c */
int				ft_exit(t_rules *rules);

#endif
