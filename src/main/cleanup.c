/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:39:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/20 17:06:25 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_str_array(void **arr, int *n)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
	if (n)
		while (i < *n)
			free(arr[i++]);
	else
		while (arr[i])
			free(arr[i++]);
	
	free(arr);
}

void	error(char *msg, t_game *game, char **file)
{
	char	*tmp;
	int		i;
	
	if (msg)
	{
		tmp = ft_strjoin("Error\n", msg);
		ft_putstr_fd(tmp, 2);
		free(tmp);
	}
	i = 0;
	while (i < 4)
		free_str_array((void *)game->textures[i++].grid, &game->textures->height);
	free(game->textures);
	free(game->sprites);
	free_str_array((void *)file, NULL);
	exit (1);
}

void	error_sprite(void)
{
	
}
