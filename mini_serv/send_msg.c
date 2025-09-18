/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   send_msg.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:48:05 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 18:26:44 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	ft_send_all(const char *str, int * pos, t_env *env, int fd, int n)
{
	int	i;
	int	s;
	int	to_send;

	i = 0;
	to_send = strlen(str);
	while (i < 10 && n < to_send)
	{
		s = send(fd, str + n, to_send - n, 0);
		if (s == -1)
			panic("Fatal Error\n", env);
		n += s;		
		i++;
	}
	if (n < to_send)
		*pos = n;
	else
		*pos = 0; // tsy niova zany
}

void	ft_clean_to_flush(int fd, t_env *env)
{
	int	pos;

	pos = 0;
	ft_send_all(env->client[fd].msg->msg, &pos, env, fd, (int)env->client[fd].pos);
	if (pos != 0)
		env->client[fd].pos = pos;
	else
	{
		env->client[fd].pos = 0;
		free(env->client[fd].buf_to_flush);		
		env->client[fd].buf_to_flush = NULL;
	}
}

void	create_message_to_flush(int fd, t_env *env)
{
	char	buf[20];
	char	*msg;
	int	s;
	char	*str;

	msg = NULL;
	sprintf(buf, "client %d: ", env->client[fd].msg->id_from);
	s = extract_message(&env->client[fd].buffer, &msg);
	if (s == 0)
		return ;
	if (s == -1)
		panic("Fatal Error\n", env);
	str = malloc(strlen(msg) + 1); // methode safe aloha de aveo vo test
	if (str == NULL)
		panic("Fatal Error\n", env);
	strcpy(str, msg);
	env->client[fd].buf_to_flush = malloc(strlen(buf) + strlen(str) + 1);
	if (env->client[fd].buf_to_flush == NULL)
		panic("Fatal Error\n", env);
	strcpy(env->client[fd].buf_to_flush, buf);
	strcat(env->client[fd].buf_to_flush, str);
	env->client[fd].pos = 0;
}

void	move_tab_to_read(int fd, int n_to_read, t_env *env)
{
	int i;
	int	j;

	i = 0;
	j = 1;
	while (i < n_to_read)
	{
		env->client[fd].tab_to_read[i] = env->client[fd].tab_to_read[j];
		i++;
		j++;
	}
}

void	send_msg(int fd, t_env *env)
{
	if (env->client[fd].buf_to_flush != NULL)
		ft_clean_to_flush(fd, env);
	if (env->client[fd].buf_to_flush == NULL && env->client[fd].msg != NULL)
	{
		create_message_to_flush(fd, env);
		if (env->client[fd].buffer[0] == '\0')
		{
			env->client[fd].n_to_read -= 1;
			if (env->client[fd].n_to_read > 0)
				move_tab_to_read(env->client[fd].fd, env->client[fd].n_to_read, env);
			env->client[fd].msg->user -= 1;
			if (env->client[fd].msg->user == 0)
			{
				free(env->client[fd].msg->msg);
				env->client[fd].msg->msg = NULL;
				env->client[fd].msg->user = 0;
				env->client[fd].msg->id_from = 0;
			}
		}
	}
}
