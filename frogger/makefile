# --------------------------------------------------------------------
# Makefile :: Frogger
# Diego Moreno Montes y Matias Blanco Leon
# --------------------------------------------------------------------
EXEC := frogger

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall `pkg-config --cflags OGRE OIS`	\
-g -D_DEBUG

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE OIS`
LDLIBS := `pkg-config --libs-only-l OGRE OIS` -lOIS -lGL -lstdc++ -lSDL -lSDL_mixer \
-lOgreTerrain

# Modo de compilación ------------------------------------------------
RELEASE = -O2 -D_RELEASE
DEBUG = -g -D_DEBUG


# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: dirs $(EXEC)

dirs:
	mkdir -p $(DIROBJ)

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# Compilación --------------------------------------------------------
$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(LDLIBS)

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~
	rm -rf $(DIROBJ)
