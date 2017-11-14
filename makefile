CC = g++ -std=c++11
CFLAGS = -Wall -Wextra -pedantic

compile: main.cpp
	${CC} -o baby_run ${CFLAGS} baby.cpp main.cpp

run:
	./main.cpp

clean:
	rm -f main.cpp
