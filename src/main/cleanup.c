/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cleanup.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/18 10:39:56 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 19:11:32 by antonweizma      ###   ########.fr       */
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

void	free_arrays(t_game *game, t_map *map)
{
	int		i;

	i = 0;
	while (i < 4)
		free_str_array((void *)game->textures[i++].grid, &game->textures->height);
	if (map)
	{
		free_str_array((void *)map->grid, &map->height);
		free(map);
	}
	free(game->textures);
}

void	error(char *msg, t_game *game, char **file, t_map *map)
{
	char	*tmp;
	
	if (msg)
	{
		tmp = ft_strjoin("Error\n", msg);
		ft_putstr_fd(tmp, 2);
		free(tmp);
	}
	free_arrays(game, map);
	free_str_array((void *)file, NULL);
	exit (1);
}
