# --------------------------------------------------------------------
# Makefile :: NITRO
# Diego Moreno Montes y Matias Blanco Leon
# --------------------------------------------------------------------
EXEC := ransom

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

# Flags de compilación -----------------------------------------------
CXXFLAGS := -I $(DIRHEA) -Wall `pkg-config --cflags OGRE OIS OgreBullet bullet`	\
`sdl-config --cflags` \
-g -D_DEBUG

# Flags del linker ---------------------------------------------------
LDFLAGS := `pkg-config --libs-only-L OGRE OIS OgreBullet bullet`
LDLIBS := `pkg-config --libs-only-l OGRE OIS OgreBullet bullet` -lGL -lstdc++ \
`sdl-config --libs`\
-lSDL -lSDL_mixer \
-lOgreTerrain \
-lConvexDecomposition

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
	rm -rf $(DIROBJ)
	rm -f *.log $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~

edit:
	emacs $(DIRSRC)*.cpp $(DIRHEA)*.h $(DIRHEA)*.hpp &
