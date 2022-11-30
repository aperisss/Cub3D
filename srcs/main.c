/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aperis <aperis@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/23 14:32:36 by aperis            #+#    #+#             */
/*   Updated: 2022/10/20 18:19:55 by aperis           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../cub.h"

void	empty_map(void)
{
	printf("Error map\n");
	exit (1);
}

int	check_directory(char **av)
{
	int	fd;

	fd = open(av[1], __O_DIRECTORY);
	if (fd > 0)
	{
		printf("error with the map\n");
		close(fd);
		return (0);
	}
	return (1);
}

int	open_fd(char **av)
{
	int	fd;

	fd = open(av[1], O_RDONLY);
	if (fd == -1)
	{
		printf("Error map\n");
		close(fd);
		exit (0);
	}
	return (fd);
}

char	**read_map(t_data *data, char **av)
{
	char	**map;

	data->x = 0;
	data->len = 0;
	data->i = 0;
	data->j = 0;
	data->line = NULL;
	data->fd = open_fd(av);
	data->str = get_next_line(data->fd);
	if (data->str == NULL)
		empty_map();
	while (data->str)
	{
		data->len = check_line(data->str);
		if (data->len > 1)
			data->x = data->len;
		data->line = ft_strjoin2(data->line, data->str);
		free(data->str);
		data->str = get_next_line(data->fd);
	}
	if (data->x != 0)
		verif_error_line(data, data->line, data->x);
	map = ft_split(data->line, '\n');
	return (free(data->line), close(data->fd), ft_space_info(data, map));
}

int	main(int ac, char **av)
{
	t_data	data;

	ft_check_arg(ac, av);
	data.map_tmp = read_map(&data, av);
	get_map_info(&data);
	get_path_text(&data);
	if (parsing(&data) == 0)
	{
		free_path_text(&data);
		free_double_char(data.mapinfo);
		free_double_char(data.map);
		free_double_char(data.map_tmp);
		printf("Error in the map\n");
		return (0);
	}
	data.map[data.psx][data.psy] = '0';
	new_window(&data);
	init_game(&data);
	data.img = mlx_new_image(data.mlx_ptr, WIDTHSCREEN, HEIGHTSCREEN);
	data.addr = (int *)mlx_get_data_addr(data.img,
			&data.bits_per_pixel, &data.line_length, &data.endian);
	write_to_image(&data);
	return (0);
}
