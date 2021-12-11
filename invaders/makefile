CXX=g++
CXXFLAGS=-std=c++17 -Wall -MMD -g
LIBS=-lncurses -lpthread
EXEC=invaders
OBJECTS=invaders.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${LIBS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
