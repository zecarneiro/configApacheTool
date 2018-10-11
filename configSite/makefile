# NOME: José Manuel Carneiro d'Alva Noronha
# Correr o comando make

CC = g++
FLAGS = -Wall
SOURCE = ./src/
BIN = ./bin/
DEPS = ./lib/ConfigApacheTool.h
MSNAME = main.cpp
CNAME = ConfigApacheTool.cpp
NAME = configApacheTool
FILE1 = $(SOURCE)$(MSNAME)
FILE2 = $(SOURCE)$(CNAME)
TARGET = $(BIN)$(NAME)
SUDO = sudo
CREATEBINPATH = $(SUDO) mkdir -p $(BIN)

# subsystem funciona como um main, ou seja, executa todas as linhas de forma
# forma sequencial
# @ = oculta os comandos na suas execuções, resumindo, só são apresentadas as
# linhas que contêm echo
subsystem:
	@$(CREATEBINPATH)
	@$(SUDO) $(CC) $(FLAGS) $(FILE1) $(DEPS) $(FILE2) -o $(TARGET)	# Cria o executavel
	@echo O executavel $(NAME) foi criado com sucesso.