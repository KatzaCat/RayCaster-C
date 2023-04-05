all: ofile
	gcc *.o -o RayCaster -s -L E:\programing\lybraries\SDL\lib -lmingw32 -lSDL2main -lSDL2 -lgdi32

ofile:
	gcc -c ./*.c -Wall -I E:\programing\lybraries\SDL\include