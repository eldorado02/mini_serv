/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_fd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:31:38 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:07:14 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	check_fd(t_env *env)
{
	int	i;

	i = 0;
	if (FD_ISSET(env->listen_sock, &(env->readfds)))
	{
		accept_connection(env);
	}
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && FD_ISSET(env->client[i].fd, &(env->readfds)))
		{
			recv_msg(env->client[i].fd, env);
		}
		if (env->client[i].fd != 0 && FD_ISSET(env->client[i].fd, &(env->writefds)))
		{
			send_msg(env->client[i].fd, env);
		}
		i++;
	}		
}