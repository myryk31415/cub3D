/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:39:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 14:04:22 by padam            ###   ########.fr       */
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

void	error(char *msg, t_game *game, char **file, t_map *map)
{
	int	i;

	i = 0;
	if (msg)
		ft_putstr_fd(msg, 2);
	while (i < 4)
		free_str_array((void *)game->textures[i++].grid, &game->textures->height);
	free_str_array((void *)map->grid, &map->height);
	free(map);
	free(game->textures);
	free_str_array((void *)file, NULL);
	exit (1);
}
