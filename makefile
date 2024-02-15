REP_BIN = bin
REP_SRC = src
REP_OBJ = obj
REP_DOC = doc
NOM_PROG = jeu
REPS = $(REP_BIN) $(REP_SRC) $(REP_OBJ) $(REP_DOC)

SOURCES = $(wildcard $(REP_SRC)/*.c)
OBJETS = $(SOURCES:$(REP_SRC)/%.c=$(REP_OBJ)/%.o)
LIB_FLAGS = `sdl2-config --libs --cflags` -lSDL2_image
SOURCES_DOC = $(wildcard $(REP_DOC)/*.tex)

# vérifie présence de compilateurs TeX
TECTONIC := $(shell command -v tectonic 2> /dev/null)
XELATEX := $(shell command -v xelatex 2> /dev/null)

.PHONY = docs reps clean remove all
.SILENT = reps

$(NOM_PROG) : $(OBJETS)
	gcc -o $(REP_BIN)/$@ $^ $(LIB_FLAGS)
$(OBJETS) : $(REP_OBJ)/%.o: $(REP_SRC)/%.c
	gcc -o $@ -c $< -I$(REP_SRC)
docs : $(SOURCES_DOC)
ifdef XELATEX
	xelatex $<
else ifdef TECTONIC
	tectonic $<
else
	@echo "pas de compilateur TeX trouvé, docs .tex non compilées"
endif
reps :
	mkdir -p $(REPS)

clean :
	rm -f $(REP_OBJ)/*
	rm -f $(REP_DOC)/*pdf
remove : clean
	rm $(REP_BIN)/*

all : reps $(NOM_PROG) docs

exe : all
	$(REP_BIN)/$(NOM_PROG)
