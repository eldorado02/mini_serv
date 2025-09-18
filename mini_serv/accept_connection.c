/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   accept_connection.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:33:21 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:13:12 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	broadcast_to_to_flush(char * str, int fd, t_env *env)
{
	int	i;

	i = 0;
	while (i < 0)
	{
		if (env->client[i].fd != fd)
		{
			env->client[i].buf_to_flush = str_join(env->client[i].buf_to_flush, str);
			if (env->client[i].buf_to_flush == NULL)
				panic("Fatal Error\n", env);
		}
		i++;
	}
}

void	accept_connection(t_env *env)
{
	int	new_sd;

	new_sd = accept(env->listen_sock, NULL, NULL);
	if (new_sd == -1)
		panic("Fatal Error\n", env);
	ft_clean_client(new_sd, env); // normalement tokny tsy hiatra ito fonction ito | no close
	env->client[new_sd].fd = new_sd;
	env->client[new_sd].id = env->cur_id++;
	env->client[new_sd].pos = 0;
	env->client[new_sd].buf_to_flush = NULL;
	env->client[new_sd].msg = NULL; // ty mbola tsy ampy anle andefasana message daholo le client
}