/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:23:45 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/07 11:49:30 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	mutex_init(t_rules *rules)
{
	int	i;

	rules->all_forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->all_forks)
		return (-1);
	pthread_mutex_init(&rules->print, NULL);
	pthread_mutex_init(&rules->check_end, NULL);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_mutex_init(&rules->all_forks[i], NULL);
	return (0);
}

t_rules	*philo_init(t_rules *rules)
{
	int		i;

	rules->philo = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philo || mutex_init(rules))
		return (write(1, "NOT ENOUGH MEMORY\n", 18), NULL);
	i = -1;
	while (++i < rules->nb_philo)
	{
	// 	rules->philo[i] = malloc(sizeof(t_philo));
		rules->philo[i].id = i + 1;
		rules->philo[i].eat = rules->eat;
		rules->philo[i].sleep = rules->sleep;
		rules->philo[i].must_eat = rules->must_eat;
		rules->philo[i].end = 0;
		pthread_mutex_init(&rules->philo[i].check_end, NULL);
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

int	check_end(t_philo *philo)
{
	pthread_mutex_lock(&philo->check_end);
	if (philo->end)
		return (pthread_mutex_unlock(&philo->check_end), 1);
	return (pthread_mutex_unlock(&philo->check_end), 0);
}

int	print_action(t_philo *philo, char *msg)
{
	if (check_end(philo))
		return (1);
	pthread_mutex_lock(philo->print);
	printf("%lu %d %s\n", get_time() - philo->start, philo->id, msg);
	pthread_mutex_unlock(philo->print);
	return (0);
}

void	unlock_fork(pthread_mutex_t *l_fork, pthread_mutex_t *r_fork)
{
	if (l_fork)
		pthread_mutex_unlock(l_fork);
	if (r_fork)
		pthread_mutex_unlock(r_fork);
}

int	eating(t_philo *philo)
{
	pthread_mutex_lock(philo->l_fork);
	if (print_action(philo, "has taken a fork"))
		return (unlock_fork(philo->l_fork, NULL), 1);
	pthread_mutex_lock(philo->r_fork);
	if (print_action(philo, "has taken a fork"))
		return (unlock_fork(philo->l_fork, philo->r_fork), 1);
	if (print_action(philo, "is eating"))
		return (unlock_fork(philo->l_fork, philo->r_fork), 1);
	philo->last_eat = get_time();
	usleep(philo->eat * 1000);
	pthread_mutex_unlock(philo->r_fork);
	pthread_mutex_unlock(philo->l_fork);
	return (0);
}

// philo->now = get_time();
// while (get_time() - philo->now <= (unsigned long)(philo->eat))
// {
// }
void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep((philo->eat * 1000) * 0.5);
	while (42)
	{
		if (eating(philo))
			return (NULL);
		if (print_action(philo, "is sleeping"))
			return (NULL);
		usleep(philo->sleep * 1000);
		if (print_action(philo, "is thinking"))
			return (NULL);
		if (0 < philo->must_eat)
			philo->must_eat--;
	}
	return (NULL);
}

void	start_threads(t_rules *rules)
{
	int	i;

	i = -1;
	rules->start = get_time();
	while (++i < rules->nb_philo)
	{
		rules->philo[i].start = rules->start;
		rules->philo[i].last_eat = rules->start;
		pthread_create(&rules->philo[i].th, NULL, routine, &rules->philo[i]);
	}
}

int	monitoring (t_philo *philo, int nb_philo, unsigned long die)
{
	int	i;

	i = -1;
	while (++i < nb_philo)
	{
		if ((philo[i].last_eat + die) < get_time())
		{
			print_action(&philo[i], "died");
			i = -1;
			while (++i < nb_philo)
			{
				pthread_mutex_lock(&philo[i].check_end);
				philo[i].end = 1;
				pthread_mutex_unlock(&philo[i].check_end);
				return (1);
			}
		}
	}
	return (0);
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
		if (monitoring(rules->philo, rules->nb_philo, rules->die))
			break ;
	}
	printf("FNINSH\n");
}
