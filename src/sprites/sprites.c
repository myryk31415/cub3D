/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sprites.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/19 11:15:28 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/20 11:44:57 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	sort_sprites(int num_sprites, double *sprite_dist, int *sprite_order)
{
	int	i;
	int	j;
	int		tmp_int;

	i = 0;
	if (num_sprites == 1)
		return ;
	while (i < num_sprites)
	{
		if (sprite_dist[sprite_order[i]] < sprite_dist[sprite_order[i + 1]])
		{
			tmp_int = sprite_order[i];
			sprite_order[i] = sprite_order[i + 1];
			sprite_order[i + 1] = tmp_int;
		}
		j = 0;
		while (j < num_sprites && sprite_dist[sprite_order[j]] > sprite_dist[sprite_order[j + 1]])
			j++;
		if (j == num_sprites)
			break;
	}
	return ;
}

void	init_sprites(t_game *game, double *sprite_dist, int *sprite_order)
{
	static int	i;

	i = 0;
	while (i < game->num_sprites)
	{
		sprite_order[i] = i;
		sprite_dist[i] = (game->pos.x - game->sprites[i].pos.x) * \
		(game->pos.x - game->sprites[i].pos.x) + (game->pos.y - \
		game->sprites[i].pos.y) * (game->pos.y- game->sprites[i].pos.y);
		game->sprites[i].texture = 4;
		i++;
	}
	sort_sprites(game->num_sprites, sprite_dist, sprite_order);
}

void	calculate_sprite(t_game *game, int *sprite_order, int i)
{
	t_vec2d	sprite_pos;
	double	transform;

	transform = 1.0 / (game->camera_plane.x * game->dir.y - \
		game->dir.x * game->camera_plane.y);
	sprite_pos = vec2d_sub(game->sprites[sprite_order[i]].pos, game->pos);
	game->transformed.x = transform * (sprite_pos.x * game->dir.y - \
		sprite_pos.y * game->dir.x);
	game->transformed.y = transform * (-game->camera_plane.y * sprite_pos.x + sprite_pos.y * game->camera_plane.x);
	// game->transformed = vec2d_sub(game->sprites[sprite_order[i]].pos, game->pos);
	// vec2d_rot(game->transformed, M_PI / 2);
	game->sprite_x_screen = (int)(game->mlx->width / 2) * (1. + game->transformed.x / game->transformed.y);
	game->sprite_height = fabs((int)game->mlx->height / game->transformed.y);
	game->sprite_width = fabs((int)game->mlx->height / game->transformed.y);
	printf("transformed y: %f\n", game->transformed.y);
	game->start_y = -game->sprite_height / 2 + game->mlx->height / 2;
	if (game->start_y < 0)
		game->start_y = 0;
	game->end_y = game->sprite_height / 2 + game->mlx->height / 2;
	if (game->end_y >= game->mlx->height)
		game->end_y = game->mlx->height - 1;
	game->start_x = -game->sprite_width / 2 + game->sprite_x_screen;
	if (game->start_x < 0)
		game->start_x = 0;
	game->end_x = game->sprite_width / 2 + game->sprite_x_screen;
	if (game->end_x >= game->mlx->width)
		game->end_x = game->mlx->width - 1;
	// printf("start pos x: %d pos y: %d\n", game->start_x, game->start_y);
}

void	draw_sprite(t_game *game, int *sprite_order, int i)
{
	int			j;
	int			k;
	int			tex_x;
	int			tex_y;
	uint32_t	color;
	int			tmp;
	
	j = game->start_x;
	while (j < game->end_x)
	{
			color = 255;
		tex_x = (int)(256 * (((int)(j) - (-game->sprite_width / 2. + game->sprite_x_screen)))
			* game->textures[game->sprites[sprite_order[i]].texture].width / game->sprite_width) / 256;
		// tex_x = ((j - game->start_x) * game->textures[game->sprites[sprite_order[i]].texture].width / game->sprite_width);
		if (game->transformed.y > 0 && j > 0 && j < game->mlx->width && game->transformed.y < game->depth[j])
		{
			k = game->start_y;
			while (k < game->end_y)
			{
				tmp = ((int)(k) * 256.) - (game->mlx->height * 128.)
							+ (game->sprite_height * 128.);
				tex_y = ((tmp * game->textures[game->sprites[sprite_order[i]].texture].height) / game->sprite_height) / 256.;
				// tex_y = ((k - game->mlx->height + game->sprite_height) * game->textures[game->sprites[sprite_order[i]].texture].height) / game->sprite_height;
				if (tex_x < game->textures[game->sprites[sprite_order[i]].texture].width && tex_y < game->textures[game->sprites[sprite_order[i]].texture].height)
					color = game->textures[game->sprites[sprite_order[i]].texture].grid[tex_x][tex_y].value;
				// if (color != 255)
					mlx_put_pixel(game->image, k, j, color);
				k++;
			}
		}
		j++;
	}
}

int	sprites(t_game *game)
{
	int		sprite_order[game->num_sprites];
	double	sprite_dist[game->num_sprites];
	int		i;

	init_sprites(game, sprite_dist, sprite_order);
	i = 0;
	while (i < game->num_sprites)
	{
		calculate_sprite(game, sprite_order, i);
		draw_sprite(game, sprite_order, i);
		i++;
	}
	return (0);
}
