/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 20:22:48 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

void	init_test(t_game *game)
{
	game->pos.x = 2.0;
	game->pos.y = 2.0;
	game->map = (t_map){.width = 10, .height = 10};
	game->map.grid = malloc(sizeof(t_pixel *) * game->map.height);
	game->mlx = mlx_init(1000, 800, "Cub3D", 1);
	game->image = mlx_new_image(game->mlx, 1000, 800);
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

// void	loop_hook(void *game)
// {
// 	raycast(game);
// }

// int	main(int argc, char **argv)
// {
// 	// t_game	game;
// 	mlx_texture_t	*test;
// 	(void)argc;
// 	(void)argv;
// 	// init_test(&game);
// 	// mlx_loop_hook(game.mlx, loop_hook, &game);
// 	// mlx_loop(game.mlx);
// 	test = mlx_load_png("/Users/antonweizmann/Desktop/42Heilbronn/curriculum/cub3d/src/floor_1.png");

// 		ft_printf("hello\n");
// 	return (0);
// }

int	main(int argc, char **argv)
{
	t_game	*game;
	if (argc < 2)
		return (0);
	game = ft_calloc(sizeof(game), 1);
	parser(game, argv[1]);
	ft_printf("done\n");
}
