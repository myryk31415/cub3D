/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 14:56:05 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/18 00:16:04 by antonweizma      ###   ########.fr       */
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
		file[i] = ft_strtrim(file[i], "\n");
		if (!file[i])
			break;
		i++;
	}
	close(fd);
	return (file);
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
