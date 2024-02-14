REP_BIN = bin
REP_SRC = src
REP_OBJ = obj
REP_DOC = doc
NOM_PROG = jeu
SOURCES = $(wildcard $(REP_SRC)/*.c)
OBJETS = $(SOURCES:$(REP_SRC)/%.c=$(REP_OBJ)/%.o)
LIB_FLAGS = `sdl2-config --libs --cflags`
SOURCES_DOC = $(wildcard $(REP_DOC)/*.tex)

$(NOM_PROG) : $(OBJETS)
	gcc -o $(REP_BIN)/$@ $^ $(LIB_FLAGS)
$(OBJETS) : $(REP_OBJ)/%.o: $(REP_SRC)/%.c
	gcc -o $@ -c $< -I$(REP_SRC)
docs : $(SOURCES_DOC)

clean :
	rm $(REP_OBJ)/*
remove : clean
	rm $(REP_BIN)/*

all : $(NOM_PROG) docs
