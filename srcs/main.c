/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:23:45 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/03 18:10:15 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

// rules->print = malloc(sizeof(pthread_mutex_t));
// if (!rules->print)
// 	return(-1);
// if (pthread_mutex_init(rules->print, NULL))
// 	return (-1);
int	mutex_init(t_rules *rules)
{
	int	i;

	rules->all_forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->all_forks)
		return (-1);
	if (pthread_mutex_init(&rules->print, NULL))
		return (-1);
	i = -1;
	while (++i < rules->nb_philo)
	{
		if (pthread_mutex_init(&rules->all_forks[i], NULL))
			return (-1);
	}
	return (0);
}

t_rules	*philo_init(t_rules *rules)
{
	int		i;

	rules->philo = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philo || mutex_init(rules))
		return (write(1, "NOT ENOUGH MEMORY\n", 18), NULL);
	i = -1;
	rules->start = get_time();
	while (++i < rules->nb_philo)
	{
		rules->philo[i].id = i + 1;
		rules->philo[i].start = rules->start;
		rules->philo[i].eat = rules->eat;
		rules->philo[i].sleep = rules->sleep;
		rules->philo[i].must_eat = rules->must_eat;
		rules->philo[i].print = &rules->print;
		rules->philo[i].r_fork = &rules->all_forks[(i + 1) % rules->nb_philo];
		rules->philo[i].l_fork = &rules->all_forks[i];
	}
	return (rules);
}

t_rules	*parsing(char **args)
{
	t_rules	*res;

	if (ft_strslen(args) != 4 && ft_strslen(args) != 5)
		return (write(1, "Wrong number of args\n", 21), NULL);
	res = malloc(sizeof(t_rules));
	if (!res)
		return (write(1, "NOT ENOUGH MEMORY\n", 18), NULL);
	res->nb_philo = ft_atoi(args[0]);
	res->die = ft_atoi(args[1]);
	res->eat = ft_atoi(args[2]);
	res->sleep = ft_atoi(args[3]);
	if (res->nb_philo < 1 || res->die < 1 || res->eat < 1 || res->sleep < 1)
		return (write(1, "Wrong input\n", 12), NULL);
	if (!args[4])
		res->must_eat = -1;
	else
	{
		res->must_eat = ft_atoi(args[4]);
		if (res->must_eat < 1)
			return (write(1, "Wrong input in must eat\n", 24), NULL);
	}
	return (philo_init(res));
}

// void	wait(t_philo *philo, unsigned long time, unsigned long tmp)
// {
// 	while (42)
// 	{
// 		philo->now = get_time();
// 		if (philo->now - tmp >= time)
// 			return ;
// 	}
// }

void	*routine(void *arg)
{
	t_philo *philo = arg;

	while (42)
	{
		/* start of eating */
		pthread_mutex_lock(philo->l_fork);
		pthread_mutex_lock(philo->print);
		printf("%lu %d has taken the left fork\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(philo->print);
		pthread_mutex_lock(philo->r_fork);
		pthread_mutex_lock(philo->print);
		printf("%lu %d has taken the right fork\n", get_time() - philo->start, philo->id);
		printf("%lu %d is eating\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(philo->print);
		philo->now = get_time();
		philo->last_eat = philo->now;
		while (get_time() - philo->now < (unsigned long)(philo->eat))
		{
		}
		pthread_mutex_unlock(philo->r_fork);
		pthread_mutex_unlock(philo->l_fork);
		/* end of eating */
		/* start of sleeping */
		pthread_mutex_lock(philo->print);
		printf("%lu %d is sleeping\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(philo->print);
		philo->now = get_time();
		while (get_time() - philo->now < (unsigned long)(philo->sleep))
		{
		}
		/* end of sleeping */
		/* start of thinking */
		pthread_mutex_lock(philo->print);
		printf("%lu %d is thinking\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(philo->print);
		/* end of thinking */
	}
	return (NULL);
}

void	start_threads(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->nb_philo)
	{
		pthread_create(&rules->philo[i].thread, NULL, routine, &rules->philo[i]);
		i += 2;
	}
	usleep((rules->eat * 1000) * 0.5);
	i = 1;
	while (i < rules->nb_philo)
	{
		pthread_create(&rules->philo[i].thread, NULL, routine, &rules->philo[i]);
		i += 2;
	}
}

int	main(int ac, char **av)
{
	t_rules	*rules;

	(void)ac;
	rules = parsing(av + 1);
	if (!rules)
		return (1);
	printf("nb_philo = %d, die = %d, eat = %d, sleep = %d, must_eat = %d\n", rules->nb_philo, rules->die, rules->eat, rules->sleep, rules->must_eat);
	start_threads(rules);
	while (42)
	{
	}
}
