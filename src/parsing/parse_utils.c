/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:56:05 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/29 13:43:56 by antonweizma      ###   ########.fr       */
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
		return (ft_putstr_fd("Error\nWrong Map\n", 2), -1);
	while (1)
	{
		tmp = get_next_line(fd);
		if (!tmp)
			break ;
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
	int		lines;

	lines = countlines(input_file);
	if (lines == -1)
		return (NULL);
	fd = open(input_file, O_RDONLY, 0666);
	if (fd == -1)
		return (ft_putstr_fd("Error\nWrong Map\n", 2), NULL);
	file = ft_calloc(sizeof(char *), lines + 1);
	if (!file)
		return (close(fd), ft_putstr_fd("Error\nAlloc Failure\n", 2), NULL);
	i = 0;
	while (i < lines)
	{
		file[i] = get_next_line(fd);
		file[i][ft_strlen(file[i]) - 1] = '\0';
		if (!file[i])
			break ;
		i++;
	}
	file[lines] = NULL;
	return (close(fd), file);
}

int	check_if_map(char *str)
{
	int	j;

	j = 0;
	while (str[j])
	{
		if (str[j] == '1')
			return (1);
		j++;
	}
	return (0);
}

char	*get_env(char *var)
{
	int			i;
	int			j;
	char		*tmp;
	extern char	**environ;

	i = 0;
	while (environ[i])
	{
		j = 0;
		while (environ[i][j] && environ[i][j] != '=')
			j++;
		tmp = ft_substr(environ[i], 0, j);
		if (!tmp)
			break ;
		if (!ft_strncmp(tmp, var, ft_strlen(var))
			&& !ft_strncmp(tmp, var, ft_strlen(tmp)))
		{
			free(tmp);
			return (ft_strdup(environ[i] + j + 1));
		}
		free(tmp);
		i++;
	}
	return (NULL);
}

int	check_valid_map(t_map *map, int i, int k, int j)
{
	while (i < map->height || i < map->width)
	{
		k = 0;
		j = map->height - 1;
		while (k < map->height - 1 && map->grid[k][i].value == 2)
			k++;
		while (j > k && map->grid[j][i].value == 2)
			j--;
		if (j <= 1 || !map->grid[k][i].value || !map->grid[j][i].value)
			return (1);
		if (i < map->height)
		{
			k = 0;
			j = map->width - 1;
			while (k < map->width - 1 && map->grid[i][k].value == 2)
				k++;
			while (j > k && map->grid[i][j].value == 2)
				j--;
			if (j <= 1 || !map->grid[i][j].value || !map->grid[i][k].value)
				return (1);
		}
		i++;
	}
	return (0);
}
