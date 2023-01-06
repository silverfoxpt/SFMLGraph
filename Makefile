all: compile link

compile:
	g++ -o graph.o -c graph.cpp -I "D:\SFML-2.5.1\include" -DSFML_STATIC
	g++ -o main.o -c main.cpp -I "D:\SFML-2.5.1\include" -DSFML_STATIC	

link: 
	g++ main.o graph.o -o main -L "D:\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -mwindows -lsfml-main