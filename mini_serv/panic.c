/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   panic.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:02:40 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 16:19:11 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

void	ft_free_all(t_env *env)
{
	(void)env;
}

void	panic(const char *s, t_env *env)
{
	ft_putstr_fd(s, 2);
	ft_free_all(env);
	exit(EXIT_FAILURE);
}