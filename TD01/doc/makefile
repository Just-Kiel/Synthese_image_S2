CC			= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lSDL2 -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers TD 01

# Fichiers exercice 02
OBJ_TD01_EX02= ex02/td01_ex02.o
EXEC_TD01_EX02= td01_ex02.out

# Regles compilation TD 01

all :

ex02 : $(OBJDIR)$(OBJ_TD01_EX02)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_TD01_EX02) -o $(BINDIR)$(EXEC_TD01_EX02) $(LDFLAGS)

clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
