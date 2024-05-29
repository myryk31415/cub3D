/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_color_texture.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:46:16 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/29 17:13:13 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	assign_color(t_pixel *color, int *i, int count, char *str)
{
	char	*tmp;
	int		j;

	while (str[++*i])
	{
		if (ft_isdigit(str[*i]))
		{
			j = *i;
			while (ft_isdigit(str[j]))
				j++;
			tmp = ft_substr(str, *i, j - *i);
			if (count == 0)
				color->bytes.r = (uint8_t)ft_atoi(tmp);
			else if (count == 1)
				color->bytes.g = (uint8_t)ft_atoi(tmp);
			else if (count == 2)
				color->bytes.b = (uint8_t)ft_atoi(tmp);
			count++;
			free(tmp);
			*i = j;
		}
	}
	return (count);
}

t_pixel	get_color(char *str, int count, t_game *game, char **file)
{
	int		i;
	int		j;
	t_pixel	color;
	char	*tmp;

	color.bytes.a = 255;
	i = -1;
	count = assign_color(&color, &i, count, str);
	if (count < 3)
		error("RGB Colors missing\n", game, file);
	return (color);
}

int	fill_map_color(t_map *tex, int i, int j, mlx_texture_t *png)
{
	tex->grid[i] = ft_calloc(sizeof(t_pixel), png->width);
	if (!tex->grid)
		return (free_str_array((void **)tex->grid, &tex->height), \
mlx_delete_texture(png), ft_putstr_fd("Error\nAllocation Failure", 2), -1);
	while (j < png->width)
	{
		tex->grid[i][j].bytes.r = png->pixels[i * png->width * 4 + j * 4];
		tex->grid[i][j].bytes.g = png->pixels[i * png->width * 4 \
		+ j * 4 + 1];
		tex->grid[i][j].bytes.b = png->pixels[i * png->width * 4 \
		+ j * 4 + 2];
		tex->grid[i][j].bytes.a = png->pixels[i * png->width * 4 \
		+ j * 4 + 3];
		j++;
	}
	return (0);
}

int	fill_map(t_map	*tex, char *tex_path, int i)
{
	mlx_texture_t	*png;
	int				j;

	png = mlx_load_png(tex_path);
	if (!png)
		return (ft_putstr_fd("Error\nTexture Invalid\n", 2), -1);
	tex->grid = ft_calloc(sizeof(t_pixel *), png->height);
	if (!tex->grid)
		return (mlx_delete_texture(png), \
			ft_putstr_fd("Error\nAllocation Failure", 2), -1);
	tex->height = png->height;
	tex->width = png->width;
	while (++i < png->height)
	{
		if (fill_map_color(tex, i, 0, png) == -1)
			return (-1);
	}
	mlx_delete_texture(png);
	return (0);
}

t_map	get_texture(char *str, int i, t_game *game, char **file)
{
	char	*texture_path;
	t_map	map;
	int		j;

	while (str[++i])
		if (str[i] == '/')
			break ;
	if (!str[i])
		error("Wrong Texture Path", game, file);
	j = i;
	while (str[j] && str[j] != ' ' && str[j] != '\n')
		j++;
	texture_path = ft_substr(str, i, j - i);
	if (str[i - 1] == '.')
		texture_path = ft_strjoin_free(get_env("PWD"), texture_path);
	if (fill_map(&map, texture_path, -1) == -1)
	{
		free(texture_path);
		error(NULL, game, file);
	}
	free(texture_path);
	return (map);
}
