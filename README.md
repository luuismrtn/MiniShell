# 🖥️ Minishell

**Minishell** es una implementación básica de un shell en lenguaje C, desarrollada como parte del programa 42 Madrid. Este proyecto replica algunas de las funcionalidades esenciales de una terminal UNIX, permitiendo la ejecución de comandos, la gestión de procesos y la manipulación de flujos de entrada y salida.

## 📦 Características Principales

* **Ejecución de Comandos:** Soporte para la ejecución de comandos tanto con rutas relativas como absolutas.
* **Argumentos de Comandos:** Capacidad para pasar múltiples argumentos a los comandos.
* **Pipes (`|`):** Implementación de la redirección de la salida de un comando a la entrada de otro.
* **Redirecciones:**
    * **Entrada (`<`):** Redirección de la entrada estándar desde un archivo.
    * **Salida (`>`):** Redirección de la salida estándar hacia un archivo (truncando el archivo si existe).
    * **Append (`>>`):** Redirección de la salida estándar hacia un archivo (añadiendo al final si existe).
    * **Heredoc (`<<`):** Mecanismo para proporcionar entrada en línea a un comando hasta encontrar un delimitador.
* **Variables de Entorno (`$VAR`):** Expansión de variables de entorno presentes en el sistema.
* **Soporte de Comillas:** Interpretación correcta de cadenas entre comillas simples (`'`) y dobles (`"`).
* **Gestión de Señales:** Manejo de señales comunes como `Ctrl+C` (SIGINT) y `Ctrl+\` (SIGQUIT).
* **Comandos Built-in:** Implementación interna de los siguientes comandos:
    * `cd`: Cambiar el directorio de trabajo actual.
    * `echo`: Mostrar una línea de texto.
    * `env`: Imprimir las variables de entorno.
    * `exit`: Salir del shell.
    * `export`: Establecer o mostrar variables de entorno.
    * `pwd`: Imprimir el directorio de trabajo actual.
    * `unset`: Eliminar una variable de entorno.

## ℹ️ Información del Proyecto

Este proyecto fue realizado para el cursus de 42 Madrid por:

* Aldara Fernandez ([@aldferna](https://github.com/aldferna))
* Luis Martín ([@lumartin](https://github.com/luuismrtn))

**Calificación Final:** 101% 🎉

---
