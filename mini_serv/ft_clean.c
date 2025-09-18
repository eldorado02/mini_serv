/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_clean.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:34:47 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:24:24 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	ft_clean_client(int fd, t_env *env) // tisy close ato
{
	if (env->client[fd].fd != 0)
	{
		env->client[fd].fd = 0;
		env->client[fd].id = 0;
		env->client[fd].pos = 0;
		if (env->client[fd].msg != NULL)
		{	
			env->client[fd].msg->user = (env->client[fd].msg->user <= 0) ? 0 : env->client[fd].msg->user - 1;
			env->client[fd].msg = NULL;
		}
		free(env->client[fd].buf_to_flush);
		env->client[fd].buf_to_flush = NULL;
	}
}