CXX=g++
CXXFLAGS=-std=c++17 -Wall -MMD -g -Ia9e
LIBS=-lncurses -lpthread

A9E_DIR=./a9e
A9E_SRC=$(shell find $(A9E_DIR) -name '*.cc')
A9E_OBJ=${A9E_SRC:.cc=.o}
A9E_DEPS=${A9E_OBJ:.o=.d}

INVADERS_EXEC=invaders-game
INVADERS_DIR=./invaders
INVADERS_SRC=$(shell find $(INVADERS_DIR) -name '*.cc')
INVADERS_OBJ=${INVADERS_SRC:.cc=.o}
INVADERS_DEPS=${INVADERS_OBJ:.o=.d}

${INVADERS_EXEC}: ${A9E_OBJ} ${INVADERS_OBJ}
	${CXX} ${A9E_OBJ} ${INVADERS_OBJ} -o ${INVADERS_EXEC} ${LIBS}

-include ${A9E_DEPS} ${INVADERS_DEPS}

.PHONY: clean

clean-a9e:
	rm -f ${A9E_OBJ} ${A9E_DEPS}

clean-invaders:
	rm -f ${INVADERS_OBJ} ${INVADERS_DEPS} ${INVADERS_EXEC}

clean: clean-a9e clean-invaders
