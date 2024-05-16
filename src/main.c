/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/08 22:43:17 by padam             #+#    #+#             */
/*   Updated: 2024/05/16 22:26:53 by padam            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	t_vec3d	vec;

	vec.x = 10.0;
	vec.y = 8.0;
	vec.z = 2.0;
	if (argc == 1)
	{
		ft_printf("Usage: %s <arg1> <arg2> <arg3> <arg4>\n", argv[0]);
		return (1);
	}
	printf("Magnitude %f\n", vec3d_len(vec));
	return (0);
}
