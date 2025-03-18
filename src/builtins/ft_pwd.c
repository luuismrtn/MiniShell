/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_pwd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:15:28 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/18 12:47:17 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Implementa el comando pwd.
 *
 * Muestra la ruta absoluta del directorio de trabajo actual.
 * Primero intenta obtener la ruta mediante la función getcwd().
 * Si esto falla (por ejemplo, si el directorio actual ha sido eliminado),
 * utiliza como respaldo el valor almacenado en la variable de entorno PWD.
 *
 * El comando pwd no acepta opciones ni argumentos en esta implementación.
 *
 * Comportamiento:
 * - Muestra la ruta absoluta del directorio actual
 * - No modifica ninguna variable de entorno
 * - No cambia el código de salida (mantiene exit_num sin cambios)
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * Necesario para acceder a la variable PWD como respaldo.
 */
void	ft_pwd(t_token *tokens)
{
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (!cwd)
		cwd = get_env_content(tokens->env_mshell, "PWD");
	ft_putendl_fd(cwd, 1);
}
