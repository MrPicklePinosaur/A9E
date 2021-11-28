CXX=g++
CXXFLAGS=-std=c++17 -Wall -MMD -g
LIBS=-lncurses
EXEC=age
OBJECTS=age.o renderer.o window.o
DEPENDS=${OBJECTS:.o=.d}

${EXEC}: ${OBJECTS}
	${CXX} ${OBJECTS} -o ${EXEC} ${LIBS}

-include ${DEPENDS}

.PHONY: clean

clean:
	rm ${OBJECTS} ${DEPENDS} ${EXEC}
