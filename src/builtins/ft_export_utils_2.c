/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export_utils_2.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/27 01:22:30 by lumartin          #+#    #+#             */
/*   Updated: 2025/04/03 21:40:32 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Procesa y expande variables de entorno
 *
 * Esta función busca una variable de entorno que comienza con '$'
 * en la posición actual de la cadena, encuentra su valor en el entorno
 * y lo añade al resultado acumulado.
 *
 * @param tokens Token que contiene la lista de variables de entorno
 * @param str Cadena que contiene la referencia a la variable
 * @param i Puntero al índice actual en la cadena (se actualiza en la función)
 * @param result Puntero al resultado acumulado (se actualiza en la función)
 */
static void	process_env_var(t_token *tokens, char *str, int *i, char **result)
{
	int		j;
	char	*var_name;
	char	*temp;
	t_env	*env_var;

	j = (*i) + 1;
	while (str[j] && str[j] != ':' && str[j] != '$' && str[j] != '='
		&& str[j] != ' ' && str[j] != '\t')
		j++;
	var_name = ft_substr(str, (*i) + 1, j - (*i) - 1);
	if (!var_name)
		return ;
	env_var = find_env_var(tokens->env_mshell, var_name);
	free(var_name);
	if (!env_var || !env_var->content)
		return ;
	temp = *result;
	*result = ft_strjoin(*result, env_var->content);
	free(temp);
	(*i) = j - 1;
}

/**
 * @brief Añade un único carácter al resultado acumulado
 *
 * Toma un carácter de la cadena de entrada y lo añade al resultado.
 * Gestiona la memoria creando una subcadena temporal y liberando
 * la memoria previa del resultado.
 *
 * @param str Cadena de entrada original
 * @param i Índice del carácter a añadir
 * @param result Puntero al resultado acumulado (se actualiza en la función)
 */
static void	add_char(char *str, int i, char **result)
{
	char	*temp;
	char	*substr;

	temp = *result;
	substr = ft_substr(str, i, 1);
	if (substr)
	{
		*result = ft_strjoin(*result, substr);
		free(substr);
		free(temp);
	}
}

/**
 * @brief Decide cómo procesar cada carácter de la cadena
 *
 * Función de control que decide si un carácter representa una
 * variable de entorno ($VAR) o un carácter normal, y llama a
 * la función apropiada para procesarlo.
 *
 * @param tokens Token que contiene la lista de variables de entorno
 * @param str Cadena de entrada a procesar
 * @param i Puntero al índice actual (se actualiza en la función)
 * @param result Puntero al resultado acumulado (se actualiza en la función)
 */
static void	join_result(t_token *tokens, char *str, int *i, char **result)
{
	if (str[(*i)] == '$')
		process_env_var(tokens, str, i, result);
	else
		add_char(str, (*i), result);
}

/**
 * @brief Expande todas las variables de entorno en una cadena
 *
 * Esta función principal recorre la cadena de entrada y reemplaza
 * todas las referencias a variables de entorno (que comienzan con $)
 * por sus valores correspondientes. Es utilizada por el comando export
 * para manejar casos como "export VAR=$HOME".
 *
 * @param str Cadena original que puede contener referencias a variables
 * @param tokens Token que contiene la lista de variables de entorno
 * @return char* Nueva cadena con todas las variables expandidas
 */
char	*handle_env_var(char *str, t_token *tokens)
{
	int		i;
	char	*result;

	if (!str)
		return (NULL);
	result = ft_strdup("");
	i = 0;
	while (str[i])
	{
		join_result(tokens, str, &i, &result);
		i++;
	}
	return (result);
}

/**
 * @brief Elimina los espacios consecutivos de una cadena
 * 
 * Esta función recorre la cadena de entrada y elimina los espacios
 * consecutivos, dejando solo un espacio entre palabras.
 * 
 * @param str Cadena de entrada a procesar
 * @return char* Nueva cadena con los espacios consecutivos eliminados
 */
char	*remove_consec_spaces(char *str)
{
	int		i;
	int		j;
	char	*result;

	if (!str)
		return (NULL);
	result = malloc(sizeof(char) * (ft_strlen(str) + 1));
	if (!result)
		return (NULL);
	i = 0;
	j = 0;
	while (str[i])
	{
		if ((str[i] == ' ' && str[i + 1] != ' ') || str[i] != ' ')
			result[j++] = str[i];
		i++;
	}
	result[j] = '\0';
	free(str);
	return (result);
}
