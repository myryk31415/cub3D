/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_file.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: antonweizmann <antonweizmann@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/17 11:02:43 by antonweizma       #+#    #+#             */
/*   Updated: 2024/05/29 14:44:04 by antonweizma      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_file(t_game *game, char **file, int i)
{
	game->textures = ft_calloc(sizeof(t_map), 6);
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
			game->floor = get_color(file[i], 0, game, file);
		else if (file[i][0] == 'C')
			game->ceiling = get_color(file[i], 0, game, file);
		else if (file[i][0] == 'P')
			game->textures[4] = get_texture(file[i], -1, game, file);
		else if (check_if_map(file[i]))
		{
			game->map = parse_map(file, i, game);
			break ;
		}
	}
	free_str_array((void **)file, NULL);
	return (0);
}

int	parser(t_game *game, char *input_file)
{
	char	**file;

	file = read_file(input_file);
	if (!file)
		exit(1);
	parse_file(game, file, -1);
	return (0);
}
