/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_map.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/29 13:48:10 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/29 14:34:15 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_max_width(char **file, int i)
{
	int	max_width;

	max_width = 0;
	while (file[i])
	{
		if (ft_strlen(file[i]) > max_width)
			max_width = ft_strlen(file[i]);
		i++;
	}
	return (max_width);
}

void	init_sprites(char **file, t_game *game)
{
	int	i;
	int	j;
	int	num_sprites;

	i = 0;
	num_sprites = 0;
	while (file[i])
	{
		j = 0;
		while (file[i][j])
		{
			if (file[i][j] == 'B')
				num_sprites++;
			j++;
		}
		i++;
	}
	game->sprites = ft_calloc(sizeof(t_sprite), num_sprites);
	game->num_sprites = num_sprites;
}

void	get_dir_spirtes(char **file, t_map *map, t_game *game, t_parse *parse)
{
	if (file[parse->i][parse->k] == 'N' || file[parse->i][parse->k] == 'S' \
		|| file[parse->i][parse->k] == 'E' || file[parse->i][parse->k] == 'W')
	{
		game->pos.x = parse->k + 0.5;
		game->pos.y = parse->j + 0.5;
		parse->start_pos += 1;
		if (file[parse->i][parse->k] == 'N')
			game->dir.y = -1;
		if (file[parse->i][parse->k] == 'S')
			game->dir.y = 1;
		if (file[parse->i][parse->k] == 'E')
			game->dir.x = 1;
		if (file[parse->i][parse->k] == 'W')
			game->dir.x = -1;
	}
	else if (file[parse->i][parse->k] == 'B')
	{
		game->sprites[parse->count].pos.x = parse->k + 0.5;
		game->sprites[parse->count].pos.y = parse->j + 0.5;
		parse->count++;
		map->grid[parse->j][parse->k].value = 3;
	}
	else
		error("Wrong charakter in map\n", game, file);
}

int	fill_value(t_map *map, char **file, t_parse *parse, t_game *game)
{
	map->grid[parse->j] = ft_calloc(sizeof(t_pixel), map->width);
	if (!map->grid[parse->j])
		error("Allocation Failure", game, file);
	parse->k = -1;
	while (++parse->k < map->width)
	{
		if (file[parse->i][parse->k] == ' ')
			map->grid[parse->j][parse->k].value = 2;
		else if (!file[parse->i][parse->k])
			while (parse->k < map->width)
				map->grid[parse->j][parse->k++].value = 2;
		else if (file[parse->i][parse->k] == '0')
			map->grid[parse->j][parse->k].value = 0;
		else if (file[parse->i][parse->k] == '1')
			map->grid[parse->j][parse->k].value = 1;
		else
			get_dir_spirtes(file, map, game, parse);
	}
	parse->i++;
	parse->j++;
	return (0);
}

t_map	parse_map(char **file, int i, t_game *game)
{
	t_map	map;
	t_parse	parse;

	parse.count = 0;
	parse.start_pos = 0;
	map.width = get_max_width(file, i);
	init_sprites(file, game);
	parse.i = i;
	parse.j = parse.i;
	while (file[parse.j])
		parse.j++;
	parse.j = 0;
	map.grid = ft_calloc(sizeof(t_pixel *), parse.j + 1);
	if (!map.grid)
		error("Allocation Failure", game, file);
	while (file[parse.i] && file[parse.i][0] != '\0')
		fill_value(&map, file, &parse, game);
	map.height = parse.j;
	if (!parse.start_pos)
		error("No Players\n", game, file);
	if (parse.start_pos > 1)
		error("Too many Players\n", game, file);
	if (check_valid_map(&map, 0, 0, 0))
		error("Map walls not closed\n", game, file);
	return (map);
}
