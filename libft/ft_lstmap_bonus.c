/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap_bonus.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/25 17:18:39 by lumartin          #+#    #+#             */
/*   Updated: 2024/09/25 17:37:23 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void *), void (*del)(void *))
{
	t_list	*sol;
	t_list	*new;
	t_list	*tmp;

	sol = NULL;
	while (lst)
	{
		new = f(lst->content);
		if (!new)
		{
			ft_lstclear(&sol, del);
			return (NULL);
		}
		tmp = ft_lstnew(new);
		if (!tmp)
		{
			del(new);
			ft_lstclear(&sol, del);
			return (NULL);
		}
		ft_lstadd_back(&sol, tmp);
		lst = lst->next;
	}
	return (sol);
}
