/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:11 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 21:55:42 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/includes/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include <math.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/uio.h>
# include <unistd.h>
# include <stdio.h>

typedef union u_pixel
{
	uint32_t	value;
	struct s_pixel
	{
		uint8_t	a;
		uint8_t	b;
		uint8_t	g;
		uint8_t	r;
	}	bytes;
}	t_pixel;

typedef struct s_map
{
	int		width;
	int		height;
	t_pixel	**grid;
}	t_map;

typedef struct s_game
{
	mlx_t		*mlx;
	mlx_image_t	*image;
	t_map		map;
	t_map		*textures;
	t_pixel		ceiling;
	t_pixel		floor;
	t_vec2d		pos;
	t_vec2d		dir;
	double		speed;
	float		turn_speed;
	t_vec2d		camera_plane;
	int			fov_factor;
}	t_game;

int		raycast(t_game *game);

// Parsing Utils
int		check_if_map(char *str);
char	**read_file(char *input_file);
int		countlines(char *input_file);

// Parsing Main
int		parser(t_game *game, char *input_file);
int		parse_file(t_game *game, char **file);
t_map	*parse_map(char **file, int i, t_game *game);
t_pixel	get_color(char *str, int count);
t_map	*get_texture(char *str);
#endif
