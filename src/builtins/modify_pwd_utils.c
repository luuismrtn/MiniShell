/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_pwd_utils.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/20 12:36:05 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/20 12:36:34 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Crea una nueva variable PWD en el entorno.
 *
 * Esta función se llama cuando la variable PWD no existe en el entorno
 * y necesita ser creada. Crea una estructura t_env con el nombre "PWD"
 * y un contenido basado en el directorio HOME y el argumento dir.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param dir El directorio a añadir a la ruta.
 */
void	create_new_pwd(t_token **tokens, char *dir)
{
	t_env	*new_var;
	char	*path;

	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup("PWD");
	path = ft_strjoin("/", dir);
	new_var->content = ft_strjoin(search_path(join_env((*tokens)->env_mshell),
				"HOME")[1], path);
	free(path);
	if (!new_var->content)
		return (free(new_var));
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}
