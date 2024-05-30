/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 11:26:54 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/30 11:27:21 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	empty_line(t_game *game, int x)
{
	int	i;

	i = 0;
	while (i < game->mlx->height / 2)
	{
		mlx_put_pixel(game->image, x, i, game->ceiling.value);
		i++;
	}
	while (i < game->mlx->height)
	{
		mlx_put_pixel(game->image, x, i, game->floor.value);
		i++;
	}
}

double	get_wall_x(int side, t_vec2d ray_dir, t_vec2d difference, t_game *game)
{
	double	angle;
	double	wall_x;

	angle = fabs(vec2d_getrot(ray_dir));
	if (side < 2)
		wall_x = game->pos.y + sin(angle) * \
		difference.x * (1 - 2 * (ray_dir.y < 0));
	else
		wall_x = game->pos.x + cos(angle) * difference.y;
	wall_x -= floor(wall_x);
	return (wall_x);
}

void	set_depth(int x, t_vec2d ray_dir, double difference, t_game *game)
{
	double	angle;

	angle = fabs(vec2d_getrot(ray_dir) - vec2d_getrot(game->dir));
	game->depth[x] = cos(angle) * difference;
	return ;
}

int	increase(int *map, double *side_dist, double delta_dist, double ray_dir)
{
	*side_dist += delta_dist;
	if (ray_dir > 0)
	{
		(*map)++;
		return (0);
	}
	(*map)--;
	return (1);
}
