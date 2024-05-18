/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:02:43 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 10:26:09 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	fill_map(t_map	*tex, char *tex_path, int i)
{
	mlx_texture_t	*png;
	t_pixel			**pixels;
	int				j;

	png = mlx_load_png(tex_path);
	if (!png)
		return (ft_putstr_fd("Error\nTexture Invalid\n", 2), -1);
	pixels = ft_calloc(sizeof(t_pixel *), png->height);
	if (!pixels)
		return (ft_putstr_fd("Error\nAllocation Failure", 2), -1);
	tex->height = png->height;
	tex->width = png->width;
	while (++i < png->height)
	{
		j = 0;
		pixels[i] = ft_calloc(sizeof(t_pixel), png->width);
		while (j < png->width)
		{
			pixels[i][j].bytes.r = png->pixels[i * png->width * 4 + j * 4];
			pixels[i][j].bytes.g = png->pixels[i * png->width * 4 + j * 4 + 1];
			pixels[i][j].bytes.b = png->pixels[i * png->width * 4 + j * 4 + 2];
			pixels[i][j].bytes.a = png->pixels[i * png->width * 4 + j * 4 + 3];
			j++;
		}
	}
	tex->grid = pixels;
	mlx_delete_texture(png);
	return (0);
}

t_map	*get_texture(char *str, int i, int j)
{
	char	*texture_path;
	t_map	*map;
	char	*pwd;

	map = ft_calloc(sizeof(map), 1);
	if (!map)
		return (ft_putstr_fd("Error\nAllocation Failure", 2), NULL);
	while (str[++i])
		if (str[i] == '/')
			break;
	if (!str[i])
		return (ft_putstr_fd("Error\nWrong Texture Path", 2), NULL);
	j = i;
	while (str[j] != ' ' && str[j] != '\n')
		j++;
	texture_path = ft_substr(str, i, j - i);
	if (str[i - 1] == '.')
	{
		pwd = ft_strjoin_free(get_env("PWD"), ft_strdup("/textures"));
		texture_path = ft_strjoin_free(pwd, texture_path);
	}
	if (fill_map(map, texture_path, -1) == -1)
		return (NULL);
	return (map);
}

t_pixel	get_color(char *str, int count)
{
	int		i;
	int		j;
	t_pixel	color;
	char	*tmp;

	color.bytes.a = 255;
	i = -1;
	while (str[++i])
		if (ft_isdigit(str[i]))
		{
			j = i;
			while (ft_isdigit(str[j]))
				j++;
			tmp = ft_substr(str, i, j - i);
			if (count == 0)
				color.bytes.r = (uint8_t)ft_atoi(tmp);
			else if (count == 1)
				color.bytes.g = (uint8_t)ft_atoi(tmp);
			else if (count == 2)
				color.bytes.b = (uint8_t)ft_atoi(tmp);
			count++;
			free(tmp);
			i = j;
		}
	return (color);
}

t_map	*parse_map(char **file, int i, t_game *game)
{
	t_map	*map;
	int		j;
	int		k;

	map = ft_calloc(sizeof(t_map), 1);
	map->width = ft_strlen(file[i]);
	j = i;
	k = -1;
	while (file[j])
		j++;
	map->grid = ft_calloc(sizeof(t_pixel *), j + 1);
	if (!map->grid)
		return (ft_putstr_fd("Error\n", 2), NULL);
	j = 0;
	while (file[i])
	{
		map->grid[j] = ft_calloc(sizeof(t_pixel), ft_strlen(file[i]));
		if (!map->grid[j])
			return (ft_putstr_fd("Error\n", 2), NULL);
		k = -1;
		while (file[i][++k])
		{
			if (file[i][k] == ' ')
				map->grid[j][k].value = 2;
			if (file[i][k] == '0')
				map->grid[j][k].value = 0;
			if (file[i][k] == '1')
				map->grid[j][k].value = 1;
			if (file[i][k] == 'N' || file[i][k] == 'S' || file[i][k] == 'E' || file[i][k] == 'W')
			{
				game->pos.x = k;
				game->pos.y = j;
				game->dir.x = 0;
				game->dir.y = 0;
				if (file[i][k] == 'N')
					game->dir.y = -1;
				if (file[i][k] == 'S')
					game->dir.y = 1;
				if (file[i][k] == 'E')
					game->dir.x = 1;
				if (file[i][k] == 'W')
					game->dir.x = -1;
			}
		}
		j++;
		i++;
	}
	map->height = j;
 	if (check_valid_map(map))
	{
		ft_putstr_fd("Error\nmap walls not closed", 2);
		return (NULL);
	}
	return (map);
}

int	parse_file(t_game *game, char **file)
{
	int	i;

	i = 0;
	game->textures = ft_calloc(sizeof(t_map), 5);
	while (file[i])
	{
		if (file[i][0] == 'N' && file[i][1] == 'O')
			game->textures[0] = *get_texture(file[i], -1, 0);
		else if (file[i][0] == 'S' && file[i][1] == 'O')
			game->textures[1] = *get_texture(file[i], -1, 0);
		else if (file[i][0] == 'W' && file[i][1] == 'E')
			game->textures[2] = *get_texture(file[i], -1, 0);
		else if (file[i][0] == 'E' && file[i][1] == 'A')
			game->textures[3] = *get_texture(file[i], -1, 0);
		else if (file[i][0] == 'F')
			game->floor = get_color(file[i], 0);
		else if (file[i][0] == 'C')
			game->ceiling = get_color(file[i], 0);
		else if (check_if_map(file[i]))
		{
			game->map = *parse_map(file, i, game);
			break ;
		}
		i++;
	}
	return (0);
}

int	parser(t_game *game, char *input_file)
{
	char **file;

	file = read_file(input_file);
	if (file == NULL)
		return (ft_putstr_fd("Error\n", 2), -1);
	if (parse_file(game, file) == -1)
		return (ft_putstr_fd("Error\n", 2), -1);
	return (0);
}
