/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_errors.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/26 20:19:51 by adrianafern       #+#    #+#             */
/*   Updated: 2025/04/04 16:28:50 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Analiza el estado de las comillas en una cadena de texto
 *
 * Esta función recorre una cadena de entrada caracter por caracter,
 * rastreando el estado de las comillas simples y dobles. Mantiene un
 * seguimiento de:
 * 1. Si actualmente estamos dentro de comillas (simples o dobles)
 * 2. El número total de cada tipo de comillas encontradas
 *
 * @param l Cadena de entrada a analizar
 * @param i Índice desde donde comenzar el análisis
 * @param in_quote_s_d Arreglo donde se almacena el estado de las comillas:
 *                     [0] para comillas simples, [1] para comillas dobles
 * @param count_s_d Arreglo donde se cuenta el número de comillas:
 *                  [0] para comillas simples, [1] para comillas dobles
 */
void	check_quotes(char *l, int i, int (*in_quote_s_d)[2],
		int (*count_s_d)[2])
{
	while (l[i])
	{
		if (l[i] == '\\' && l[i + 1] && (l[i + 1] == '\'' || l[i + 1] == '\"'))
		{
			i += 2;
			continue ;
		}
		if (l[i] == '\"' && !(*in_quote_s_d)[0])
		{
			if (!(*in_quote_s_d)[1])
				(*in_quote_s_d)[1] = 1;
			else
				(*in_quote_s_d)[1] = 0;
			(*count_s_d)[1]++;
		}
		else if (l[i] == '\'' && !(*in_quote_s_d)[1])
		{
			if (!(*in_quote_s_d)[0])
				(*in_quote_s_d)[0] = 1;
			else
				(*in_quote_s_d)[0] = 0;
			(*count_s_d)[0]++;
		}
		i++;
	}
}

/**
 * @brief Verifica si todas las comillas en una cadena están correctamente
 * cerradas
 *
 * Esta función analiza una cadena de entrada para determinar si todas las
 * comillas (simples y dobles) están correctamente cerradas. Una comilla está
 * bien cerrada si hay un número par de cada tipo de comilla. Se tienen en
 * cuenta los siguientes casos especiales:
 * - Comillas escapadas con barra invertida (\' o \")
 * - Comillas simples dentro de comillas dobles (que no cuentan para cerrar)
 * - Comillas dobles dentro de comillas simples (que no cuentan para cerrar)
 *
 * @param input Cadena de entrada a verificar
 * @return int SUCCESS (0) si todas las comillas están correctamente cerradas,
 *             ERROR (-1) si algún tipo de comilla está abierto
 */
int	check_quotes_closed(char *input)
{
	int	i;
	int	count_s_d[2];
	int	in_quote_s_d[2];

	i = 0;
	count_s_d[0] = 0;
	count_s_d[1] = 0;
	in_quote_s_d[0] = 0;
	in_quote_s_d[1] = 0;
	check_quotes(input, i, &in_quote_s_d, &count_s_d);
	if ((count_s_d[1] % 2 == 0) && (count_s_d[0] % 2 == 0))
		return (SUCCESS);
	return (ERROR);
}

/**
 * @brief Muestra un mensaje de error para el comando cd.
 *
 * Imprime un mensaje de error estandarizado para el comando cd cuando
 * una ruta no es válida o accesible. También establece el código de salida
 * en 1 para indicar que ocurrió un error.
 *
 * @param path La ruta que causó el error.
 */
void	print_cd_error(char *path, t_token **tokens, char *input_path)
{
	if (input_path)
	{
		ft_putstr_fd("cd: error retrieving current directory: getcwd: ", 2);
		ft_putstr_fd("cannot access parent directories: ", 2);
		ft_putstr_fd("No such file or directory\n", 2);
		modify_pwd(tokens, input_path);
	}
	else
	{
		ft_putstr_fd("cd: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		g_exit_num = 1;
	}
}
