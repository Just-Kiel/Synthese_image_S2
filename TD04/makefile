CC			= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lSDL2 -lSDL2_image -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers TD 04

# Fichiers exercice 01
OBJ_TD04_EX01= ex01/td04_ex01.o
EXEC_TD04_EX01= td04_ex01.out

# Fichiers exercice 02
OBJ_TD04_EX02= ex02/td04_ex02.o
EXEC_TD04_EX02= td04_ex02.out

# Fichiers exercice 03
OBJ_TD04_EX03= ex03/td04_ex03.o
EXEC_TD04_EX03= td04_ex03.out


# Regles compilation TD 04

all :

ex01 : $(OBJDIR)$(OBJ_TD04_EX01)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_TD04_EX01) -o $(BINDIR)$(EXEC_TD04_EX01) $(LDFLAGS)

ex02 : $(OBJDIR)$(OBJ_TD04_EX02)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_TD04_EX02) -o $(BINDIR)$(EXEC_TD04_EX02) $(LDFLAGS)

ex03 : $(OBJDIR)$(OBJ_TD04_EX03)
	$(CC) $(CFLAGS) $(OBJDIR)$(OBJ_TD04_EX03) -o $(BINDIR)$(EXEC_TD04_EX03) $(LDFLAGS)


clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
