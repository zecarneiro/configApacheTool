# NOME: José Manuel Carneiro d'Alva Noronha
# Correr o comando make

CC = g++
FLAGS = -Wall

# PATHS
SOURCE = ./src/
LIB = ./lib/
BIN = ./bin/

# HEADERS
INCLUDE_H = includes.h

# CPP FILES
MSNAME = main.cpp
ALLAPACHE = AllApache.cpp
ALLCAKEPHP = AllCakePHP.cpp
ALLCOMMANDS = AllCommands.cpp
CONFIGSITETOOL = ConfigSiteTool.cpp

# BIN NAME
NAME = ConfigSiteTool

# DEFINE TARGET HEADER
HEADER = $(LIB)$(INCLUDE_H)

# DEFINE TARGET CPP FILES
FILE = $(SOURCE)$(MSNAME)
FILE1 = $(SOURCE)$(ALLAPACHE)
FILE2 = $(SOURCE)$(ALLCAKEPHP)
FILE3 = $(SOURCE)$(ALLCOMMANDS)
FILE4 = $(SOURCE)$(CONFIGSITETOOL)
ALLFILE = $(FILE1) $(FILE2) $(FILE3) $(FILE4)

# DEFINE BIN FILE
TARGET = $(BIN)$(NAME)

SUDO = sudo
CREATEBINPATH = $(SUDO) mkdir -p $(BIN)

# subsystem funciona como um main, ou seja, executa todas as linhas de forma
# forma sequencial
# @ = oculta os comandos na suas execuções, resumindo, só são apresentadas as
# linhas que contêm echo
subsystem:
	@$(CREATEBINPATH)
	@$(SUDO) $(CC) $(FLAGS) $(FILE) $(HEADER) $(ALLFILE) -o $(TARGET)	# Cria o executavel
	@echo O executavel $(NAME) foi criado com sucesso.