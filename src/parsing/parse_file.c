/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:02:43 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/17 14:53:50 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	countlines(char *input_file)
{
	int		fd;
	int		lines;
	char	*tmp;

	lines = 0;
	fd = open(input_file, O_RDONLY, 0666);
	if (fd == -1)
		return (ft_putstr_fd("Error\n", 2), -1);
	while(1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break;
		lines++;
		free(tmp);
	}
	close(fd);
	return (lines);
}

char	**read_file(char *input_file)
{
	char	**file;
	int		fd;
	int		i;

	fd = open(input_file, O_RDONLY, 0666);
	if (fd == -1)
		return (ft_putstr_fd("Error\n", 2), NULL);
	file = ft_calloc(sizeof(char *), countlines(input_file));
	if (!file)
		return (ft_putstr_fd("Error\n", 2), NULL);
	i = 0;
	while(1)
	{
		file[i] = get_next_line(fd);
		if (!file[i])
			break;
		i++;
	}
	close(fd);
	return (file);
}

t_map	get_texture(char *str)
{

}

t_pixel	get_color(char *str)
{

}

t_map	parse_map(char **file, int i)
{

}

int	check_if_map(char *str)
{

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
			game->floor = get_color(file[i]);
		else if (file[i][0] == 'C')
			game->ceiling = get_color(file[i]);
		else if (check_if_map(file[i]))
			game->map = parse_map(file, i);
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
