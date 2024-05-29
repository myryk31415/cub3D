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

int	draw_line(int x, int side, double wall_x, t_game *game)
{
	int		draw_bound;
	int		line_height;
	double	step;
	t_vec2d texPos;

	line_height = (int)(game->mlx->width * game->wall_height / game->depth[x]);
	draw_bound = -line_height / 2 + game->mlx->height / 2;
	draw_bound *= (draw_bound > 0);
	step = 1.0 * game->textures[side].height / line_height;
	texPos.x = wall_x * (double)game->textures[side].width;
	texPos.y = 0;
	if (line_height > game->mlx->height)
		texPos.y = (-game->mlx->height / 2 + line_height / 2) * step;
	line_height = 0;
	while (line_height < draw_bound)
		mlx_put_pixel(game->image, x, line_height++, game->ceiling.value);
	draw_bound = game->mlx->height - draw_bound - 1;
	while (line_height < draw_bound)
	{
		mlx_put_pixel(game->image, x, line_height++,
			game->textures[side].grid[(int)texPos.y][(int)texPos.x].value);
		texPos.y += step;
	}
	while (line_height < game->mlx->height	)
		mlx_put_pixel(game->image, x, line_height++, game->floor.value);
	return (0);
}

/*
 *@brief Draws a vertical line, top half is ceiling color, bottom half floor color.
 *@param game The game structure.
 *@param x The screen x coordinate.
*/
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
	double angle = fabs(vec2d_getrot(ray_dir));
	double wall_x;

	if (side < 2)
		wall_x = game->pos.y + sin(angle) * difference.x * (1 - 2 * (ray_dir.y < 0));
	else
		wall_x = game->pos.x + cos(angle) * difference.y;
	wall_x -= floor(wall_x);
	return (wall_x);
}

void	set_depth(int x, t_vec2d ray_dir, double difference, t_game *game)
{
	double	angle = fabs(vec2d_getrot(ray_dir) - vec2d_getrot(game->dir));

	game->depth[x] = cos(angle) * difference;
	return;
}

// int	calc_wall_dist(int x, int side, t_vec2d ray_dir, t_vec2d difference, t_game *game)
// {
// 	double	wall_x;
// 	double	angle = fabs(vec2d_getrot(ray_dir) - vec2d_getrot(game->dir));
// 	double	angle2 = fabs(vec2d_getrot(ray_dir));

// 	if (side < 2)
// 	{
// 		game->depth[x] = cos(angle) * difference.x;
// 		wall_x = game->pos.y + sin(angle2) * difference.x * (1 - 2 * (ray_dir.y < 0));
// 	}
// 	else
// 	{
// 		game->depth[x] = cos(angle) * difference.y;
// 		wall_x = game->pos.x + cos(angle2) * difference.y;
// 	}
// 	wall_x -= floor(wall_x);
// 	return (draw_line(x, side, wall_x, game));
// }

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

int	cast_loop(t_vec2d ray_dir, t_vec2d delta_dist, t_vec2d *side_dist, t_game *game)
{
	int		side;
	t_int2d	map;

	map.x = (int)game->pos.x;
	map.y = (int)game->pos.y;
	while (1)
	{
		if (side_dist->x < side_dist->y)
			side = increase(&map.x, &side_dist->x, delta_dist.x, ray_dir.x);
		else
			side = 2 + increase(&map.y, &side_dist->y, delta_dist.y, ray_dir.y);
		if (map.x < 0 && ray_dir.x <= 0)
			return (-1);
		if (map.x >= game->map.width && ray_dir.x >= 0)
			return (-1);
		if (map.y < 0 && ray_dir.y <= 0)
			return (-1);
		if (map.y >= game->map.height && ray_dir.y >= 0)
			return (-1);
		if (map.y >= 0 && map.y < game->map.height && map.x >= 0 &&
			map.x < game->map.width && game->map.grid[map.y][map.x].value == 1)
			return (side);
	}
	return (0);
}

int	calculate_ray(int x, t_vec2d ray_dir, t_game *game)
{
	t_int2d map;
	t_vec2d	delta_dist;
	t_vec2d	side_dist;
	int		side;

	map.x = (int)game->pos.x;
	map.y = (int)game->pos.y;
	delta_dist.x = fabs(vec2d_len(ray_dir) / ray_dir.x);
	delta_dist.y = fabs(vec2d_len(ray_dir) / ray_dir.y);
	if (ray_dir.x < 0)
		side_dist.x = (game->pos.x - map.x) * delta_dist.x;
	else
		side_dist.x = (map.x + 1.0 - game->pos.x) * delta_dist.x;
	if (ray_dir.y < 0)
		side_dist.y = (game->pos.y - map.y) * delta_dist.y;
	else
		side_dist.y = (map.y + 1.0 - game->pos.y) * delta_dist.y;
	side = cast_loop(ray_dir, delta_dist, &side_dist, game);
	if (side == -1)
		return (empty_line(game, x), 0);
	if (side < 2)
		set_depth(x, ray_dir,side_dist.x - delta_dist.x, game);
	else
		set_depth(x, ray_dir,side_dist.y - delta_dist.y, game);
	return (draw_line(x, side, get_wall_x(side, ray_dir, vec2d_sub(side_dist, delta_dist), game), game));
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
