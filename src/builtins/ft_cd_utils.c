/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cd_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lumartin <lumartin@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 11:49:21 by lumartin          #+#    #+#             */
/*   Updated: 2025/03/18 12:09:40 by lumartin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

/**
 * @brief Cuenta los niveles de directorio y acumula los niveles a subir.
 *
 * Recorre un array de componentes de ruta y cuenta cuántos niveles de directorio
 * existen. Además, incrementa el contador de niveles a subir por cada ".."
 * encontrado.
 *
 * @param components Array de strings con componentes de una ruta.
 * @param levels_up Puntero a un contador de niveles a subir,
 * aumenta con cada "..".
 * @return int Número total de componentes en la ruta.
 */
static int	count_directory_levels(char **components, int *levels_up)
{
	int	i;

	i = 0;
	while (components && components[i])
	{
		if (ft_strncmp(components[i], "..", 3) == 0)
			(*levels_up)++;
		i++;
	}
	return (i);
}

/**
 * @brief Calcula la ruta deseada después de aplicar cambios relativos.
 *
 * Esta función toma una ruta actual (pwd) y una ruta relativa (dir),
 * y calcula la ruta absoluta resultante después de aplicar la ruta relativa.
 * Es especialmente útil cuando getcwd() falla y necesitamos calcular la ruta
 * programáticamente.
 *
 * @param pwd La ruta actual (directorio de trabajo).
 * @param dir La ruta relativa a aplicar.
 * @return char* La nueva ruta absoluta (debe ser liberada por el llamador).
 */
char	*find_desired_path(char *pwd, char *dir)
{
	char	**components_dir;
	char	**components_pwd;
	char	*result;
	int		levels_up;
	int		i;

	levels_up = 0;
	components_dir = ft_split(dir, '/');
	components_pwd = ft_split(pwd, '/');
	i = count_directory_levels(components_pwd, &levels_up);
	levels_up *= 2;
	i = count_directory_levels(components_dir, &levels_up);
	free_array(components_dir);
	free_array(components_pwd);
	i = ft_strlen(pwd);
	while (i > 0 && levels_up > 0)
	{
		if (pwd[i] == '/')
			levels_up--;
		i--;
	}
	result = ft_substr(pwd, 0, i + 1);
	return (result);
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
void	print_cd_error(char *path)
{
	ft_putstr_fd("cd: ", 2);
	ft_putstr_fd(path, 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	exit_num = 1;
}
