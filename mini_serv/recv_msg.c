/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   recv_msg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:48:19 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 18:22:50 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	read_msg(int fd, t_env *env)
{
	char	buffer[64101];
	int		n;

	do
	{
		n = recv(fd, buffer, 60100, 0);
		if (n == -1)
			panic("Fatal Error\n", env);
		if (n == 0)
			disconnect_client(fd, env);
		else
		{
			buffer[n] = '\0';
			env->msg[fd].msg = str_join(env->msg[fd].msg, buffer);
			if (env->msg[fd].msg == NULL)
				panic("Fatal Error\n", env);
		}
			
	} while(n >= 64100);
	env->msg[fd].user = 0;
	env->msg[fd].id_from = env->client[fd].id;
}

void	add_msg_to_all(int fd, t_env *env) // fd le nahazaona le msg
{
	int	i;

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			if (env->client[i].msg == NULL)
			{
				env->client[i].msg = &env->msg[fd];
				env->client[i].buffer = (char *)malloc(strlen(env->client[i].msg->msg) + 1);
				if (env->client[i].buffer == NULL)
					panic("Fatal Error\n", env);
				strcpy(env->client[i].buffer, env->client[i].msg->msg);
				env->client[i].tab_to_read[0] = fd;
				env->msg[fd].user += 1;
				env->client[i].n_to_read = 1;
			}
			else
			{
				env->client[i].tab_to_read[env->client[i].n_to_read++] = fd;
				env->msg[fd].user += 1;
			}
		}
		i++;
	}
}

void	recv_msg(int fd, t_env *env)
{
	if (env->msg[fd].msg != NULL)
	{
		read_msg(fd, env);
		add_msg_to_all(fd, env);		
	}
}