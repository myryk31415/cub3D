/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/16 23:07:12 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 02:37:17by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_next_intersection()
{

}

int	calculate_ray(t_vec2d ray_dir, t_game *game)
{
	int		map_x;
	int		map_y;
	t_vec2d	delta_dist;
	t_vec2d	side_dist;
	t_vec2d	step;
	int		hit;
	int		side;
	double	wall_dist;
	int		count;

	map_x = (int)game->pos.x;
	map_y = (int)game->pos.y;
	delta_dist.x = fabs(1 / ray_dir.x);
	delta_dist.y = fabs(1 / ray_dir.y);
	if (ray_dir.x < 0)
	{
		step.x = -1;
		side_dist.x = (game->pos.x - map_x) * delta_dist.x;
	}
	else
	{
		step.x = 1;
		side_dist.x = (map_x + 1.0 - game->pos.x) * delta_dist.x;
	}
	if (ray_dir.y < 0)
	{
		step.y = -1;
		side_dist.y = (game->pos.y - map_y) * delta_dist.y;
	}
	else
	{
		step.y = 1;
		side_dist.y = (map_y + 1.0 - game->pos.y) * delta_dist.y;
	}
	count = 0;
	while (!hit)
	{
		if (side_dist.x < side_dist.y)
		{
			side_dist.x += delta_dist.x;
			map_x += step.x;
			side = 0;
			if (ray_dir.x < 0)
				side = 1;
		}
		else
		{
			side_dist.y += delta_dist.y;
			map_y += step.y;
			side = 2;
			if (ray_dir.y < 0)
				side = 3;
		}
		if (game->map.grid[map_y][map_x].value)
			hit = 1;
		count++;
	}
	double angle = fabs(vec2d_getrot(ray_dir) - vec2d_getrot(game->dir));
	if (side < 2)
		wall_dist = cos(angle) * (side_dist.y - delta_dist.y);
	else
		wall_dist = cos(angle) * (side_dist.x - delta_dist.x);
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
		ray_dir = vec2d_add(ray_dir, game->dir);
		if (calculate_ray(ray_dir, game))
			return (1);
		x++;
	}
	return (0);
}
