# --------------------------------------------------------------------
# Makefile Genérico :: Módulo 2. Curso Experto Desarrollo Videojuegos
# Carlos González Morcillo     Escuela Superior de Informática (UCLM)
# --------------------------------------------------------------------
EXEC := shooter

DIRSRC := src/
DIROBJ := obj/
DIRHEA := include/

DIROGREBULLET_DIN := /usr/include/OgreBullet/Dynamics
DIROGREBULLET_COL := /usr/include/OgreBullet/Collisions
DIROGREBULLET_DIN2 := /usr/local/include/OgreBullet/Dynamics
DIROGREBULLET_COL2 := /usr/local/include/OgreBullet/Collisions
DIRLIBWIIMOTE := /usr/local/include/libcwiimote

CXX := g++

CC=g++
CFLAGS=-c  -Wall -Wno-deprecated-declarations -Wno-unused-variable -I$(DIRHEA)  -I$(DIRLIBWIIMOTE) -I$(DIROGREBULLET_DIN) -I$(DIROGREBULLET_COL) -I$(DIROGREBULLET_DIN2) -I$(DIROGREBULLET_COL2) `pkg-config --cflags OGRE OGRE-Overlay MYGUI mxml bullet `  `sdl-config --cflags` --std=c++11
CPP_FILES := $(wildcard ./src/*.cpp)
OBJ_FILES := $(addprefix ./obj/,$(notdir $(CPP_FILES:.cpp=.o)))
LDLIBS= `pkg-config --libs-only-l OGRE OGRE-Overlay MYGUI mxml bullet` `sdl-config --libs` -lOgreBulletCollisions -lOgreBulletDynamics -lSDL_mixer -lboost_system -lboost_graph -lOIS -lGL -lstdc++   -lMyGUI.OgrePlatform -lbluetooth -lcwiimote


# Modo de compilación (-mode=release -mode=debug) --------------------
ifeq ($(mode), release) 
	CFLAGS += -O2 -D_RELEASE
else 
	CFLAGS += -g -D_DEBUG
	mode := debug
endif



# Obtención automática de la lista de objetos a compilar -------------
OBJS := $(subst $(DIRSRC), $(DIROBJ), \
	$(patsubst %.cpp, %.o, $(wildcard $(DIRSRC)*.cpp)))

.PHONY: all clean

all: info dirs $(EXEC)

info:
	@echo '------------------------------------------------------'
	@echo '>>> Using mode $(mode)'
	@echo '    (Please, call "make" with [mode=debug|release])  '
	@echo '------------------------------------------------------'

dirs:
	mkdir -p $(DIROBJ)

# Enlazado -----------------------------------------------------------
$(EXEC): $(OBJS)
	$(CXX)  -o $@ $^   $(LDLIBS)

# Compilación --------------------------------------------------------

$(DIROBJ)%.o: $(DIRSRC)%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

# Limpieza de temporales ---------------------------------------------
clean:
	rm -f *.log $(EXEC) *~ $(DIRSRC)*~ $(DIRHEA)*~ 
	rm -rf $(DIROBJ)