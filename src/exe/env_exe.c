/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_exe.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/24 23:26:19 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 16:24:37 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Crea una cadena con formato para una variable de entorno.
 *
 * Concatena el nombre y el contenido de una variable de entorno
 * en el formato "NOMBRE=VALOR".
 *
 * @param env_var Estructura que contiene nombre y valor.
 * @return char* Cadena formateada (debe liberarse después).
 */
static char	*create_env_string(t_env *env_var)
{
	char	*result;
	char	*temp;

	if (env_var->content == NULL)
		return (ft_strjoin(env_var->name, ""));
	temp = ft_strjoin(env_var->name, "=");
	if (!temp)
		return (NULL);
	result = ft_strjoin(temp, env_var->content);
	free(temp);
	return (result);
}

/**
 * @brief Obtiene la variable PATH del entorno.
 *
 * Busca en el array de variables de entorno hasta encontrar
 * la variable PATH y devuelve su valor (saltando "PATH=").
 * En el caso de que PATH no exista, enviamos el valor de PWD
 *
 * @param env Array de strings de variables de entorno.
 * @return char* Puntero al valor de PATH (no debe liberarse).
 */
char	*get_path_from_env(char **env)
{
	int		i;
	t_env	*env_s;
	t_env	*pwd;
	char	*path;

	i = 0;
	while (env[i] != NULL && ft_strncmp(env[i], "PATH=", 5) != 0)
		i++;
	if (env[i] == NULL)
	{
		env_s = env_buildin(env);
		pwd = find_env_var(env_s, "PWD");
		path = ft_strdup(pwd->content);
		free_env_list(env_s);
		return (path);
	}
	return (ft_strdup(env[i] + 5));
}

/**
 * @brief Cuenta el número de variables en la lista de entorno.
 *
 * Recorre la lista enlazada de variables de entorno y cuenta
 * el número de nodos.
 *
 * @param env_list Lista enlazada de variables de entorno.
 * @return int Número de variables.
 */
int	count_env_vars(t_env *env_list)
{
	int		count;
	t_env	*current;

	count = 0;
	current = env_list;
	while (current)
	{
		count++;
		current = current->next;
	}
	return (count);
}

/**
 * @brief Libera un array de cadenas de entorno.
 *
 * Libera la memoria ocupada por un array de cadenas
 * de variables de entorno.
 *
 * @param env Array de cadenas a liberar.
 * @param count Número de elementos a liberar.
 */
static void	free_env_strings(char **env, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(env[i]);
		i++;
	}
	free(env);
}

/**
 * @brief Convierte la lista de variables de entorno en un array de strings.
 *
 * Transforma la estructura de lista enlazada de variables de entorno
 * en un array terminado en NULL que puede usarse en execve().
 *
 * @param env_list Lista enlazada de variables de entorno.
 * @return char** Array de cadenas en formato "NOMBRE=VALOR" (debe liberarse).
 */
char	**join_env(t_env *env_list)
{
	char	**env;
	t_env	*current;
	int		count;
	int		i;

	count = count_env_vars(env_list);
	env = malloc(sizeof(char *) * (count + 1));
	if (!env)
		return (NULL);
	current = env_list;
	i = 0;
	while (current)
	{
		env[i] = create_env_string(current);
		if (!env[i])
		{
			free_env_strings(env, i);
			return (NULL);
		}
		current = current->next;
		i++;
	}
	env[i] = NULL;
	return (env);
}
