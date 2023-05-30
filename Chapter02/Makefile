OBJ := Math.o Actor.o Component.o AnimSpriteComponent.o TileMapComponent.o BGSpriteComponent.o SpriteComponent.o Ship.o Game.o

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
OPTS_SDL2_INC := -F/Library/Frameworks
OPTS_SDL2_LNK := -framework SDL2 -framework SDL2_image
else
OPTS_SDL2_INC := -I/Library/Frameworks/SDL2.framework/Headers/ -I/Library/Frameworks/SDL2_image.framework/Headers/
OPTS_SDL2_LNK := -lSDL2
endif

OPTS := -std=c++20 -stdlib=libc++ -g -Wall -arch arm64

main: Main.cpp $(OBJ)
	clang++ $(OPTS_SDL2_INC) -o $@ $(OPTS) $(OPTS_SDL2_LNK) $^;

%.o: %.cpp %.h
	clang++ $(OPTS_SDL2_INC) -o $@ -c $< $(OPTS);

clean:
	rm -f *.o;
	rm -rf *.dSYM;
	rm -f main;

info:
	system_profiler SPDisplaysDataType;
