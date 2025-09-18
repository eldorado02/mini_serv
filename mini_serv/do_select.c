/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   do_select.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:29:38 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 17:07:21 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	do_select(t_env *env)
{
	if (select(env->nfds, &env->readfds, &env->writefds, NULL, NULL) == -1)
		panic("Fatal Error\n", env);
}