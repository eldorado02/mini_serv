/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_fd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:22:27 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 16:59:43 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	init_fd(t_env *env)
{
	int	i;
	int	max_fd;

	i = 0;
	FD_ZERO(&(env->readfds));
	FD_ZERO(&(env->writefds));
	FD_SET(env->listen_sock, &(env->readfds));
	max_fd = env->listen_sock;
	while (i < 2048)
	{
		if (env->client[i].fd != 0)
		{
			FD_SET(env->client[i].fd, &(env->readfds));
			if (env->client[i].buf_to_flush != NULL || env->client[i].msg != NULL)
				FD_SET(env->client[i].fd, &(env->writefds));
			if (env->client[i].fd > max_fd)
				max_fd = env->client[i].fd;
		}
		i++;
	}
	env->nfds = max_fd + 1;
}