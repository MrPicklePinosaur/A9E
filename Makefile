CXX=g++
CXXFLAGS=-std=c++17 -Wall -MMD -fPIC -g -Ia9e
LIBS=-lncurses -lpthread

INSTALL_LIB_PATH=/usr/lib
INSTALL_INC_PATH=/usr/include/a9e

# don't include ./ in front
A9E_DIR=a9e
A9E_SO=liba9e.so
A9E_SRC=$(shell find $(A9E_DIR) -name '*.cc')
A9E_OBJ=${A9E_SRC:.cc=.o}
A9E_DEPS=${A9E_OBJ:.o=.d}

INVADERS_EXEC=invaders-game
INVADERS_DIR=invaders
INVADERS_SRC=$(shell find $(INVADERS_DIR) -name '*.cc')
INVADERS_OBJ=${INVADERS_SRC:.cc=.o}
INVADERS_DEPS=${INVADERS_OBJ:.o=.d}

BIRB_EXEC=birb-game
BIRB_DIR=birb
BIRB_SRC=$(shell find $(BIRB_DIR) -name '*.cc')
BIRB_OBJ=${BIRB_SRC:.cc=.o}
BIRB_DEPS=${BIRB_OBJ:.o=.d}

.PHONY: clean-a9e clean-invaders clean-birb clean install uninstall

all: ${A9E_SO} ${INVADERS_EXEC} ${BIRB_EXEC}

${A9E_SO}: ${A9E_OBJ}
	${CXX} $^ -shared -o $@ ${LIBS}

-include ${A9E_DEPS}

${INVADERS_EXEC}: ${A9E_OBJ} ${INVADERS_OBJ}
	${CXX} $^ -o $@ ${LIBS}

-include ${A9E_DEPS} ${INVADERS_DEPS}

${BIRB_EXEC}: ${A9E_OBJ} ${BIRB_OBJ}
	${CXX} $^ -o $@ ${LIBS}

-include ${A9E_DEPS} ${BIRB_DEPS}

clean-a9e:
	rm -f ${A9E_OBJ} ${A9E_DEPS} ${A9E_SO}

clean-invaders:
	rm -f ${INVADERS_OBJ} ${INVADERS_DEPS} ${INVADERS_EXEC}

clean-birb:
	rm -f ${BIRB_OBJ} ${BIRB_DEPS} ${BIRB_EXEC}

clean: clean-a9e clean-invaders clean-birb

.ONEHSELL:
install: ${A9E_SO}
	cp ${A9E_SO} ${INSTALL_LIB_PATH}
	mkdir -p ${INSTALL_INC_PATH}
	find ${A9E_DIR} -type d | cut -sd / -f 2- | xargs -i mkdir -p ${INSTALL_INC_PATH}/{}
	find ${A9E_DIR} -name '*.h' | cut -sd / -f 2- | xargs -i xargs cp ${A9E_DIR}/{} ${INSTALL_INC_PATH}/{}

.ONEHSELL:
uninstall:
	rm -f ${INSTALL_LIB_PATH}/${A9E_SO}
	rm -rf ${INSTALL_INC_PATH}

