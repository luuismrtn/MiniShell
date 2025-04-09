/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 00:16:14 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/09 15:02:24 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Añade una nueva variable de entorno a la lista.
 *
 * Crea una nueva estructura t_env con el nombre y contenido proporcionados,
 * y la agrega al inicio de la lista de variables de entorno. Si el contenido
 * comienza con '$', se trata como una referencia a otra variable y se obtiene
 * su valor.
 *
 * @param tokens Puntero a la estructura de tokens con la lista de variables.
 * @param name Nombre de la nueva variable a añadir.
 * @param content Contenido de la variable (o NULL si no tiene valor).
 */
void	add_env_var(t_token *tokens, char *name, char *content)
{
	t_env	*new_env;

	new_env = malloc(sizeof(t_env));
	if (!new_env)
		return ;
	new_env->name = ft_strdup(name);
	if (content || find_env_var(tokens->env_mshell, name))
		handle_add_var(tokens, new_env, remove_consec_spaces(content));
	else
	{
		if (find_env_var(tokens->exp_var, name))
			return ;
		new_env->content = NULL;
		new_env->next = tokens->exp_var;
		tokens->exp_var = new_env;
	}
}

/**
 * @brief Procesa una asignación de variable en el comando export.
 *
 * Maneja argumentos que contienen un signo igual (=), como "VAR=value".
 * Si la variable ya existe, actualiza su valor. Si no existe, crea una nueva.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param arg La cadena de asignación completa (ej: "VAR=value").
 */
static void	process_assignment(t_token *tokens, char *arg)
{
	char	*var_name;
	char	*var_content;
	t_env	*current;

	var_name = ft_substr(arg, 0, ft_strchr(arg, '=') - arg);
	var_content = handle_env_var(ft_strchr(arg, '=') + 1, tokens);
	current = find_env_var(tokens->env_mshell, var_name);
	if (current)
	{
		free(current->content);
		current->content = remove_consec_spaces(var_content);
	}
	else
		add_env_var(tokens, var_name, var_content);
	free(var_name);
}

/**
 * @brief Procesa una declaración de variable sin valor.
 *
 * Maneja argumentos que no contienen signo igual, como "VAR".
 * Si la variable no existe, la crea con valor NULL.
 * Si ya existe, no hace nada (mantiene su valor actual).
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param arg El nombre de la variable a declarar.
 */
static void	process_declaration(t_token *tokens, char *arg)
{
	t_env	*current;

	current = find_env_var(tokens->env_mshell, arg);
	if (!current)
		add_env_var(tokens, arg, NULL);
}

/**
 * @brief Valida los argumentos del comando export.
 *
 * Verifica que todos los argumentos contengan nombres de variables válidos.
 * Muestra mensajes de error para los identificadores no válidos y establece
 * el código de salida adecuado.
 *
 * @param args Array de argumentos del comando.
 * @return int SUCCESS si todos los argumentos son válidos,
 * ERROR en caso contrario.
 */
static int	check_args(char **args)
{
	int		i;
	char	*var_name;
	int		result;

	i = 1;
	result = SUCCESS;
	while (args[i])
	{
		var_name = extract_var_name(args[i]);
		if (!is_valid_var_name(var_name))
		{
			ft_putstr_fd("export: `", 2);
			ft_putstr_fd(args[i], 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			g_exit_num = 1;
			result = ERROR;
		}
		free(var_name);
		i++;
	}
	return (result);
}

/**
 * @brief Implementa el comando export.
 *
 * Sin argumentos, muestra todas las variables de entorno en formato
 * "declare -x".
 * Con argumentos, añade o actualiza variables en el entorno según los
 * siguientes casos:
 * - "VAR=value": Asigna 'value' a la variable 'VAR'
 * - "VAR": Declara 'VAR' sin valor si no existe
 *
 * El comando export sigue estas reglas:
 * 1. Los nombres de variables deben ser válidos (letras, números, guión bajo)
 * 2. Las variables existentes se actualizan con nuevos valores
 * 3. Se pueden declarar variables sin valor
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param args Array de argumentos del comando (args[0] es "export").
 */
void	ft_export(t_token *tokens, char **args)
{
	int		i;
	char	*var_name;

	if (!args[1])
	{
		print_env_as_export(tokens);
		return ;
	}
	if (check_args(args) == ERROR)
		return ;
	i = 1;
	while (args[i])
	{
		var_name = extract_var_name(args[i]);
		if (is_valid_var_name(var_name))
		{
			if (ft_strchr(args[i], '='))
				process_assignment(tokens, args[i]);
			else
				process_declaration(tokens, args[i]);
		}
		free(var_name);
		i++;
	}
}
