OBJ := Component.o ControllerComponent.o Entity.o System.o Registry.o Game.o

UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
OPTS_SDL2_INC := -F/Library/Frameworks
OPTS_SDL2_LNK := -framework SDL2
else
OPTS_SDL2_INC := -I/Library/Frameworks/SDL2.framework/Headers/
OPTS_SDL2_LNK := -lSDL2
endif

OPTS := -std=c++20 -stdlib=libc++ -g -Wall -arch arm64

main: Main.cpp $(OBJ)
	clang++ $(OPTS_SDL2_INC) -o $@ $< $(OPTS) $(OPTS_SDL2_LNK) $(OBJ);

%.o: %.cpp %.hpp
	clang++ $(OPTS_SDL2_INC) -o $@ -c $< $(OPTS);

clean:
	rm -f *.o;
	rm -rf *.dSYM;
	rm -f main poc;

info:
	system_profiler SPDisplaysDataType;

poc: poc.cpp
	clang++ $(OPTS_SDL2_INC) -o $@ $< $(OPTS);
