/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:59:37 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/18 23:32:28 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Verifica si un nombre de variable es válido.
 *
 * Comprueba que el nombre de la variable siga las reglas de bash:
 * - No puede estar vacío
 * - Debe comenzar con una letra o guión bajo
 * - Solo puede contener letras, números o guiones bajos
 *
 * @param name El nombre de la variable a verificar.
 * @return int 1 si el nombre es válido, 0 en caso contrario.
 */
int	is_valid_var_name(char *name)
{
	int	i;

	if (!name || name[0] == '\0')
		return (0);
	i = 0;
	if (!(ft_isalpha(name[i]) || name[i] == '_'))
		return (0);
	i++;
	while (name[i])
	{
		if (!(ft_isalnum(name[i]) || name[i] == '_'))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Extrae el nombre de la variable de un argumento.
 *
 * Separa el nombre de la variable de su valor, utilizando el
 * signo igual ('=') como delimitador. Si no hay un signo igual,
 * se considera que todo el argumento es el nombre de la variable.
 *
 * Ejemplos:
 * - "VAR=value" → Devuelve "VAR"
 * - "VAR" → Devuelve "VAR"
 *
 * @param arg Cadena que contiene el argumento completo.
 * @return char* El nombre de la variable extraído (debe ser liberado por el
 * llamador).
 */
char	*extract_var_name(char *arg)
{
	char	*equal_pos;
	char	*var_name;

	equal_pos = ft_strchr(arg, '=');
	if (equal_pos)
		var_name = ft_substr(arg, 0, equal_pos - arg);
	else
		var_name = ft_strdup(arg);
	return (var_name);
}

/**
 * @brief Muestra las variables de entorno en formato del comando export.
 *
 * Recorre la lista de variables de entorno y las muestra con el formato
 * "declare -x NOMBRE="VALOR"" para variables con valor asignado, o
 * "declare -x NOMBRE" para variables sin valor. Esta función replica el
 * comportamiento del comando export sin argumentos en bash.
 *
 * @param env Puntero a la lista enlazada de variables de entorno.
 */
void	print_env_as_export(t_env *env)
{
	while (env)
	{
		if (env->content)
			printf("declare -x %s=\"%s\"\n", env->name, env->content);
		else
			printf("declare -x %s\n", env->name);
		env = env->next;
	}
}

/**
 * @brief Busca una variable de entorno por su nombre.
 *
 * Recorre la lista de variables de entorno comparando el nombre exacto
 * con el proporcionado. A diferencia de otras funciones similares, esta
 * devuelve un puntero a la estructura completa de la variable, no solo
 * su contenido.
 *
 * @param env Puntero a la lista enlazada de variables de entorno.
 * @param var_name Nombre de la variable a buscar.
 * @return t_env* Puntero a la estructura de la variable si se encuentra,
 *               NULL en caso contrario. No se debe liberar este puntero.
 */
t_env	*find_env_var(t_env *env, char *var_name)
{
	while (env)
	{
		if (ft_strncmp(env->name, var_name, ft_strlen(var_name)) == 0
			&& ft_strlen(env->name) == ft_strlen(var_name))
			return (env);
		env = env->next;
	}
	return (NULL);
}
