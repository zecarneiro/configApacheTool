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

#### CPP FILES ####
# DATA BASE & MAIN & OTHERS
MSNAME = main.cpp
ALLOPERATIONGLOBAL = AllOperationGlobal.cpp
ALLDATABASE = AllDataBase.cpp
CONFIGSITETOOL = ConfigSiteTool.cpp
# SERVERS
ALLAPACHE = AllApache.cpp
ALLNGINX = AllNginx.cpp
# FRAMEWORKS
ALLCAKEPHP = AllCakePHP.cpp

# BIN NAME
NAME = configSite

# DEFINE TARGET HEADER
HEADER = $(LIB)$(INCLUDE_H)

# DEFINE TARGET CPP FILES
# DATA BASE & MAIN & OTHERS
FILE = $(SOURCE)$(MSNAME)
FILE3 = $(SOURCE)$(ALLOPERATIONGLOBAL)
FILE5 = $(SOURCE)$(ALLDATABASE)
FILE4 = $(SOURCE)$(CONFIGSITETOOL)
# SERVERS
FILE1 = $(SOURCE)$(ALLAPACHE)
FILE6 = $(SOURCE)$(ALLNGINX)
# FRAMEWORKS
FILE2 = $(SOURCE)$(ALLCAKEPHP)
ALLFILE = $(FILE1) $(FILE2) $(FILE3) $(FILE4) $(FILE5)

# DEFINE BIN FILE
TARGET = $(BIN)$(NAME)

# OTHER
SUDO = sudo
CREATEBINPATH = $(SUDO) mkdir -p $(BIN)

# subsystem funciona como um main, ou seja, executa todas as linhas de forma
# forma sequencial
# @ = oculta os comandos na suas execuções, resumindo, só são apresentadas as
# linhas que contêm echo
subsystem:
	@$(CREATEBINPATH)
	@$(SUDO) $(CC) $(FLAGS) $(FILE) $(HEADER) $(ALLFILE) -lsqlite3 -o $(TARGET)	# Cria o executavel
	@echo O executavel $(NAME) foi criado com sucesso.