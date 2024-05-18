/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/19 00:36:52 by padam            ###   ########.fr       */
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
	game->mlx = mlx_init(1000, 800, "Cub3D", 1);
	mlx_set_cursor_mode(game->mlx, MLX_MOUSE_DISABLED);
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

void	key_binds(t_game *game)
{
	t_vec2d	pos_tmp;
	double	dist;

	dist = game->speed * game->mlx->delta_time;
	pos_tmp = game->pos;
	if (mlx_is_key_down(game->mlx, MLX_KEY_W))
		pos_tmp = vec2d_add(pos_tmp, vec2d_mul(game->dir, dist));
	if (mlx_is_key_down(game->mlx, MLX_KEY_S))
		pos_tmp = vec2d_sub(pos_tmp, vec2d_mul(game->dir, dist));
	if (mlx_is_key_down(game->mlx, MLX_KEY_A))
		pos_tmp = vec2d_add(pos_tmp, vec2d_rot(vec2d_mul(game->dir, dist), -M_PI / 2));
	if (mlx_is_key_down(game->mlx, MLX_KEY_D))
		pos_tmp = vec2d_add(pos_tmp, vec2d_rot(vec2d_mul(game->dir, dist), M_PI / 2));
	if (game->map.grid[(int)pos_tmp.y][(int)pos_tmp.x].value == 0
		&& (game->map.grid[(int)pos_tmp.y][(int)game->pos.x].value == 0
		|| game->map.grid[(int)game->pos.y][(int)pos_tmp.x].value == 0))
	{
		game->pos.x = pos_tmp.x;
		game->pos.y = pos_tmp.y;
	}
	else if (game->map.grid[(int)pos_tmp.y][(int)game->pos.x].value == 0
		|| game->map.grid[(int)game->pos.y][(int)pos_tmp.x].value == 0)
	{
		if (game->map.grid[(int)pos_tmp.y][(int)game->pos.x].value == 0)
			game->pos.y = pos_tmp.y;
		else
			game->pos.x = pos_tmp.x;
	}
	if (mlx_is_key_down(game->mlx, MLX_KEY_RIGHT))
		game->dir = vec2d_rot(game->dir, game->turn_speed * game->mlx->delta_time);
	if (mlx_is_key_down(game->mlx, MLX_KEY_LEFT))
		game->dir = vec2d_rot(game->dir, -game->turn_speed * game->mlx->delta_time);
	if (mlx_is_key_down(game->mlx, MLX_KEY_ESCAPE))
		mlx_close_window(game->mlx);
}

/*
 *@brief Function that gets executed every frame.
 *@param game The game structure.
*/
void	loop_hook(void *in)
{
	t_game	*game = (t_game *)in;
	int	x;
	int	y;
	
	key_binds(in);
	mlx_get_mouse_pos(game->mlx, &x, &y);
	game->dir = vec2d_rot(game->dir, (double)(x - game->mlx->width / 2) * game->turn_speed * game->mlx->delta_time / 50);
	mlx_set_mouse_pos(game->mlx, game->mlx->width / 2, game->mlx->height / 2);
	if (game->mlx->height != game->image->height || game->mlx->width != game->image->width)
	{
		mlx_delete_image(game->mlx, game->image);
		game->image = mlx_new_image(game->mlx, game->mlx->width, game->mlx->height);
		mlx_image_to_window(game->mlx, game->image, 0, 0);
	}
	raycast(in);
}

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
	parser(&game, argv[1]);
	printmap(&game);
	initialize(&game);
	// init_test(&game);
	mlx_loop_hook(game.mlx, loop_hook, &game);
	mlx_loop(game.mlx);
	mlx_delete_image(game.mlx, game.image);
	mlx_close_window(game.mlx);
	mlx_terminate(game.mlx);

	i = 0;
	while (i < 4)
		free_str_array((void *)game.textures[i++].grid, &game.textures->height);
	free_str_array((void *)game.map.grid, &game.map.height);
	free(game.textures);
	return (0);
}
