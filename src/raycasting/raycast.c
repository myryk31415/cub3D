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

int	draw_line(int x, double wall_dist, t_game *game)
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		i;

	line_height = (int)(game->mlx->height / wall_dist);
	draw_start = -line_height / 2 + game->mlx->height / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + game->mlx->height / 2;
	if (draw_end >= game->mlx->height)
		draw_end = game->mlx->height - 1;
	i = 0;
	while (i < draw_start)
	{
		mlx_put_pixel(game->image, x, i, game->ceiling.value);
		i++;
	}
	while (i < draw_end)
	{
		mlx_put_pixel(game->image, x, i, game->textures->grid[i % game->textures->height][i % game->textures->width].value);
		i++;
	}
	while (i < game->mlx->height)
	{
		mlx_put_pixel(game->image, x, i, game->floor.value);
		i++;
	}
	return (0);
}

int	calculate_ray(int x, t_vec2d ray_dir, t_game *game)
{
	int		map_x;
	int		map_y;
	t_vec2d	delta_dist;
	t_vec2d	side_dist;
	t_vec2d	step;
	int		hit;
	int		side;
	double	wall_dist;
	double	len;

	hit = 0;
	map_x = (int)game->pos.x;
	map_y = (int)game->pos.y;
	len = vec2d_len(ray_dir);
	delta_dist.x = fabs(len / ray_dir.x);
	delta_dist.y = fabs(len / ray_dir.y);
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
	}
	double angle = fabs(vec2d_getrot(ray_dir) - vec2d_getrot(game->dir));
	if (side < 2)
		wall_dist = cos(angle) * (side_dist.y - delta_dist.y);
	else
		wall_dist = cos(angle) * (side_dist.x - delta_dist.x);
	return (draw_line(x, wall_dist, game));
}

int	raycast(t_game *game)
{
	t_vec2d	ray_dir;
	int		x;
	int		offset;

	x = 0;
	while (x < game->mlx->width)
	{
		offset = x * 2 / (double)game->mlx->width - 1;
		ray_dir = vec2d_mul(game->camera_plane, offset);
		ray_dir = vec2d_add(ray_dir, game->dir);
		if (calculate_ray(x, ray_dir, game))
			return (1);
		x++;
	}
	return (0);
}
