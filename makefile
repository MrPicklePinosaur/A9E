CXX=g++
CXXFLAGS=-std=c++17 -Wall -MMD -g
LIBS=-lncurses -lpthread
EXEC=age
OBJECTS=age.o renderer.o input.o window.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${LIBS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
