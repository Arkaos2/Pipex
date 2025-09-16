/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: saibelab <saibelab@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 14:39:38 by saibelab          #+#    #+#             */
/*   Updated: 2025/06/09 15:40:10 by saibelab         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include"libft.h"

char	*extract_line(char *stock)
{
	char	*new;
	int		i;

	new = NULL;
	i = 0;
	if (!stock || stock[0] == '\0')
		return (NULL);
	while (stock[i] && stock[i] != '\n')
		i++;
	if (stock[i] == '\n')
		i++;
	new = malloc(sizeof(char) * (i + 1));
	if (!new)
		return (NULL);
	ft_strncpy(new, stock, i);
	new[i] = '\0';
	return (new);
}

char	*remove_line(char *stock)
{
	int		i;
	int		j;
	char	*new_stock;

	if (!stock || stock[0] == '\0')
		return (NULL);
	i = 0;
	j = 0;
	while (stock[i] && stock[i] != '\n')
		i++;
	if (!stock[i])
	{
		free(stock);
		return (NULL);
	}
	i++;
	while (stock[i + j])
		j++;
	new_stock = malloc(sizeof(char) * (j + 1));
	if (!new_stock)
		return (NULL);
	ft_strncpy(new_stock, stock + i, j);
	new_stock[j] = '\0';
	free (stock);
	return (new_stock);
}

char	*read_to_stock(int fd, char *stock)
{
	char	*buf;
	ssize_t	bytes;

	buf = malloc(BUFFER_SIZE + 1);
	if (!buf)
		return (NULL);
	while (find_newline(stock) == -1)
	{
		bytes = read(fd, buf, BUFFER_SIZE);
		if (bytes < 0)
		{
			free(buf);
			if (stock)
				free(stock);
			return (NULL);
		}
		if (bytes == 0)
			break ;
		buf[bytes] = '\0';
		stock = append(stock, buf);
		if (!stock)
			return (free(buf), NULL);
	}
	free(buf);
	return (stock);
}

char	*get_next_line(int fd)
{
	static char	*stock[1024];
	char		*line;

	if (fd < 0 || BUFFER_SIZE <= 0 || fd >= 1024)
		return (NULL);
	if (!stock[fd])
	{
		stock[fd] = malloc(1);
		if (!stock[fd])
			return (NULL);
		stock[fd][0] = '\0';
	}
	stock[fd] = read_to_stock(fd, stock[fd]);
	if (!stock[fd])
		return (NULL);
	if (!*stock[fd])
	{
		free(stock[fd]);
		stock[fd] = NULL;
		return (NULL);
	}
	line = extract_line(stock[fd]);
	stock[fd] = remove_line(stock[fd]);
	return (line);
}
