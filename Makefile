LIBS   = -L./lib -L. -lgmpfrxx -lpaillier -lmpfr -lgmpxx -lgmp -lm


CC=g++ -g -Wall -std=c++11
BIN=bin
INC=include
LIB=lib
OBJ=obj
SRC=src


all: ${LIB}/libpaillier.a ${BIN}/test test

lib: ${LIB}/libpaillier.a

test: ${BIN}/unit_test runtest

runtest:
	${BIN}/unit_test

${LIB}/libpaillier.a: ${OBJ}/utils.o ${OBJ}/public_key.o ${OBJ}/private_key.o ${OBJ}/encrypted_number.o ${OBJ}/encoded_number.o
	ar rs ${LIB}/libpaillier.a ${OBJ}/utils.o ${OBJ}/public_key.o ${OBJ}/private_key.o ${OBJ}/encrypted_number.o ${OBJ}/encoded_number.o
	ranlib ${LIB}/libpaillier.a

${BIN}/test: 
	${CC} ${SRC}/test.cpp -o ${BIN}/test -I${INC} ${LIBS} 
	
${BIN}/unit_test: 
	${CC} ${SRC}/unit_test.cpp -o ${BIN}/unit_test -I${INC} ${LIBS} 

${OBJ}/utils.o: ${INC}/utils.h ${SRC}/utils.cpp
	${CC} -c $(CFLAGS) -o ${OBJ}/utils.o ${SRC}/utils.cpp -I${INC}

${OBJ}/public_key.o: ${INC}/public_key.h ${SRC}/public_key.cpp
	${CC} -c $(CFLAGS) -o ${OBJ}/public_key.o ${SRC}/public_key.cpp -I${INC}

${OBJ}/private_key.o: ${INC}/private_key.h ${SRC}/private_key.cpp
	${CC} -c $(CFLAGS) -o ${OBJ}/private_key.o ${SRC}/private_key.cpp -I${INC}

${OBJ}/encrypted_number.o: ${INC}/encrypted_number.h ${SRC}/encrypted_number.cpp
	${CC} -c $(CFLAGS) -o ${OBJ}/encrypted_number.o ${SRC}/encrypted_number.cpp -I${INC}

${OBJ}/encoded_number.o: ${INC}/encoded_number.h ${SRC}/encoded_number.cpp
	${CC} -c $(CFLAGS) -o ${OBJ}/encoded_number.o ${SRC}/encoded_number.cpp -I${INC}

# Cleans directories
clean:
	rm -rf ${OBJ}/* ${BIN}/* libpaillier.a

distclean: clean
	rm -rf ${LIB}/* 