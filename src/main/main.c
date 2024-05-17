/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 21:55:21 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	init_test(t_game *game)
{
	game->map = (t_map){.width = 10, .height = 10};
	game->map.grid = malloc(sizeof(t_pixel *) * game->map.height);
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
			game->textures->grid[i][j].bytes.r = (double)i / game->textures->height * 255;
			game->textures->grid[i][j].bytes.g = 0;
			game->textures->grid[i][j].bytes.b = (double)j / game->textures->width * 255;
			game->textures->grid[i][j].bytes.a = 255;
		}
	}
	game->ceiling.bytes.r = 0;
	game->ceiling.bytes.g = 0;
	game->ceiling.bytes.b = 255;
	game->ceiling.bytes.a = 255;
	game->floor.bytes.r = 0;
	game->floor.bytes.g = 10;
	game->floor.bytes.b = 0;
	game->floor.bytes.a = 255;
}

void	initialize(t_game *game)
{
	game->speed = 3.5;
	game->turn_speed = 3.5;
	game->mlx = mlx_init(1000, 800, "Cub3D", 1);
	game->image = mlx_new_image(game->mlx, 1000, 800);
	mlx_image_to_window(game->mlx, game->image, 0, 0);
}

void	printmap(t_game *game)
{
	for (int i = 0; i < game->map.height; i++)
	{
		for (int j = 0; j < game->map.width; j++)
		{
			printf("%d", game->map.grid[i][j].value);
		}
		printf("\n");
	}
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

	if (argc != 2)
	{
		printf("Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	parser(&game, argv[1]);
	initialize(&game);
	printmap(&game);
	// init_test(&game);
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_loop(game.mlx);
	return (0);
}
