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

typedef struct s_philo
{
	int				id;
	pthread_t		thread;
	int				eat;
	int				sleep;
	int				must_eat;
	int				died;
	int				someone_died;
	unsigned long	start;
	unsigned long	now;
	unsigned long	last_eat;
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
	pthread_mutex_t	print;
	pthread_mutex_t	*all_forks;
	t_philo			*philo;
}					t_rules;

/* utils.c */
int	ft_strlen(char *str);
int	ft_strslen(char **strs);
int	ft_atoi(char *str);

#endif
