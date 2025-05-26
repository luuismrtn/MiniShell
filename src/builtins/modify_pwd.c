/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modify_pwd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: aldara <aldara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:44:38 by lumartin          #+#    #+#             */
/*   Updated: 2025/05/26 15:49:49 by aldara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/* 
 * @brief Actualiza la variable PWD cuando el directorio es ".".
 *
 * Maneja el caso especial cuando cambiamos al directorio actual (".").
 * Intenta obtener la ruta con getcwd(). Si falla (directorio inaccesible),
 * construye la ruta basándose en el valor actual de PWD.
 *
 * @param aux Puntero a la variable PWD en el entorno.
 * @param dir El directorio a añadir (en este caso ".").
 */
static void	update_pwd_dot(t_env *aux, char *dir)
{
	char	*new_pwd;
	char	*temp;
	char	*cwd;

	cwd = getcwd(NULL, 0);
	if (cwd == NULL)
	{
		temp = ft_strjoin("/", dir);
		new_pwd = ft_strjoin(aux->content, temp);
		free(temp);
		free(aux->content);
		aux->content = new_pwd;
		return ;
	}
	new_pwd = ft_strdup(cwd);
	free(aux->content);
	free(cwd);
	aux->content = new_pwd;
}

/**
 * @brief Actualiza la variable PWD cuando el directorio es HOME o derivado.
 *
 * Maneja específicamente el caso cuando cambiamos al directorio HOME o a un
 * subdirectorio directo de HOME. En lugar de construir la ruta,
 * usa directamente
 * el valor de HOME o la ruta completa proporcionada.
 *
 * @param aux Puntero a la variable PWD en el entorno.
 * @param dir El directorio al que se cambió.
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
static void	update_pwd_home(t_env *aux, char *dir, t_token **tokens)
{
	char	*new_pwd;
	char	*home_value;

	home_value = find_env_var((*tokens)->env_mshell, "HOME")->content;
	if (ft_strlen(dir) == ft_strlen(home_value))
		new_pwd = ft_strdup(home_value);
	else
		new_pwd = ft_strdup(dir);
	free(aux->content);
	aux->content = new_pwd;
}

/**
 * @brief Actualiza la variable PWD para casos generales.
 *
 * Maneja el caso general cuando cambiamos a un directorio cualquiera
 * (que no es ni "." ni HOME). Construye la nueva ruta añadiendo el directorio
 * al valor actual de PWD.
 *
 * @param aux Puntero a la variable PWD en el entorno.
 * @param dir El directorio al que se cambió.
 */
 static void	update_pwd_other(t_env *aux, char *dir)
{
	char	*path;
	char	*new_pwd;

	if (aux->content[ft_strlen(aux->content) - 1] == '/')
		path = ft_strdup(dir);
	else
		path = ft_strjoin("/", dir);
	if (dir[0] == '/')
		new_pwd = ft_strdup(dir);
	else
 	new_pwd = ft_strjoin(aux->content, path);
	free(path);
	free(aux->content);
	aux->content = new_pwd;
}

/**
 * @brief Modifica la variable OLDPWD en el entorno.
 *
 * Esta función se llama antes de modificar la variable PWD para actualizar
 * la variable OLDPWD. Busca la variable PWD en el entorno y actualiza
 * OLDPWD con su valor.
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 */
static void	modify_oldpwd(t_token *tokens)
{
	t_env	*aux;
	t_env	*oldpwd;
	char	*a_pwd;

	a_pwd = NULL;
	aux = tokens->env_mshell;
	while (aux)
	{
		if (ft_strncmp(aux->name, "PWD", ft_strlen("PWD")) == 0)
		{
			a_pwd = ft_strdup(aux->content);
			break ;
		}
		aux = aux->next;
	}
	oldpwd = find_env_var(tokens->env_mshell, "OLDPWD");
	if (oldpwd)
	{
		free(oldpwd->content);
		oldpwd->content = a_pwd;
	}
	else
		add_env_var(tokens, "OLDPWD", a_pwd);
}
/**
 * @brief Actualiza la variable PWD después de un cambio de directorio.
 *
 * Esta función principal examina el tipo de directorio al que se cambió
 * y llama a la función especializada correspondiente para actualizar
 * la variable PWD. Si la variable no existe en el entorno, la crea.
 *
 * Casos especiales:
 * - Si dir es exactamente ".", no se hace nada
 * - Si dir empieza con ".", se usa update_pwd_dot
 * - Si dir coincide con HOME o comienza por HOME, se usa update_pwd_home
 * - Para cualquier otro caso, se usa update_pwd_other
 *
 * @param tokens Puntero a la estructura de tokens con variables de entorno.
 * @param dir El directorio al que se cambió.
 */

void	modify_pwd_error(t_token **tokens, char *dir)
{
	t_env	*aux;

	modify_oldpwd((*tokens));
	if (ft_strncmp(dir, ".", 2) == 0)
		return ;
	aux = (*tokens)->env_mshell;
	while (aux)
	{
		if (ft_strncmp(aux->name, "PWD", ft_strlen("PWD")) == 0)
		{
			if (ft_strncmp(dir, ".", 1) == 0)
				update_pwd_dot(aux, dir);
			else if (find_env_var((*tokens)->env_mshell, "HOME")
				&& !ft_strncmp(dir, find_env_var((*tokens)->env_mshell,
						"HOME")->content, 5))
				update_pwd_home(aux, dir, tokens);
			else if (match_string(dir, "/") || ft_strncmp(dir, "//", 3) == 0)
				update_pwd_home(aux, dir, tokens);
			else
				update_pwd_other(aux, dir);
			return ;
		}
		aux = aux->next;
	}
	create_new_pwd(tokens, dir);
}

void modify_pwd(t_token **tokens, char *dir)
{
    t_env *pwd_var;
    char *cwd;

    modify_oldpwd((*tokens));
    cwd = getcwd(NULL, 0);
    if (!cwd)
	{
		modify_pwd_error(tokens, dir);
        return;
	}
    pwd_var = find_env_var((*tokens)->env_mshell, "PWD");
    if (pwd_var)
    {
        free(pwd_var->content);
        pwd_var->content = ft_strdup(cwd);
    }
    else
        add_env_var((*tokens), "PWD", ft_strdup(cwd));
    free(cwd);
}
