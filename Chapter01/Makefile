OBJ := Component.o ControllerComponent.o Entity.o System.o Registry.o Game.o
LIB_BOX2D_BUILD=../../box2d

OPTS_SDL2_INC := -F/Library/Frameworks -I${LIB_BOX2D_BUILD}/include 
OPTS_SDL2_LNK := -framework SDL2 -L${LIB_BOX2D_BUILD}/build/bin -lbox2d

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
