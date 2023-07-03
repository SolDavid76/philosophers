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

t_philo	*fork_mutex_init(t_philo *philo)
{
	int				i;

	philo->all_forks = malloc(sizeof(pthread_mutex_t) * philo->nb_philo);
	if (!philo->all_forks)
		return (write(1, "NOT ENOUGH MEMORY\n", 18), free(philo), NULL);
	i = -1;
	while (++i < philo->nb_philo)
	{
		pthread_mutex_init(&philo->all_forks[i], NULL);
		philo[i].l_fork = i;
		// philo[i].l_fork = res[i];
	}
	i = -1;
	while (++i < philo->nb_philo)
		philo[i].r_fork = i + 1;
	philo[i].r_fork = 0;
	// philo[--i].r_fork = res[0];
	// while (--i)
	// 	philo[i].r_fork = res[i + 1];
	return (philo);
}

t_philo	*philo_init(t_rules *rules)
{
	t_philo			*philo;
	pthread_mutex_t	print;
	unsigned long	start;
	int				i;

	philo = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!philo)
		return (write(1, "NOT ENOUGH MEMORY\n", 18), free(rules), NULL);
	pthread_mutex_init(&print, NULL);
	i = -1;
	start = get_time();
	while (++i < rules->nb_philo)
	{
		philo[i].id = i + 1;
		philo[i].start = start;
		philo[i].nb_philo = rules->nb_philo;
		philo[i].die = rules->die;
		philo[i].eat = rules->eat;
		philo[i].sleep = rules->sleep;
		philo[i].must_eat = rules->must_eat;
		philo[i].print = print;
	}
	return (free(rules), fork_mutex_init(philo));
}

t_philo	*parsing(char **args)
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
	if (!args[4])
		res->must_eat = -1;
	else
	{
		res->must_eat = ft_atoi(args[4]);
		if (res->must_eat < 1)
			return (write(1, "Wrong input in must eat\n", 24), free(res), NULL);
	}
	if (res->nb_philo < 1 || res->die < 1 || res->eat < 1 || res->sleep < 1)
		return (write(1, "Wrong input\n", 12), free(res), NULL);
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

	philo->index = 0;
	while (42)
	{
		/* start of eating */
		dprintf(2, "#DEBUG : locking the l_fork (%u) of %d\n", philo->l_fork, philo->id);
		pthread_mutex_lock(&philo->all_forks[philo->l_fork]);
		printf("%lu %d has taken a fork\n", get_time() - philo->start, philo->id);
		dprintf(2, "#DEBUG : locking the r_fork (%u) of %d\n", philo->r_fork, philo->id);
		pthread_mutex_lock(&philo->all_forks[philo->r_fork]);
		printf("%lu %d has taken a fork\n", get_time() - philo->start, philo->id);
		pthread_mutex_lock(&philo->print);
		printf("%lu %d is eating\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print);
		philo->wait = get_time();
		while (get_time() - philo->wait < (unsigned long)(philo->eat))
		{
		}
		dprintf(2, "#DEBUG : unlocking the r_fork (%u) of %d\n", philo->r_fork, philo->id);
		pthread_mutex_unlock(&philo->all_forks[philo->r_fork]);
		dprintf(2, "#DEBUG : unlocking the l_fork (%u) of %d\n", philo->r_fork, philo->id);
		pthread_mutex_unlock(&philo->all_forks[philo->l_fork]);
		/* end of eating */
		/* start of sleeping */
		pthread_mutex_lock(&philo->print);
		printf("%lu %d is sleeping\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print);
		philo->wait = get_time();
		while (get_time() - philo->wait < (unsigned long)(philo->sleep))
		{
		}
		/* end of sleeping */
		/* start of thinking */
		pthread_mutex_lock(&philo->print);
		printf("%lu %d is thinking\n", get_time() - philo->start, philo->id);
		pthread_mutex_unlock(&philo->print);
		/* end of thinking */
	}
	return (NULL);
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	(void)ac;
	philo = parsing(av + 1);
	if (!philo)
		return (1);
	printf("nb_philo = %d, die = %d, eat = %d, sleep = %d, must_eat = %d\n", philo->nb_philo, philo->die, philo->eat, philo->sleep, philo->must_eat);
	pthread_create(&(philo[0].thread_id), NULL, routine, &philo[0]);
	pthread_create(&(philo[1].thread_id), NULL, routine, &philo[1]);
	while (42)
	{
	}
}
