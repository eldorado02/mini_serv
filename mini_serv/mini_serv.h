/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 15:54:45 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 18:14:38 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINI_SERV_H
# define MINI_SERV_H

# include <stdio.h>
# include <unistd.h>
# include <errno.h>
# include <netinet/in.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <sys/select.h>
# include <errno.h>
# include <string.h>
# include <netdb.h>
# include <stdlib.h>

typedef struct s_msg
{
	int		id_from;
	int		user;
	char	*msg;
}				t_msg;

typedef struct s_client
{
	int		fd;
	int		id;
	size_t	pos;
	t_msg	*msg; // tsy allouer
	char	*buf_to_flush; //allouer
	char	*buffer; // allouer
	int		n_to_read;
	int		tab_to_read[100];
}				t_client;

typedef struct s_env
{
	int			listen_sock;
	t_client	client[2048];
	t_msg		msg[2048];
	fd_set		readfds;
	fd_set		writefds;
	int			cur_id;
	int			nfds;
}				t_env;

unsigned int	ft_htonl(unsigned int addr);
unsigned short	ft_htons(unsigned short port);
int				init_connection(int port, t_env *env);
void			panic(const char *s, t_env *env);
int				ft_putstr_fd(const char *s, int fd);
int 			extract_message(char **buf, char **msg);
char 			*str_join(char *buf, char *add);
void			main_loop(t_env *env);

void			do_select(t_env *env);
void			init_fd(t_env *env);
void			check_fd(t_env *env);
void			ft_clean_client(int fd, t_env *env);
void			recv_msg(int fd, t_env *env);
void			send_msg(int fd, t_env *env);
void			accept_connection(t_env *env);
void			disconnect_client(int fd, t_env *env);

#endif