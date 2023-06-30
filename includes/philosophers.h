/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:18:59 by djanusz           #+#    #+#             */
/*   Updated: 2023/06/30 16:57:26 by djanusz          ###   ########.fr       */
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
	pthread_t		id;
	unsigned long	start;
	unsigned long	now;
	unsigned long	index;
	int				die;
	int				eat;
	int				sleep;
	int				must_eat;
	t_rules			*rules;
}					t_philo;

/* main.c */
void	ft_exit(t_philo *philo, t_rules *rules, char *error_msg);

/* utils.c */
int	ft_strlen(char *str);
int	ft_strslen(char **strs);
int	ft_atoi(char *str);


#endif
