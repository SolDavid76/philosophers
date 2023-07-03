/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:18:59 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/03 17:50:10 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <sys/time.h>

typedef struct s_rules
{
	int	nb_philo;
	int	die;
	int	eat;
	int	sleep;
	int	must_eat;
}		t_rules;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	unsigned long	start;
	unsigned long	wait;
	unsigned long	index;
	int				nb_philo;
	int				die;
	int				eat;
	int				sleep;
	int				must_eat;
	pthread_mutex_t	print;
	pthread_mutex_t	*all_forks;
	unsigned int	r_fork;
	unsigned int	l_fork;
	// pthread_mutex_t	r_fork;
	// pthread_mutex_t	l_fork;
	t_rules			*rules;
}					t_philo;

/* utils.c */
int	ft_strlen(char *str);
int	ft_strslen(char **strs);
int	ft_atoi(char *str);

#endif
