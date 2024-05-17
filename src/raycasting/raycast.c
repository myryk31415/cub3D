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

int	draw_line(int x, int side, double wall_dist, double wall_x, t_vec2d ray_dir, t_game *game)
{
	int		line_height;
	int		draw_start;
	int		draw_end;
	int		i;
	double	step;
	t_vec2d texPos;

	if (wall_dist > 0)
		line_height = (int)(game->mlx->height / wall_dist);
	else
		line_height = game->mlx->height;
	draw_start = -line_height / 2 + game->mlx->height / 2;
	if (draw_start < 0)
		draw_start = 0;
	draw_end = line_height / 2 + game->mlx->height / 2;
	if (draw_end >= game->mlx->height)
		draw_end = game->mlx->height - 1;
	step = 1.0 * game->textures[side].height / line_height;
	texPos.x = wall_x * (double)game->textures[side].width;
	// why??
	if (side <= 1 && ray_dir.y < 0)
		texPos.x = game->textures[side].width - texPos.x - 1;
	// if (side > 1 && ray_dir.y < 0)
	// 	texPos.x = game->textures[0].width - texPos.x - 1;
	texPos.y = (draw_start - game->mlx->height / 2 + line_height / 2) * step;
	i = 0;
	while (i < draw_start)
	{
		mlx_put_pixel(game->image, x, i, game->ceiling.value);
		i++;
	}
	while (i < draw_end)
	{
		t_pixel	color = game->textures[side].grid[(int)texPos.y][(int)texPos.x];
		texPos.y += step;
		if (side < 2)
		{
			color.bytes.r /= 3;
			color.bytes.g /= 3;
		}
		mlx_put_pixel(game->image, x, i, color.value);
		// if (side < 2)
		// 	mlx_put_pixel(game->image, x, i, 0xFF0000FF);
		// else
		// 	mlx_put_pixel(game->image, x, i, 0x0000FFFF);
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
	double	wall_x;

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
	double angle2 = fabs(vec2d_getrot(ray_dir));
	if (side < 2)
	{
		wall_dist = cos(angle) * (side_dist.x - delta_dist.x);
		wall_x = game->pos.y * (1 - 2 * (ray_dir.y < 0)) + sin(angle2) * (side_dist.x - delta_dist.x);
	}
	else
	{
		wall_dist = cos(angle) * (side_dist.y - delta_dist.y);
		wall_x = game->pos.x + cos(angle2) * (side_dist.y - delta_dist.y);
	}
	wall_x -= floor(wall_x);
	// if (x == 200 || x == 600)
	// 	printf("x: %d, wall_x: %f, angle2: %f, side_dist: %f\n", x, wall_x, angle2, side_dist.x -delta_dist.x);
	return (draw_line(x, side, wall_dist, wall_x, ray_dir, game));
}

int	raycast(t_game *game)
{
	t_vec2d	ray_dir;
	int		x;
	double	offset;

	game->fov_factor = 1;
	game->camera_plane = vec2d_rot(game->dir, M_PI / 2);
	game->camera_plane = vec2d_mul(game->camera_plane, 1.0 / game->fov_factor);
	x = 0;
	while (x < game->mlx->width)
	{
		offset = (double)x * 2 / game->mlx->width - 1;
		ray_dir = vec2d_mul(game->camera_plane, offset);
		ray_dir = vec2d_add(ray_dir, game->dir);
		if (calculate_ray(x, ray_dir, game))
			return (1);
		x++;
	}
	return (0);
}
