/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/30 11:54:20 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

/*
 *@brief Sets speed, turn_speed, fov_factor, mlx, and image.
 *@param game The game structure to initialize.
*/
void	initialize(t_game *game)
{
	game->speed = SPEED;
	game->turn_speed = TURN_SPEED;
	game->wall_height = WALL_HEIGHT;
	game->fov_factor = 1;
	game->mlx = mlx_init(START_WIDTH, START_HEIGHT, "Cub3D", 1);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
	game->image = mlx_new_image(game->mlx, START_WIDTH, START_HEIGHT);
	mlx_image_to_window(game->mlx, game->image, 0, 0);
	mlx_set_mouse_pos(game->mlx, game->mlx->width / 2, game->mlx->height / 2);
	game->init = 5;
	game->depth = ft_calloc(START_WIDTH, sizeof(double));
}

// void	printmap(t_game *game)
// {
// 	for (int i = 0; i < game->map.height; i++)
// 	{
// 		for (int j = 0; j < game->map.width; j++)
// 		{
// 			printf("%d", game->map.grid[i][j].value);
// 		}
// 		printf("\n");
// 	}
// }

/*
 *@brief A raycasting engine inspired by Wolfenstein 3D.
 *@param argv Path to the map file.
*/
int	main(int argc, char **argv)
{
	t_game	game;
	int		i;

	if (argc != 2)
	{
		printf("Usage: %s <map.cub>\n", argv[0]);
		return (1);
	}
	game.dir.x = 0;
	game.dir.y = 0;
	parser(&game, argv[1]);
	initialize(&game);
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_loop(game.mlx);
	mlx_terminate(game.mlx);
	i = 0;
	while (i < 5)
		free_str_array((void *)game.textures[i++].grid, &game.textures->height);
	free_str_array((void *)game.map.grid, &game.map.height);
	free(game.sprites);
	free(game.textures);
	free(game.depth);
	return (0);
}
