EXEC := triangulos

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

CXX := g++

CXXFLAGS := -Wall -I./include

LDFLAGS := -lstdc++

CXXFLAGS += -g 
#CXXFLAGS += -g -D__DEBUG__

# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: $(EXEC)

$(EXEC): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(EXEC) *~ $(DIROBJ)* $(DIRSRC)*~ $(DIRHEA)*~
