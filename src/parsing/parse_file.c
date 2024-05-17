/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:02:43 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/17 16:17:35 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


t_map	get_texture(char *str)
{

}

t_pixel	get_color(char *str, int count)
{
	int		i;
	int		j;
	t_pixel	color;
	char	*tmp;

	color.bytes.a = 255;
	i = -1;
	j = i;
	while (str[++i])
		if (ft_isdigit(str[j]))
		{
			while (ft_isdigit(str[j]))
				j++;
			tmp = ft_substr(str, i, j - i);
			if (count == 0)
				color.bytes.r = ft_atoi(tmp);
			else if (count == 1)
				color.bytes.g = ft_atoi(tmp);
			else if (count == 2)
				color.bytes.b = ft_atoi(tmp);
			count++;
			free(tmp);
		}
	return (color);
}

t_map	*parse_map(char **file, int i)
{
	t_map	map;
	int		j;
	int		k;

	map.width = ft_strlen(file[i]);
	j = i;
	k = -1;
	while (file[j])
		j++;
	map.grid = ft_calloc(sizeof(t_pixel *), j + 1);
	if (!map.grid)
		return (ft_putstr_fd("Error\n", 2), NULL);
	j = 0;
	while (file[i])
	{
		map.grid[j] = ft_calloc(sizeof(t_pixel), ft_strlen(file[i]));
		if (!map.grid[j])
			return (ft_putstr_fd("Error\n", 2), NULL);
		while (file[i][++k])
			map.grid[j][k].value = ft_atoi(file[i][k]);
		j++;
		i++;
	}
	map.height = j;
	return (&map);
}

int	parse_file(t_game *game, char **file)
{
	int	i;

	i = 0;
	game->textures = ft_calloc(sizeof(t_map), 5);
	while (file[i])
	{
		if (file[i][0] == 'N' && file[i][1] == 'O')
			game->textures[0] = get_texture(file[i]);
		else if (file[i][0] == 'S' && file[i][1] == 'O')
			game->textures[1] = get_texture(file[i]);
		else if (file[i][0] == 'W' && file[i][1] == 'E')
			game->textures[2] = get_texture(file[i]);
		else if (file[i][0] == 'E' && file[i][1] == 'A')
			game->textures[3] = get_texture(file[i]);
		else if (file[i][0] == 'F')
			game->floor = get_color(file[i], 0);
		else if (file[i][0] == 'C')
			game->ceiling = get_color(file[i], 0);
		else if (check_if_map(file[i]))
		{
			game->map = *parse_map(file, i);
			break ;
		}
		i++;
	}
}

int	parser(t_game *game, char *input_file)
{
	char **file;

	file = read_file(input_file);
	if (file == NULL)
		return (ft_putstr_fd("Error\n", 2), -1);
}
