CC		= gcc
CFLAGS	= -Wall -O2 -g
LDFLAGS	= -lSDL -lGLU -lGL -lm

BINDIR	= bin/
SRCDIR	= src/
OBJDIR	= obj/

# Fichiers TD 05

# Fichiers exercice 01a
OBJ_TD05_EX01a= $(OBJDIR)ex01a/td05_ex01a.o $(OBJDIR)fakesdlimage.o $(OBJDIR)lodepng.o
EXEC_TD05_EX01a= td05_ex01a.out

# Fichiers exercice 01b
OBJ_TD05_EX01b= $(OBJDIR)ex01b/td05_ex01b.o $(OBJDIR)fakesdlimage.o $(OBJDIR)lodepng.o
EXEC_TD05_EX01b= td05_ex01b.out

# Fichiers exercice 02
OBJ_TD05_EX02= $(OBJDIR)ex02/td05_ex02.o $(OBJDIR)fakesdlimage.o $(OBJDIR)lodepng.o
EXEC_TD05_EX02= td05_ex02.out


# Regles compilation TD 05

all :

ex01a : $(OBJ_TD05_EX01a)
	$(CC) $(CFLAGS) $(OBJ_TD05_EX01a) -o $(BINDIR)$(EXEC_TD05_EX01a) $(LDFLAGS)

ex01b : $(OBJ_TD05_EX01b)
	$(CC) $(CFLAGS) $(OBJ_TD05_EX01b) -o $(BINDIR)$(EXEC_TD05_EX01b) $(LDFLAGS)

ex02 : $(OBJ_TD05_EX02)
	$(CC) $(CFLAGS) $(OBJ_TD05_EX02) -o $(BINDIR)$(EXEC_TD05_EX02) $(LDFLAGS)


clean :
	rm -rf *~
	rm -rf $(SRCDIR)*/*~
	rm -rf $(OBJDIR)
	rm -rf $(BINDIR)*

$(OBJDIR)%.o: $(SRCDIR)%.c
	mkdir -p `dirname $@`
	$(CC) -o $@ -c $< $(CFLAGS)
