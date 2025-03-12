NAME = minishell
CC = cc
CFLAGS = -Wall -Wextra -Werror  -g3 #-fsanitize=address
RM = rm -f

SRC = src/main.c \
	src/pipex.c \
	src/history.c \
	src/tokenize.c \
	src/env_build_in.c \
	src/automata.c \
	src/exe.c \
	src/builtins/builtins.c \
	src/builtins/ft_cd.c \
	src/builtins/ft_echo.c \
	src/builtins/ft_env.c \
	src/builtins/ft_exit.c \
	src/builtins/ft_export.c \
	src/builtins/ft_pwd.c \
	src/builtins/ft_unset.c \

INCLUDE = -I libft -I.

LIB = libft/libft.a

OBJ = $(SRC:.c=.o)

all: $(NAME)

$(NAME): $(OBJ)
	make -C libft
	$(CC) $(CFLAGS) $(OBJ) $(LIB) -lreadline -o $(NAME)

clean:
	$(RM) $(OBJ)
	make clean -C libft

fclean: clean
	$(RM) $(NAME)
	make fclean -C libft

re: fclean all

.PHONY: all clean fclean re


# LEAKS
# quotes not closed -> leak + espacio final en clean_tokens
# leaks con: hola... "no such file or directory"
# con: ñ
# despues de haber ejecutado algo, si se hace control d
# al ejecutar cada builtin
# leaks con: echo hola
# echo $?

# TODOLIST
# revisar si dejamos fd's abiertos
# echo $$ da un num
# el heredoc (+ meterlo tb en el pipex)
# si solo 'export' imprimir las variables exportadas
#bash-3.2$ echo < 1 | echo < 2 | echo < 3
#    bash: echo: write error: Broken pipe //meter un if en ecchooo??

# PIPES:

