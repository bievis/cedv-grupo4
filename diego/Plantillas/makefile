CXX=g++

ifeq ($(mode), release)
        CXXFLAGS=-O2
else
        CXXFLAGS=-ggdb -Wall -D__DEBUG__
endif

LDFLAGS=

TARGET=Listas

OBJS=main.o
OBJS+=Noticia.o

all: $(TARGET)

# Enlazado -----------------------------------------------------------
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compilación --------------------------------------------------------
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ 

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.o *.~ *~ $(TARGET)
