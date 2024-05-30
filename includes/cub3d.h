/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cub3d.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:11 by padam             #+#    #+#             */
/*   Updated: 2024/05/30 11:28:31 by antonweizma      ###   ########.fr       */
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

# ifndef SPEED
#  define SPEED 3.5
# endif

# ifndef TURN_SPEED
#  define TURN_SPEED 3.5
# endif

# ifndef WALL_HEIGHT
#  define WALL_HEIGHT 0.5
# endif

# define START_HEIGHT 800
# define START_WIDTH 1000
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

typedef struct s_sprite
{
	t_vec2d	pos;
	int		texture;
}	t_sprite;

typedef struct s_int2d
{
	int	x;
	int	y;
}	t_int2d;

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
	double		wall_height;
	double		speed;
	float		turn_speed;
	t_vec2d		camera_plane;
	int			fov_factor;
	double		*depth;
	int			num_sprites;
	t_sprite	*sprites;
	t_vec2d		transformed;
	int			start_y;
	int			end_y;
	int			start_x;
	int			end_x;
	int			init;
	int			sprite_width;
	int			sprite_height;
	int			x_screen;
}	t_game;

typedef struct s_parse
{
	int	i;
	int	k;
	int	j;
	int	start_pos;
	int	count;
}	t_parse;
// Main Functions
int		raycast(t_game *game);
int		sprites(t_game *game);
void	loop_hook(void *in);

// Parsing Utils
int		check_if_map(char *str);
char	**read_file(char *input_file);
int		countlines(char *input_file);
char	*get_env(char *var);
int		check_valid_map(t_map *map, int i, int k, int j);

// Parsing Main
t_map	get_texture(char *str, int i, t_game *game, char **file);
int		parser(t_game *game, char *input_file);
int		parse_file(t_game *game, char **file, int i);
t_map	parse_map(char **file, int i, t_game *game);
t_pixel	get_color(char *str, int count, t_game *game, char **file);

// Raycast Utils
int		increase(int *map, double *side_dist, \
		double delta_dist, double ray_dir);
void	set_depth(int x, t_vec2d ray_dir, double difference, t_game *game);
double	get_wall_x(int side, t_vec2d ray_dir, t_vec2d difference, t_game *game);
void	empty_line(t_game *game, int x);
// Cleanup
void	free_str_array(void **arr, int *n);
void	error(char *msg, t_game *game, char **file, int sprite_allocated);
void	error_sprite(void);
#endif