# env | sort | grep -v SHLVL | grep -v ^_ 
# nuestro output = ()
# bash output = (COLORTERM=truecolor DBUS_SESSION_BUS_ADDRESS=unix:path=/run/user/1000/bus DISPLAY=:0 GIT_ASKPASS=/home/luis-la/.vscode-server/bin/2fc07b811f760549dab9be9d2bedd06c51dfcb9a/extensions/git/dist/askpass.sh HOME=/home/luis-la HOSTTYPE=x86_64 LANG=C.UTF-8 LESSCLOSE=/usr/bin/lesspipe %s %s LESSOPEN=| /usr/bin/lesspipe %s LOGNAME=luis-la LS_COLORS=rs=0:di=01;34:ln=01;36:mh=00:pi=40;33:so=01;35:do=01;35:bd=40;33;01:cd=40;33;01:or=40;31;01:mi=00:su=37;41:sg=30;43:ca=30;41:tw=30;42:ow=34;42:st=37;44:ex=01;32:*.tar=01;31:*.tgz=01;31:*.arc=01;31:*.arj=01;31:*.taz=01;31:*.lha=01;31:*.lz4=01;31:*.lzh=01;31:*.lzma=01;31:*.tlz=01;31:*.txz=01;31:*.tzo=01;31:*.t7z=01;31:*.zip=01;31:*.z=01;31:*.dz=01;31:*.gz=01;31:*.lrz=01;31:*.lz=01;31:*.lzo=01;31:*.xz=01;31:*.zst=01;31:*.tzst=01;31:*.bz2=01;31:*.bz=01;31:*.tbz=01;31:*.tbz2=01;31:*.tz=01;31:*.deb=01;31:*.rpm=01;31:*.jar=01;31:*.war=01;31:*.ear=01;31:*.sar=01;31:*.rar=01;31:*.alz=01;31:*.ace=01;31:*.zoo=01;31:*.cpio=01;31:*.7z=01;31:*.rz=01;31:*.cab=01;31:*.wim=01;31:*.swm=01;31:*.dwm=01;31:*.esd=01;31:*.jpg=01;35:*.jpeg=01;35:*.mjpg=01;35:*.mjpeg=01;35:*.gif=01;35:*.bmp=01;35:*.pbm=01;35:*.pgm=01;35:*.ppm=01;35:*.tga=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:*.tiff=01;35:*.png=01;35:*.svg=01;35:*.svgz=01;35:*.mng=01;35:*.pcx=01;35:*.mov=01;35:*.mpg=01;35:*.mpeg=01;35:*.m2v=01;35:*.mkv=01;35:*.webm=01;35:*.webp=01;35:*.ogm=01;35:*.mp4=01;35:*.m4v=01;35:*.mp4v=01;35:*.vob=01;35:*.qt=01;35:*.nuv=01;35:*.wmv=01;35:*.asf=01;35:*.rm=01;35:*.rmvb=01;35:*.flc=01;35:*.avi=01;35:*.fli=01;35:*.flv=01;35:*.gl=01;35:*.dl=01;35:*.xcf=01;35:*.xwd=01;35:*.yuv=01;35:*.cgm=01;35:*.emf=01;35:*.ogv=01;35:*.ogx=01;35:*.aac=00;36:*.au=00;36:*.flac=00;36:*.m4a=00;36:*.mid=00;36:*.midi=00;36:*.mka=00;36:*.mp3=00;36:*.mpc=00;36:*.ogg=00;36:*.ra=00;36:*.wav=00;36:*.oga=00;36:*.opus=00;36:*.spx=00;36:*.xspf=00;36: MOTD_SHOWN=update-motd NAME=Code OLDPWD=/home/luis-la/MiniShell PATH=/home/luis-la/.vscode-server/bin/2fc07b811f760549dab9be9d2bedd06c51dfcb9a/bin/remote-cli:/home/luis-la/.local/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin:/usr/games:/usr/local/games:/usr/lib/wsl/lib:/mnt/c/Python313/Scripts/:/mnt/c/Python313/:/mnt/c/Program Files/Java/jdk-19/bin:/mnt/c/Windows/System32:/mnt/c/Windows/System32/WindowsPowerShell/v1.0/powershell.exe:/mnt/c/WINDOWS/System32/WindowsPowerShell/v1.0/:/mnt/c/Program Files/Docker/Docker/resources/bin:/mnt/c/Program Files/Git LFS:/mnt/c/WINDOWS/system32:/mnt/c/WINDOWS:/mnt/c/WINDOWS/System32/Wbem:/mnt/c/WINDOWS/System32/OpenSSH/:/mnt/c/Program Files/Git/cmd:/mnt/c/Program Files/Microsoft SQL Server/Client SDK/ODBC/170/Tools/Binn/:/mnt/c/Program Files/Microsoft SQL Server/150/Tools/Binn/:/mnt/c/Program Files/dotnet/:/mnt/c/ProgramData/chocolatey/bin:/mnt/c/Program Files/nodejs/:/mnt/c/Users/luism/AppData/Local/Android/Sdk/platform-tools:/mnt/c/Users/luism/AppData/Local/Android/Sdk/tools:/mnt/c/Users/luism/AppData/Local/pnpm:/mnt/c/Windows/System32/WindowsPowerShell/v1.0/PowerShell.exe:/mnt/c/Windows/System32/WindowsPowerShell/v1.0/PowerShell:/mnt/c/Users/luism/AppData/Local/spicetify:/mnt/c/Users/luism/AppData/Local/Programs/Microsoft VS Code/bin:/mnt/c/Users/luism/Desktop/Flutter/flutter/bin:/mnt/c/Users/luism/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/luism/.dotnet/tools:/mnt/c/Users/luism/AppData/Local/Microsoft/WindowsApps:/mnt/c/Users/luism/AppData/Roaming/npm:/snap/bin:/home/luis-la/.vscode-server/data/User/globalStorage/github.copilot-chat/debugCommand PULSE_SERVER=unix:/mnt/wslg/PulseServer PWD=/home/luis-la/MiniShell/minishell_tester SHELL=/bin/bash TERM=xterm-256color TERM_PROGRAM=vscode TERM_PROGRAM_VERSION=1.98.1 USER=luis-la VSCODE_GIT_ASKPASS_EXTRA_ARGS= VSCODE_GIT_ASKPASS_MAIN=/home/luis-la/.vscode-server/bin/2fc07b811f760549dab9be9d2bedd06c51dfcb9a/extensions/git/dist/askpass-main.js VSCODE_GIT_ASKPASS_NODE=/home/luis-la/.vscode-server/bin/2fc07b811f760549dab9be9d2bedd06c51dfcb9a/node VSCODE_GIT_IPC_HANDLE=/run/user/1000/vscode-git-3d89716c00.sock VSCODE_IPC_HOOK_CLI=/run/user/1000/vscode-ipc-f1da9724-16d2-4a25-98e5-d500071e5b0c.sock WAYLAND_DISPLAY=wayland-0 WSL2_GUI_APPS_ENABLED=1 WSLENV=ELECTRON_RUN_AS_NODE/w: WSL_DISTRO_NAME=Ubuntu WSL_INTEROP=/run/WSL/676_interop XDG_DATA_DIRS=/usr/local/share:/usr/share:/var/lib/snapd/desktop XDG_RUNTIME_DIR=/run/user/1000/)
# mini exit code = 139
# bash exit code = 0
# mini error = ( Broken pipe)
# bash error = ()

# cat minishell.h | grep ");"$ 
# mini exit code = 0
# bash exit code = 1

