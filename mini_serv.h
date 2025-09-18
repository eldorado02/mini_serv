/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 08:07:59 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 13:49:40 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SERV_H
# define MINI_SERV_H

# include <unistd.h>
# include <stdio.h>
# include <sys/socket.h>
# include <netinet/in.h>
# include <string.h>
# include <sys/select.h>
# include <stdlib.h>
# include <time.h>
# include <sys/types.h>

typedef struct s_client
{
	int			fd;
	int			id;
	char		*buffer_read;
	char		*buffer_write; // ato le ho soratana
	char		*buf_to_flush; // ato le tokony alefa de ref vide ito zay vo maka ao @ le buf write ndray
}				t_client;

typedef struct s_env
{
	int			listen_socket;
	int			current_id;
	t_client	client[2048];
	fd_set		readfds;
	fd_set		writefds;
	int			nfds;
}				t_env;

unsigned short	ft_htons(unsigned short port);
unsigned int	ft_htonl(unsigned int addr);
void			init_fd(t_env *env);

#endif