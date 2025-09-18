/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_connection.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:07:20 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 16:18:45 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

int	init_connection(int port, t_env *env)
{
	int	fd;
	struct sockaddr_in	addr;

	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd == -1)
		panic("Fatal Error\n", env);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = ft_htonl(INADDR_LOOPBACK);
	addr.sin_port = ft_htons(port);
	if (bind(fd, (struct sockaddr *)&addr, sizeof(struct sockaddr_in)) == -1)
		panic("Fatal Error\n", env);
	if (listen(fd, 200) == -1)
		panic("Fatal Error\n", env);
	return (fd);
}