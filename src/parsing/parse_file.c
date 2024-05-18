/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:02:43 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 14:52:06 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	fill_map(t_map	*tex, char *tex_path, int i)
{
	mlx_texture_t	*png;
	int				j;

	png = mlx_load_png(tex_path);
	if (!png)
		return (ft_putstr_fd("Error\nTexture Invalid\n", 2), -1);
	tex->grid = ft_calloc(sizeof(t_pixel *), png->height);
	if (!tex->grid)
		return (mlx_delete_texture(png), ft_putstr_fd("Error\nAllocation Failure", 2), -1);
	tex->height = png->height;
	tex->width = png->width;
	while (++i < png->height)
	{
		j = 0;
		tex->grid[i] = ft_calloc(sizeof(t_pixel), png->width);
		if (!tex->grid)
			return (free_str_array((void **)tex->grid, &tex->height), mlx_delete_texture(png), ft_putstr_fd("Error\nAllocation Failure", 2), -1);
		while (j < png->width)
		{
			tex->grid[i][j].bytes.r = png->pixels[i * png->width * 4 + j * 4];
			tex->grid[i][j].bytes.g = png->pixels[i * png->width * 4 + j * 4 + 1];
			tex->grid[i][j].bytes.b = png->pixels[i * png->width * 4 + j * 4 + 2];
			tex->grid[i][j].bytes.a = png->pixels[i * png->width * 4 + j * 4 + 3];
			j++;
		}
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
			break;
	if (!str[i])
		error("Wrong Texture Path", game, file, NULL);
	j = i;
	while (str[j] && str[j] != ' ' && str[j] != '\n')
		j++;
	texture_path = ft_substr(str, i, j - i);
	if (str[i - 1] == '.')
		texture_path = ft_strjoin_free(ft_strjoin_free(get_env("PWD"), \
			ft_strdup("/textures")), texture_path);
	if (fill_map(&map, texture_path, -1) == -1)
	{
		free(texture_path);
		error(NULL, game, file, NULL);
	}
	free(texture_path);
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
	int		start_pos;

	start_pos = 0;
	map = ft_calloc(sizeof(t_map), 1);
	if (!map)
		error("Allocation Failure", game, file, map);
	map->width = ft_strlen(file[i]);
	j = i;
	k = -1;
	while (file[j])
		j++;
	map->grid = ft_calloc(sizeof(t_pixel *), j + 1);
	if (!map->grid)
		error("Allocation Failure", game, file, map);
	j = 0;
	while (file[i])
	{
		map->grid[j] = ft_calloc(sizeof(t_pixel), ft_strlen(file[i]));
		if (!map->grid[j])
			error("Allocation Failure", game, file, map);
		k = -1;
		while (file[i][++k])
		{
			if (file[i][k] == ' ')
				map->grid[j][k].value = 2;
			else if (file[i][k] == '0')
				map->grid[j][k].value = 0;
			else if (file[i][k] == '1')
				map->grid[j][k].value = 1;
			else if (file[i][k] == 'N' || file[i][k] == 'S' || file[i][k] == 'E' || file[i][k] == 'W')
			{
				game->pos.x = k;
				game->pos.y = j;
				game->dir.x = 0;
				game->dir.y = 0;
				start_pos = 1;
				if (file[i][k] == 'N')
					game->dir.y = -1;
				if (file[i][k] == 'S')
					game->dir.y = 1;
				if (file[i][k] == 'E')
					game->dir.x = 1;
				if (file[i][k] == 'W')
					game->dir.x = -1;
			}
			else if (file[i][k] != '\n')
				error("Wrong charakter in map\n", game, file, map);
		}
		j++;
		i++;
	}
	map->height = j;
	if (!start_pos)
		error("No startposition\n", game, file, map);
 	if (check_valid_map(map))
		error("Map walls not closed\n", game, file, map);
	return (map);
}

int	parse_file(t_game *game, char **file)
{
	int	i;

	i = -1;
	game->textures = ft_calloc(sizeof(t_map), 5);
	while (file[++i])
	{
		if (file[i][0] == 'N' && file[i][1] == 'O')
			game->textures[0] = get_texture(file[i], -1, game, file);
		else if (file[i][0] == 'S' && file[i][1] == 'O')
			game->textures[1] = get_texture(file[i], -1, game, file);
		else if (file[i][0] == 'W' && file[i][1] == 'E')
			game->textures[2] = get_texture(file[i], -1, game, file);
		else if (file[i][0] == 'E' && file[i][1] == 'A')
			game->textures[3] = get_texture(file[i], -1, game, file);
		else if (file[i][0] == 'F')
			game->floor = get_color(file[i], 0);
		else if (file[i][0] == 'C')
			game->ceiling = get_color(file[i], 0);
		else if (check_if_map(file[i]))
		{
			game->map = *parse_map(file, i, game);
			break ;
		}
	}
	return (0);
}

int	parser(t_game *game, char *input_file)
{
	char **file;

	file = read_file(input_file);
	if (!file)
		return (ft_putstr_fd("Error\n", 2), -1);
	parse_file(game, file);
	return (0);
}
