/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: isingara <isingara@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/29 22:31:43 by isingara          #+#    #+#             */
/*   Updated: 2025/11/29 22:31:43 by isingara         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static size_t n_len(long nb)
{
	size_t len;

	len = 1;
	if (nb < 0)
	{
		nb *= -1;
		len++;
	}
	while (nb / 10)
	{
		len++;
		nb /= 10;
	}
	return (len);
}

char *ft_itoa(int n)
{
	char *str;
	long nb;
	size_t len;

	nb = n;
	len = n_len(nb);
	str = malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	if (nb < 0)
		nb *= -1;
	while (len--)
	{
		str[len] = (nb % 10) + '0';
		nb /= 10;
		if (nb == 0)
			break;
	}
	if (n < 0)
		str[0] = '-';
	return (str);
}
