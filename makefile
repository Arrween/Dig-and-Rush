REP_BIN = bin
REP_SRC = src
REP_OBJ = obj
REP_DOC = doc
REP_LIB = lib
NOM_BIN = dignrush
TEST_BIN = test_unit
NOM_PROG = dignrush.sh
REPS = $(REP_BIN) $(REP_SRC) $(REP_OBJ) $(REP_DOC) $(REP_LIB)

REP_SDL = $(REP_LIB)/SDL2
REP_SDLINC = $(REP_SDL)/include
REP_SDLBIN = $(REP_SDL)/bin

# définition conditionnelle des linker flags et des includes selon le système
SYSTEME = $(shell uname)
ifeq ($(SYSTEME), Linux)
$(info système linux, utilisation d’une librairie SDL locale…)
LIB_FLAGS = `$(REP_SDLBIN)/sdl2-config --libs --cflags` -lSDL2_image -lSDL2_ttf -lSDL2_mixer
INCLUDES = -I$(REP_SRC) -I$(REP_SDLINC)
else
$(info système non linux, utilisation de la librairie SDL système…)
LIB_FLAGS = -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lcunit
INCLUDES = -I$(REP_SRC)
endif

SOURCES = $(wildcard $(REP_SRC)/*.c)
OBJETS = $(SOURCES:$(REP_SRC)/%.c=$(REP_OBJ)/%.o)
ENTETES = $(REP_SRC)/constantes.h
WARNING_FLAGS = -Wall -Wextra # -Wconversion -Wno-float-conversion -Wno-sign-conversion #-fanalyzer -fsanitize=undefined #-fsanitize=address
DEBUG_FLAGS =

DOCS_PDF = $(REP_DOC)/description_détaillée.pdf $(REP_DOC)/rapport.pdf $(REP_DOC)/manuel_installation_et_utilisation.pdf
SOURCES_TEX = $(wildcard $(REP_DOC)/*.tex)
DOXYFILE = $(REP_DOC)/Doxyfile

OUTIL_MESSAGE = outils/bannière.sh

# vérifie présence de compilateurs TeX
TECTONIC := $(shell command -v tectonic 2> /dev/null)
XELATEX := $(shell command -v xelatex 2> /dev/null)

.PHONY = docs docs_tex docs_doxy reps clean remove all
.SILENT = reps

$(NOM_BIN) : $(filter-out $(REP_OBJ)/test_unit.o, $(OBJETS))
	@ $(OUTIL_MESSAGE) Compilation du jeu…
	gcc -o $(REP_BIN)/$@ $^ $(LIB_FLAGS) $(INCLUDES) $(WARNING_FLAGS) $(DEBUG_FLAGS) 

$(TEST_BIN) : $(filter-out $(REP_OBJ)/main.o, $(OBJETS))
	@ $(OUTIL_MESSAGE) Compilation des tests unitaires...
	gcc -o $(REP_BIN)/$@ $^ $(LIB_FLAGS) $(INCLUDES) $(WARNING_FLAGS) $(DEBUG_FLAGS) -lcunit

$(OBJETS) : $(REP_OBJ)/%.o: $(REP_SRC)/%.c $(ENTETES)
	gcc -o $@ -c $< $(WARNING_FLAGS) $(DEBUG_FLAGS) $(INCLUDES)

docs : docs_tex docs_doxy
docs_tex: $(DOCS_PDF)
$(DOCS_PDF): $(REP_DOC)/%.pdf: $(REP_DOC)/%.tex
	@ $(OUTIL_MESSAGE) Compilation des fichiers LaTeX…
ifdef TECTONIC
	@# compilateur TeX alternatif utilisé chez Matthieu
	tectonic $<
else ifdef XELATEX
	@# se déplacer dans doc/ pour compiler doc/*.tex, le doc/ étant retiré par subst
	@# « -interaction batchmode » pour limiter la loquacité de xelatex
	# cd $(REP_DOC) && xelatex -interaction batchmode $(subst $(REP_DOC)/, , $<)
	cd $(REP_DOC) && xelatex $(subst $(REP_DOC)/, , $<) && xelatex $(subst $(REP_DOC)/, , $<)
else
	@echo "pas de compilateur TeX trouvé, docs .tex non compilées"
endif
docs_doxy: $(DOXYFILE)
	@ $(OUTIL_MESSAGE) Génération de la doc Doxygen…
	cd $(REP_DOC) && doxygen $(subst $(REP_DOC)/, , $<)
ifeq ($(shell whoami), s123690)
	rm -r $(HOME)/public_html/doxygen_dignrush
	cp -r $(REP_DOC)/html $(HOME)/public_html/doxygen_dignrush
endif

reps :
	mkdir -p $(REPS)

clean :
	rm -f $(REP_OBJ)/*
	rm -f $(REP_DOC)/*pdf
remove : clean
	rm $(REP_BIN)/$(NOM_BIN)

all : reps $(NOM_BIN) docs

jeu : reps $(NOM_BIN)

test : reps $(TEST_BIN)
	@ $(OUTIL_MESSAGE) Lancement des tests...
	$(REP_BIN)/$(TEST_BIN)

debug : DEBUG_FLAGS += -g
debug : $(NOM_BIN)

exe : jeu
	@ $(OUTIL_MESSAGE) Lancement du jeu…
	$(REP_BIN)/$(NOM_PROG)
