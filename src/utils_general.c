/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_general.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 14:58:28 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/03 21:57:32 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Cuenta el número de pipes en una lista de tokens
 *
 * Esta función recorre la lista de tokens y cuenta cuántos separadores de
 * tipo pipe (|) contiene.
 *
 * @param tokens Puntero al primer token de la lista (token de cabecera)
 * @return int Número de pipes encontrados en la lista de tokens
 */
int	num_pipes(t_token *tokens)
{
	t_token	*aux;
	int		num;

	aux = tokens->next;
	if (aux == NULL)
		return (0);
	num = 0;
	while (aux != NULL)
	{
		if (aux->type == T_PIPE)
			num++;
		aux = aux->next;
	}
	return (num);
}

/**
 * @brief Calcula la longitud de un nombre de variable
 *
 * Esta función mide cuántos caracteres forman parte de un nombre de variable
 * a partir de la posición indicada. Un nombre de variable consiste en letras,
 * números y guiones bajos.
 *
 * @param str Cadena que contiene el nombre de la variable
 * @param i Índice inicial desde donde empezar a contar
 * @return int Longitud del nombre de la variable
 */
int	ft_len_var_name(char *str, int i)
{
	int	count;

	count = 0;
	while (str[i] && (ft_isalnum(str[i]) || str[i] == '_'))
	{
		count++;
		i++;
	}
	return (count);
}

/**
 * @brief Calcula la longitud de un array de strings
 *
 * Esta función cuenta el número de elementos en un array de strings
 * terminado en NULL.
 *
 * @param array Array de strings terminado en NULL
 * @return int Número de strings en el array (sin contar el NULL final)
 */
int	len_array(char **array)
{
	int	count;

	count = 0;
	while (array[count])
		count++;
	return (count);
}

/**
 * @brief Libera la memoria de un array de strings
 *
 * Esta función libera la memoria asignada para cada string del array
 * y posteriormente el array mismo. Maneja correctamente el caso de que
 * el array sea NULL.
 *
 * @param array Array de strings terminado en NULL a liberar
 */
void	free_array(char **array)
{
	int	i;

	i = 0;
	if (!array)
		return ;
	while (array[i])
	{
		free(array[i]);
		i++;
	}
	free(array);
}

/**
 * @brief Compara si dos strings son exactamente iguales
 *
 * Esta función verifica si dos strings son idénticas comparando
 * tanto su contenido como su longitud. A diferencia de ft_strncmp,
 * asegura que ambas strings tengan exactamente el mismo tamaño.
 *
 * @param str1 Primera string a comparar
 * @param str2 Segunda string a comparar
 * @return int 1 si las strings son iguales, 0 si son diferentes
 */
int	match_string(char *str1, char *str2)
{
	return (ft_strncmp(str1, str2, ft_strlen(str2)) == 0
		&& ft_strlen(str1) == ft_strlen(str2));
}
