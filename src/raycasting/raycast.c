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
	if (side <= 1 && ray_dir.y < 0)
		texPos.x = game->textures[side].width - texPos.x - 1;
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
		mlx_put_pixel(game->image, x, i, color.value);
		i++;
	}
	while (i < game->mlx->height)
	{
		mlx_put_pixel(game->image, x, i, game->floor.value);
		i++;
	}
	return (0);
}

/*
 *@brief Draws a vertical black line.
 *@param game The game structure.
 *@param x The screen x coordinate.
*/
void	black_line(t_game *game, int x)
{
	int	i;

	i = 0;
	while (i < game->mlx->height)
	{
		mlx_put_pixel(game->image, x, i, 0x000000FF);
		i++;
	}
}

int	calc_wall_dist(int x, int side, t_vec2d ray_dir, t_vec2d side_dist, t_vec2d delta_dist, t_game *game)
{
	double	wall_dist;
	double	wall_x;
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
	return (draw_line(x, side, wall_dist, wall_x, ray_dir, game));
}

int	cast_loop(int map_x, int map_y, int x, t_vec2d step, t_vec2d ray_dir, t_vec2d delta_dist, t_vec2d *side_dist, t_game *game)
{
	int		side;
	int		hit;

	hit = 0;
	while (!hit)
	{
		if (side_dist->x < side_dist->y)
		{
			side_dist->x += delta_dist.x;
			map_x += step.x;
			side = 0;
			if (ray_dir.x < 0)
				side = 1;
		}
		else
		{
			side_dist->y += delta_dist.y;
			map_y += step.y;
			side = 2;
			if (ray_dir.y < 0)
				side = 3;
		}
		if (map_x < 0 && ray_dir.x <= 0)
			return (black_line(game, x), -1);
		if (map_x >= game->map.width && ray_dir.x >= 0)
			return (black_line(game, x), -1);
		if (map_y < 0 && ray_dir.y <= 0)
			return (black_line(game, x), -1);
		if (map_y >= game->map.height && ray_dir.y >= 0)
			return (black_line(game, x), -1);
		if (map_y >= 0 && map_y < game->map.height && map_x >= 0 && map_x < game->map.width && game->map.grid[map_y][map_x].value)
			hit = 1;
	}
	return (side);
}

int	calculate_ray(int x, t_vec2d ray_dir, t_game *game)
{
	int		map_x;
	int		map_y;
	t_vec2d	delta_dist;
	t_vec2d	side_dist;
	t_vec2d	step;
	int		side;

	map_x = (int)game->pos.x;
	map_y = (int)game->pos.y;
	delta_dist.x = fabs(vec2d_len(ray_dir) / ray_dir.x);
	delta_dist.y = fabs(vec2d_len(ray_dir) / ray_dir.y);
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
	side = cast_loop(map_x, map_y, x, step, ray_dir, delta_dist, &side_dist, game);
	if (side == -1)
		return (0);
	return (calc_wall_dist(x, side, ray_dir, side_dist, delta_dist, game));
}

/*
 *@brief Raycaster, calculates walls, ceiling, floor.
 *@param game The game structure.
*/
int	raycast(t_game *game)
{
	t_vec2d	ray_dir;
	int		x;
	double	offset;

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
