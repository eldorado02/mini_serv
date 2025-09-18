/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:44 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 15:03:52 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

unsigned short	ft_htons(unsigned short port)
{
	return ((port >> 8) | (port << 8));
}

unsigned int	ft_htonl(unsigned int addr)
{
	return ((addr & 0xff000000) >> 24 |
			(addr & 0x00ff0000) >> 8 | 
			(addr & 0x0000ff00) << 8 |
			(addr & 0x000000ff) << 24  );
}

void	init_fd(t_env *env)
{
	int	i;
	int	max_fd;

	i = 0;
	max_fd = env->listen_socket;
	FD_ZERO(&(env->readfds));
	FD_ZERO(&(env->writefds));
	FD_SET(env->listen_socket, &(env->readfds));
	while (i < 2048)
	{
		if (env->client[i].fd != 0)
		{
			FD_SET(env->client[i].fd, &(env->readfds));
			if (env->client[i].buf_to_flush != NULL && strlen(env->client[i].buf_to_flush) != 0)
				FD_SET(env->client[i].fd, &(env->writefds));	
			if (env->client[i].buffer_write != NULL && strlen(env->client[i].buffer_write) != 0)
				FD_SET(env->client[i].fd, &(env->writefds));	
			if (env->client[i].fd > max_fd)
				max_fd = env->client[i].fd;
		}
		i++;
	}
	env->nfds = max_fd + 1;
}

void	ft_free_all(t_env *env)
{
	int	i;

	i = 0;
	close(env->listen_socket);
	while (i < 2048)
	{
		if (env->client[i].fd != 0)
		{
			free(env->client[i].buffer_read);
			free(env->client[i].buffer_write);
			free(env->client[i].buf_to_flush);
			close(env->client[i].fd);
			env->client[i].fd = 0;
		}
		i++;
	}
}

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

void	insert_msg_to_flush_client(char * buf, int fd, t_env *env)
{
	int	i;

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			env->client[i].buf_to_flush = str_join(env->client[i].buf_to_flush,
				buf);
		}
		i++;
	}
}

void	accept_connection(t_env *env)
{
	int	fd;
	char	buffer[100];

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

	i = 0;
	while (i < 2048)
	{
		if (env->client[i].fd != 0 && env->client[i].fd != fd)
		{
			env->client[i].buffer_write = str_join(env->client[i].buffer_write,
				buffer);
			if (env->client[i].buffer_write == NULL)
				panic("Fatal Error\n");
		}
		i++;
	}
}

void	recv_msg(int fd, t_env *env)
{
	char	buffer[4097];
	int		n;

	n = recv(fd, buffer, 4097, 0);
	if (n == 0)
	{
		env->client[fd].fd = 0;
		free(env->client[fd].buffer_read);
		env->client[fd].buffer_read = NULL;
		free(env->client[fd].buffer_write);
		env->client[fd].buffer_write = NULL;
		free(env->client[fd].buf_to_flush);
		env->client[fd].buf_to_flush = NULL;
		sprintf(buffer, "server: client %d just left\n", env->client[fd].id);
		insert_msg_to_flush_client(buffer, fd, env);
		close(fd);
	}
	else if (n == -1)
		panic("Fatal Error\n");
	else
	{
		buffer[n] = '\0';
		insert_msg2(fd, buffer, env);
	}
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

void	create_line_before_send(int fd, t_env * env)
{
	char	buf[20];
	char	*str;

	create_to_flush_from_buf_write(fd, env);
	sprintf(buf, "client %d: ", env->client[fd].id);
	str = (char *)malloc(strlen(buf) + strlen(env->client[fd].buf_to_flush) + 1);
	if (str == NULL)
		panic("Fatal Error\n");
	strcpy(str, buf);
	strcat(str, env->client[fd].buf_to_flush);
	free(env->client[fd].buf_to_flush);
	env->client[fd].buf_to_flush = str;
}

void	send_msg(int fd, t_env *env)
{
	if (env->client[fd].buf_to_flush != NULL)
	{
		ft_send(fd, env->client[fd].buf_to_flush); // supposons hoe lasa daholo zany
		free(env->client[fd].buf_to_flush);
		env->client[fd].buf_to_flush = NULL;
		return ;
	}
	else
	{
		create_line_before_send(fd, env);
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