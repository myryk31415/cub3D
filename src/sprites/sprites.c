/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:15:28 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/30 09:54:37 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	sort_sprites(int num_sprites, double *sprite_dist, int *sprite_order)
{
	int	i;
	int	j;
	int	tmp_int;
	int	swap;

	j = -1;
	while (++j < num_sprites - 1)
	{
		i = 0;
		swap = 0;
		while (i < num_sprites - 1)
		{
			if (sprite_dist[sprite_order[i]] < sprite_dist[sprite_order[i + 1]])
			{
				tmp_int = sprite_order[i];
				sprite_order[i] = sprite_order[i + 1];
				sprite_order[i + 1] = tmp_int;
				swap = 1;
			}
			i++;
		}
		if (!swap)
			break ;
	}
	return ;
}

static void	init_sprites(t_game *game, double *sprite_dist, int *sprite_order)
{
	static int	i;

	i = 0;
	while (i < game->num_sprites)
	{
		sprite_order[i] = i;
		sprite_dist[i] = (game->pos.x - game->sprites[i].pos.x) * \
		(game->pos.x - game->sprites[i].pos.x) + (game->pos.y - \
		game->sprites[i].pos.y) * (game->pos.y - game->sprites[i].pos.y);
		game->sprites[i].texture = 4;
		i++;
	}
	sort_sprites(game->num_sprites, sprite_dist, sprite_order);
}

void	calculate_sprite(t_game *game, int *sprite_order, \
						int i, double transform)
{
	t_vec2d	sprite_pos;

	transform = 1.0 / (game->camera_plane.x * game->dir.y - \
		game->dir.x * game->camera_plane.y);
	sprite_pos = vec2d_sub(game->sprites[sprite_order[i]].pos, game->pos);
	game->transformed.x = transform * (sprite_pos.x * game->dir.y - \
		sprite_pos.y * game->dir.x);
	game->transformed.y = transform * (-game->camera_plane.y * \
	sprite_pos.x + sprite_pos.y * game->camera_plane.x);
	game->x_screen = (int)(game->mlx->width / 2) * (1. + \
	game->transformed.x / game->transformed.y);
	game->sprite_height = fabs((int)game->mlx->height / game->transformed.y);
	game->sprite_width = fabs((int)game->mlx->height / game->transformed.y);
	game->start_y = -game->sprite_height / 2 + game->mlx->height / 2;
	if (game->start_y < 0)
		game->start_y = 0;
	game->end_y = game->sprite_height / 2 + game->mlx->height / 2;
	if (game->end_y >= game->mlx->height)
		game->end_y = game->mlx->height - 1;
	game->start_x = -game->sprite_width / 2 + game->x_screen;
	if (game->start_x < 0)
		game->start_x = 0;
	game->end_x = game->sprite_width / 2 + game->x_screen;
	if (game->end_x >= game->mlx->width)
		game->end_x = game->mlx->width - 1;
}

void	draw_sprite(t_game *game, int *sprite_order, t_map texture)
{
	int			j;
	int			k;
	int			t_x;
	int			t_y;

	j = game->start_x - 1;
	while (++j < game->end_x)
	{
		t_x = (int)((((j) - (-game->sprite_width / 2. + game->x_screen)))
				* texture.width / game->sprite_width);
		if (game->transformed.y > 0 && j > 0 && j < game->mlx->width \
		&& game->transformed.y < game->depth[j])
		{
			k = game->start_y - 1;
			while (game->start_y >= 0 && ++k < game->end_y)
			{
				t_y = ((k * 256 - game->mlx->height * 128 + game->sprite_height \
				* 128) * texture.height) / game->sprite_height / 256;
				if (t_x >= 0 && t_y >= 0 && t_x < texture.width && t_y \
				< texture.height && texture.grid[t_y][t_x].value != 255)
					mlx_put_pixel(game->image, j, k, \
					texture.grid[t_y][t_x].value);
			}
		}
	}
}

int	sprites(t_game *game)
{
	int		*sprite_order;
	double	*sprite_dist;
	int		i;

	sprite_order = ft_calloc(sizeof(int), game->num_sprites);
	sprite_dist = ft_calloc(sizeof(double), game->num_sprites);
	init_sprites(game, sprite_dist, sprite_order);
	i = 0;
	while (i < game->num_sprites)
	{
		calculate_sprite(game, sprite_order, i, 0);
		draw_sprite(game, sprite_order, \
		game->textures[game->sprites[sprite_order[i]].texture]);
		i++;
	}
	free(sprite_dist);
	free(sprite_order);
	return (0);
}
