/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 17:04:24 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	init_test(t_game *game)
{
	game->pos.x = 1.0;
	game->pos.y = 1.0;
	game->dir.x = 0.0;
	game->dir.y = -1.0;
	game->speed = 2.5;
	game->turn_speed = 2.5;
	game->map = (t_map){.width = 8, .height = 8};
	game->map.grid = malloc(sizeof(t_pixel *) * game->map.height);
	game->mlx = mlx_init(1920, 1080, "Cub3D", 1);
	game->image = mlx_new_image(game->mlx, 1920, 1080);
	mlx_image_to_window(game->mlx, game->image, 0, 0);
	for (int i = 0; i < game->map.height; i++)
	{
		game->map.grid[i] = ft_calloc(game->map.width, sizeof(t_pixel));
		for (int j = 0; j < game->map.width; j++)
		{
			if (i == 0 || i == game->map.height -1 || j == 0 || j == game->map.width - 1)
				game->map.grid[i][j].value = 1;
			else
				game->map.grid[i][j].value = 0;
		}
	}
	game->textures = ft_calloc(1, sizeof(t_map));
	game->textures->width = 64;
	game->textures->height = 64;
	game->textures->grid = malloc(sizeof(t_pixel *) * game->textures->height);
	for (int i = 0; i < game->textures->height; i++)
	{
		game->textures->grid[i] = ft_calloc(game->textures->width, sizeof(t_pixel));
		for (int j = 0; j < game->textures->width; j++)
		{
			game->textures->grid[i][j].bytes.r = ((double)i / game->textures->height) * 255;
			game->textures->grid[i][j].bytes.g = 0;
			game->textures->grid[i][j].bytes.b = ((double)j / game->textures->width) * 255;
			game->textures->grid[i][j].bytes.a = 255;
		}
	}
	game->ceiling.bytes.r = 0;
	game->ceiling.bytes.g = 0;
	game->ceiling.bytes.b = 200;
	game->ceiling.bytes.a = 255;
	game->floor.bytes.r = 0;
	game->floor.bytes.g = 50;
	game->floor.bytes.b = 0;
	game->floor.bytes.a = 255;
}

void	arrow_keys(t_game *game)
{
	if (mlx_is_key_down(game->mlx, MLX_KEY_UP))
		game->pos = vec2d_add(game->pos, vec2d_mul(game->dir, game->speed * game->mlx->delta_time));
	if (mlx_is_key_down(game->mlx, MLX_KEY_DOWN))
		game->pos = vec2d_sub(game->pos, vec2d_mul(game->dir, game->speed * game->mlx->delta_time));
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		game->dir = vec2d_rot(game->dir, -game->turn_speed * game->mlx->delta_time);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		game->dir = vec2d_rot(game->dir, game->turn_speed * game->mlx->delta_time);
}

void	loop_hook(void *game)
{
	arrow_keys(game);
	raycast(game);
}

int	main(int argc, char **argv)
{
	t_game	game;

	(void)argc;
	(void)argv;
	init_test(&game);
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_loop(game.mlx);
	return (0);
}
