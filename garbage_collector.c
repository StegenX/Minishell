/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   garbage_collector.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zael-mou <zael-mou@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/26 16:15:50 by zael-mou          #+#    #+#             */
/*   Updated: 2025/07/09 11:41:13 by zael-mou         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_free_all(t_flags *flags)
{
	t_list	*temp;

	while (flags->garbage_collector)
	{
		temp = flags->garbage_collector;
		flags->garbage_collector = flags->garbage_collector->next;
		free(temp->content);
		free(temp);
	}
	if (flags->infile_fd > 0)
		ft_close(&flags->infile_fd);
	if (flags->outfile_fd > 0)
		ft_close(&flags->outfile_fd);
	if (flags->here_doc_fd > 0)
		ft_close(&flags->here_doc_fd);
}

t_list	*ft_lstnew(void *content)
{
	t_list	*new;

	new = (t_list *)malloc(sizeof(t_list));
	if (!new)
		return (NULL);
	new->content = content;
	new->next = NULL;
	return (new);
}

void	ft_lstadd_back(t_list **lst, t_list *new)
{
	t_list	*tmp;

	if (!*lst)
	{
		*lst = new;
		new->next = NULL;
	}
	else
	{
		tmp = *lst;
		while (tmp->next)
			tmp = tmp->next;
		tmp->next = new;
		new->next = NULL;
	}
}

void	*ft_malloc(size_t size, t_flags *flags)
{
	void	*ptr;
	t_list	*temp;

	ptr = ft_memset(malloc(size), 0, size);
	if (!ptr)
		return (NULL);
	temp = ft_lstnew(ptr);
	if (!temp)
	{
		free(ptr);
		malloc_failed(__func__, __LINE__, flags, flags->built_in_fork);
	}
	if (!flags->garbage_collector)
		flags->garbage_collector = temp;
	else
		ft_lstadd_back(&flags->garbage_collector, temp);
	return (ptr);
}

void	add_array(t_flags *flags, char **array)
{
	int		i;
	t_list	*temp;

	i = 0;
	while (array[i])
	{
		temp = ft_lstnew(array[i++]);
		ft_lstadd_back(&flags->garbage_collector, temp);
	}
	ft_lstadd_back(&flags->garbage_collector, ft_lstnew(array));
}
