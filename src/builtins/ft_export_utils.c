/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:59:37 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/20 21:21:30 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Elimina una variable de entorno de la lista.
 *
 * Busca una variable por su nombre y la elimina de la lista de variables de
 * entorno. Libera la memoria de la estructura y sus campos.
 *
 * @param env Puntero al puntero de la lista enlazada de variables de entorno.
 * @param name Nombre de la variable a eliminar.
 */
static void	remove_var(t_env **env, char *name)
{
	t_env	*current;
	t_env	*prev;

	current = *env;
	prev = NULL;
	while (current)
	{
		if (ft_strcmp(current->name, name) == 0)
		{
			if (prev)
				prev->next = current->next;
			else
				*env = current->next;
			free(current->name);
			free(current->content);
			free(current);
			return ;
		}
		prev = current;
		current = current->next;
	}
}

/**
 * @brief Añade una variable con contenido el env_mshell.
 *
 * Crea una nueva estructura t_env con el nombre y contenido proporcionados,
 * y la agrega al inicio de la lista de variables de entorno.
 *
 * @param tokens Puntero a la estructura de tokens con la lista de variables.
 * @param new_env Estructura con el nombre y contenido de la variable.
 * @param content Contenido de la variable (nunca es NULL).
 */
void	handle_add_var(t_token *tokens, t_env *new_env, char *content)
{
	char	*to_search;
	t_env	*current;

	if (content[0] == '$')
	{
		to_search = ft_substr(content, 1, ft_strlen(content) - 1);
		current = find_env_var(tokens->env_mshell, to_search);
		free(to_search);
		new_env->content = ft_strdup(current->content);
	}
	else
		new_env->content = ft_strdup(content);
	new_env->next = tokens->env_mshell;
	tokens->env_mshell = new_env;
	if (find_env_var(tokens->exp_var, new_env->name))
		remove_var(&tokens->exp_var, new_env->name);
}

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
void	print_env_as_export(t_token *tokens)
{
	t_env	*env;
	t_env	*vars;

	vars = tokens->exp_var;
	env = tokens->env_mshell;
	while (env)
	{
		printf("declare -x %s=\"%s\"\n", env->name, env->content);
		env = env->next;
	}
	while (vars)
	{
		printf("declare -x %s\n", vars->name);
		vars = vars->next;
	}
}
