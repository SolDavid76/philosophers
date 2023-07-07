/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:18:59 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/07 15:09:06 by djanusz          ###   ########.fr       */
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
	int				eat;
	int				sleep;
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

/* utils.c */
int	ft_strslen(char **strs);
int	ft_atoi(char *str);

#endif
