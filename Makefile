all: compile link

compile:
	g++ -o graph.o -c graph.cpp -I "D:\SFML-2.5.1\include" -DSFML_STATIC
	g++ -o main.o -c main.cpp -I "D:\SFML-2.5.1\include" -DSFML_STATIC	
	g++ -o RPN.o -c RPN.cpp -I "D:\SFML-2.5.1\include" -DSFML_STATIC	

	g++ -o imgui.o -c "D:\SFML PROJECTS\SFMLGraph\imgui-master\imgui.cpp" -I "D:\SFML PROJECTS\SFMLGraph\imgui-master"
	g++ -o imgui_widgets.o -c "D:\SFML PROJECTS\SFMLGraph\imgui-master\imgui_widgets.cpp" -I "D:\SFML PROJECTS\SFMLGraph\imgui-master"
	g++ -o imgui_draw.o -c "D:\SFML PROJECTS\SFMLGraph\imgui-master\imgui_draw.cpp" -I "D:\SFML PROJECTS\SFMLGraph\imgui-master"
	g++ -o imgui_tables.o -c "D:\SFML PROJECTS\SFMLGraph\imgui-master\imgui_tables.cpp" -I "D:\SFML PROJECTS\SFMLGraph\imgui-master"

link: 
	g++ main.o graph.o RPN.o imgui.o imgui_widgets.o imgui_draw.o imgui_tables.o -o main -L "D:\SFML-2.5.1\lib" -lsfml-graphics-s -lsfml-window-s -lsfml-system-s -lopengl32 -lfreetype -lwinmm -lgdi32 -lsfml-main