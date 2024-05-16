/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 23:07:12 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 00:29:58 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	calculate_ray(t_vec2d ray_dir, t_game *game)
{
	double	delta_dist_x;
	double	delta_dist_y;

	delta_dist_x = fabs(1 / ray_dir.x);
	delta_dist_y = fabs(1 / ray_dir.y);
}

int	raycast(t_game *game)
{
	t_vec2d	ray_dir;
	int		x;
	int		offset;

	while (x < game->mlx->width)
	{
		offset = x * 2 / (double)game->mlx->width - 1;
		ray_dir = vec2d_mul(game->camera_plane, offset);
		ray_dir = vec2d_add(ray_dir, game->direction);
		if (calculate_ray(ray_dir, game))
			return (1);
		x++;
	}
	return (0);
}
