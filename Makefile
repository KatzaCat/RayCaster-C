all: ofile
	gcc *.o -o RayCaster -s -L E:\programing\lybraries\SDL2-2.26.5\lib -lmingw32 -lSDL2main -lSDL2 -lopengl32 -lgdi32

ofile:
	gcc -c ray-caster.c -O3 -Wall -I E:\programing\lybraries\SDL2-2.26.5\include