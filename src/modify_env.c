/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_env.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/25 15:04:33 by aldferna          #+#    #+#             */
/*   Updated: 2025/04/04 14:00:35 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/**
 * @brief Añade una variable de entorno a la lista
 *
 * Esta función crea un nodo para una variable de entorno a partir
 * de una línea en formato "NAME=VALUE" y la añade a la lista existente.
 * Si hay algún error durante la creación del nodo, libera toda la lista.
 *
 * @param env_mshell Puntero al puntero de la lista de entorno
 * @param line Línea que contiene la variable en formato "NAME=VALUE"
 * @return int SUCCESS si se añadió correctamente, ERROR en caso contrario
 */
static int	add_env_var_line(t_env **env_mshell, char *line)
{
	t_env	*node;

	node = create_env_node(line);
	if (!node)
	{
		free_env_list(*env_mshell);
		*env_mshell = NULL;
		return (ERROR);
	}
	add_last(env_mshell, node);
	return (SUCCESS);
}

/**
 * @brief Crea un entorno mínimo para la shell
 *
 * Esta función genera un entorno básico con las variables esenciales
 * para el funcionamiento de la shell cuando no hay entorno heredado:
 * - PATH desde /etc/environment
 * - PWD con el directorio de trabajo actual
 * - SHLVL con valor 1
 *
 * @return t_env* Lista de variables de entorno o NULL si hay error
 */
t_env	*make_little_env(void)
{
	t_env	*env_mshell;
	int		fd;
	char	*line;
	char	*pwd;
	char	*shlvl_str;

	env_mshell = NULL;
	fd = open("/etc/environment", O_RDONLY);
	line = get_next_line(fd);
	close(fd);
	if (add_env_var_line(&env_mshell, line) == ERROR)
		return (free(line), NULL);
	free(line);
	pwd = getcwd(NULL, 0);
	line = ft_strjoin("PWD=", pwd);
	if (add_env_var_line(&env_mshell, line) == ERROR)
		return (free(pwd), free(line), NULL);
	free(line);
	shlvl_str = ft_itoa(1);
	line = ft_strjoin("SHLVL=", shlvl_str);
	free(shlvl_str);
	if (add_env_var_line(&env_mshell, line) == ERROR)
		return (free(pwd), free(line), NULL);
	return (free(pwd), free(line), env_mshell);
}

/**
 * @brief Modifica o crea la variable SHLVL en el entorno
 *
 * Esta función se encarga de incrementar el nivel de shell (SHLVL)
 * cada vez que se inicia una nueva instancia de la shell. Si la variable
 * ya existe en el entorno, incrementa su valor en 1. Si no existe,
 * la crea con valor inicial 1.
 *
 * El nivel de shell (SHLVL) indica la profundidad de anidamiento de shells.
 * Por ejemplo, si ejecutas bash dentro de bash, SHLVL será 2.
 *
 * @param tokens Doble puntero al token que contiene la lista del entorno
 * @param var Nombre de la variable a modificar (normalmente "SHLVL")
 */
void	modify_shlvl(t_token **tokens, char *var)
{
	t_env	*aux;
	t_env	*new_var;
	int		shlvl;

	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (strncmp(aux->name, var, strlen(var)) == 0)
		{
			shlvl = ft_atoi(aux->content) + 1;
			free(aux->content);
			aux->content = ft_itoa(shlvl);
			return ;
		}
		aux = aux->next;
	}
	shlvl = 1;
	new_var = malloc(sizeof(t_env));
	if (!new_var)
		return ;
	new_var->name = ft_strdup(var);
	new_var->content = ft_itoa(shlvl);
	new_var->next = (*tokens)->env_mshell;
	(*tokens)->env_mshell = new_var;
}
