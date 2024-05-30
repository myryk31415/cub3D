/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   loop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/30 10:34:30 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/30 10:36:55 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	collision_check(t_vec2d pos_tmp, t_game *game)
{
	if (game->map.grid[(int)pos_tmp.y][(int)game->pos.x].value == 0
		|| game->map.grid[(int)game->pos.y][(int)pos_tmp.x].value == 0)
	{
		if (game->map.grid[(int)pos_tmp.y][(int)pos_tmp.x].value == 0)
		{
			game->pos.x = pos_tmp.x;
			game->pos.y = pos_tmp.y;
		}
		else if (game->map.grid[(int)pos_tmp.y][(int)game->pos.x].value == 0)
			game->pos.y = pos_tmp.y;
		else
			game->pos.x = pos_tmp.x;
	}
}

void	key_binds(t_game *game)
{
	t_vec2d	pos_tmp;
	double	dist;

	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
	dist = game->speed * game->mlx->delta_time;
	pos_tmp = game->pos;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		pos_tmp = vec2d_add(pos_tmp, vec2d_mul(game->dir, dist));
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		pos_tmp = vec2d_sub(pos_tmp, vec2d_mul(game->dir, dist));
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		pos_tmp = vec2d_add(pos_tmp, vec2d_rot(\
		vec2d_mul(game->dir, dist), -M_PI / 2));
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		pos_tmp = vec2d_add(pos_tmp, vec2d_rot(\
		vec2d_mul(game->dir, dist), M_PI / 2));
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		game->dir = vec2d_rot(game->dir, game->turn_speed \
		* game->mlx->delta_time);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		game->dir = vec2d_rot(game->dir, -game->turn_speed \
		* game->mlx->delta_time);
	collision_check(pos_tmp, game);
}

void	resize_window(t_game *game)
{
	mlx_delete_image(game->mlx, game->image);
	game->image = mlx_new_image(game->mlx, game->mlx->width, game->mlx->height);
	mlx_image_to_window(game->mlx, game->image, 0, 0);
	free(game->depth);
	game->depth = ft_calloc(game->mlx->width, sizeof(double));
	game->init = 5;
}

/*
 *@brief Function that gets executed every frame.
 *@param game The game structure.
*/

void	loop_hook(void *in)
{
	t_game	*game;
	int		x;
	int		y;

	game = (t_game *)in;
	key_binds(in);
	if (game->mlx->height != game->image->height || \
	game->mlx->width != game->image->width)
		resize_window(game);
	if (game->init == 0)
	{
		mlx_get_mouse_pos(game->mlx, &x, &y);
		game->dir = vec2d_rot(game->dir, (double)(x - \
		game->mlx->width / 2) * game->turn_speed * game->mlx->delta_time / 50);
	}
	else
		game->init--;
	mlx_set_mouse_pos(game->mlx, game->mlx->width / 2, game->mlx->height / 2);
	raycast(in);
	sprites(game);
}
