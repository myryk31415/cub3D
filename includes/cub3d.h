/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:11 by padam             #+#    #+#             */
/*   Updated: 2024/05/17 16:32:20 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CUB3D_H
# define CUB3D_H

# include "../libft/includes/libft.h"
# include "../MLX42/include/MLX42/MLX42.h"
# include <math.h>
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

int	raycast(t_game *game);
#endif
