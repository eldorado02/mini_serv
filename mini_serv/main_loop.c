/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main_loop.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:21:05 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:01:44 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	main_loop(t_env *env)
{
	while (1)
	{
		init_fd(env);
		do_select(env);
		check_fd(env);
	}
}