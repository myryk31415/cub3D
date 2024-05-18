/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:39:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 11:00:47 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	free_str_array(void **arr)
{
	int	i;

	i = 0;
	if (!arr)
		return ;
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
		free_str_array((void *)game->textures[i++].grid);
	free_str_array((void *)map->grid);
	free(map);
	free(game->textures);
	free_str_array((void *)file);
	exit (1);
}
