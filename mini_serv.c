/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:44 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 10:40:33 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

char *str_join(char *buf, char *add)
{
	char	*newbuf;
	int		len;

	if (buf == 0)
		len = 0;
	else
		len = strlen(buf);
	newbuf = malloc(sizeof(*newbuf) * (len + strlen(add) + 1));
	if (newbuf == 0)
		return (0);
	newbuf[0] = 0;
	if (buf != 0)
		strcat(newbuf, buf);
	free(buf);
	strcat(newbuf, add);
	return (newbuf);
}

int	ft_putstr_fd(const char *s, int fd)
{
	return (write(fd, s, strlen(s)));
}

void	panic(const char *s)
{
	ft_putstr_fd(s, 2);
	exit(EXIT_FAILURE);
}

short	ft_htons(short port)
{
	return ((port >> 8) | (port << 8));
}

int	ft_htonl(int addr)
{
	return ((addr & 0xff000000) >> 24 |
			(addr & 0x00ff0000) >> 8 | 
			(addr & 0x0000ff00) << 8 |
			(addr & 0x000000ff) << 24  );
}

int	init_connection(int port)
{
	int	sockfd;
	struct sockaddr_in	addr;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd == -1)
		panic("Fatal Error socket\n");
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ft_htonl(INADDR_LOOPBACK);
	addr.sin_port = ft_htons(port);
	if (bind(sockfd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
	{
		close(sockfd);
		panic("Fatal Error bind\n");
	}
	if (listen(sockfd, 256) == -1)
	{
		close(sockfd);
		panic("Fatal Error listen\n");	
	}
	return (sockfd);
}

void	init_fd(t_env *env)
{
	int	i;

	i = 0;
	env->nfds = 0;
	FD_ZERO(&(env->readfds));
	FD_ZERO(&(env->writefds));
	FD_SET(env->listen_socket, &(env->readfds));
	while (i < 2048)
	{
		if (env->client[i].fd != 0)
		{
			FD_SET(env->client[i].fd, &(env->readfds));
			env->nfds += 1;
			if (env->client[i].buffer_write != NULL && strlen(env->client[i].buffer_write) != 0)
			{
				FD_SET(env->client[i].fd, &(env->writefds));	
				env->nfds += 1;
			}
		}
		i++;
	}	
}

void	insert_msg_to_write_client(char * buf, int fd, t_env *env)
{
	int	i;

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			env->client[fd].buffer_write = str_join(env->client[fd].buffer_write,
				buf);
		}
		i++;
	}
}

void	insert_msg_to_flush_client(char * buf, int fd, t_env *env)
{
	int	i;

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			env->client[fd].buf_to_flush = str_join(env->client[fd].buf_to_flush,
				buf);
		}
		i++;
	}
}

void	accept_connection(t_env *env)
{
	int	fd;
	char	buffer[100];

	printf("Ye tonga tato @ accept\n");
	fd = accept(env->listen_socket, NULL, NULL);
	if (fd == -1)
		panic ("Fatal Error");
	env->client[fd].id = env->current_id++;
	env->client[fd].fd = fd;
	env->client[fd].buffer_read = NULL;
	env->client[fd].buffer_write = NULL;
	sprintf(buffer, "server: client %d just arrived\n", env->client[fd].id);
	insert_msg_to_flush_client(buffer, fd, env); // atao ato lehizy satria ato n 
}

void	ft_send(int fd, char *msg)
{
	int	n;
	// int	i = 0;
	int	pos = 0;

	while ((size_t)pos < strlen(msg))
	{
		n = send(fd, msg + pos, strlen(msg) - pos, 0);
		if (n == -1)
			panic("Fatal Error\n");
		pos += n;
	}
}

void	insert_msg2(int fd, char *buffer, t_env * env)
{
	int	i;
	char	buf[20];

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			sprintf(buf, "client %d: ", env->client[i].id);
			env->client[fd].buffer_write = str_join(env->client[fd].buffer_write,
				buffer);
			if (env->client[fd].buffer_write == NULL)
				panic("Fatal Error\n");
			env->client[fd].buffer_write = str_join(env->client[fd].buffer_write,
				buffer);
			if (env->client[fd].buffer_write == NULL)
				panic("Fatal Error\n");
		}
	}
}

void	recv_msg(int fd, t_env *env)
{
	char	buffer[4097];
	int		n;

	printf("Ye tonga tato @ recv message koa\n");	
	n = recv(fd, buffer, 4096, 0);
	if (n == 0)
	{
		env->client[fd].fd = 0;
		free(env->client[fd].buffer_read);
		env->client[fd].buffer_read = NULL;
		free(env->client[fd].buffer_write);
		env->client[fd].buffer_write = NULL;
		sprintf(buffer, "server: client %d just left\n", env->client[fd].id);
		insert_msg_to_flush_client(buffer, fd, env);
	}
	else if (n == -1)
		panic("Fatal Error\n");
	else
		insert_msg2(fd, buffer, env);
}

void	do_select(t_env *env)
{
	select(env->nfds, &env->readfds, &env->writefds, NULL, NULL);
}

int extract_message(char **buf, char **msg)
{
	char	*newbuf;
	int	i;

	*msg = 0;
	if (*buf == 0)
		return (0);
	i = 0;
	while ((*buf)[i])
	{
		if ((*buf)[i] == '\n')
		{
			newbuf = calloc(1, sizeof(*newbuf) * (strlen(*buf + i + 1) + 1));
			if (newbuf == 0)
				return (-1);
			strcpy(newbuf, *buf + i + 1);
			*msg = *buf;
			(*msg)[i + 1] = 0;
			*buf = newbuf;
			return (1);
		}
		i++;
	}
	return (0);
}

void	create_to_flush_from_buf_write(int fd, t_env *env)
{
	// eto lo zany sur fa vide le to_flush mato makato
	char	*str = NULL;
	int		n;

	n = extract_message(&env->client[fd].buffer_write, &str);
	if (n == -1)
		panic("Fatal Error\n");
	if (str != NULL)
	{
		env->client[fd].buf_to_flush = str_join(env->client[fd].buf_to_flush, str);
		if (env->client[fd].buf_to_flush == NULL)
			panic("Fatal Error\n");
		free(str);
	}
}

void	send_msg(int fd, t_env *env)
{
	if (env->client[fd].buf_to_flush != NULL)
	{
		ft_send(fd, env->client[fd].buf_to_flush); // supposons hoe lasa daholo zany
		free(env->client[fd].buf_to_flush);
		env->client[fd].buf_to_flush = NULL;
		create_to_flush_from_buf_write(fd, env);
		return ;
	}
	else
	{
		create_to_flush_from_buf_write(fd, env);
		ft_send(fd, env->client[fd].buf_to_flush);
		free(env->client[fd].buf_to_flush);
		env->client[fd].buf_to_flush = NULL;
	}
}

void	check_fd(t_env *env)
{
	int	i = 0;
	
	if (FD_ISSET(env->listen_socket, &(env->readfds)))
		accept_connection(env);
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

void	main_loop(t_env *env)
{
	while (1)
	{
		init_fd(env);
		do_select(env);			
		check_fd(env);
	}
}

int	main(int ac, char **av)
{
	t_env	env;

	memset(&env, 0, sizeof(t_env));
	if (ac == 1)
	{
		ft_putstr_fd("Fatal Error begin\n", 2);
		return (1);				
	}
	env.listen_socket = init_connection(atoi(av[1]));
	main_loop(&env);	
}