/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mini_serv.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:02:29 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:01:39 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

int	main(int ac, char **av)
{
	t_env	env;

	memset(&env, 0, sizeof(t_env));
	if (ac == 1)
	{
		panic("Wrong number of arguments\n", &env);
	}
	env.listen_sock = init_connection(atoi(av[1]), &env);
	main_loop(&env);
	return (0);
}