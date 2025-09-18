/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_hton.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: arafaram <arafaram@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/09/18 16:07:04 by arafaram          #+#    #+#             */
/*   Updated: 2025/09/18 16:18:11 by arafaram         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "mini_serv.h"

unsigned int	ft_htonl(unsigned int addr)
{
	return ((addr & 0xff000000) >> 24 | 
			(addr & 0x00ff0000) >> 8 |
			(addr & 0x0000ff00) << 8 |
			(addr & 0x000000ff) << 24 );
}

unsigned short	ft_htons(unsigned short port)
{
	return ((port >> 8) | (port << 8));
}