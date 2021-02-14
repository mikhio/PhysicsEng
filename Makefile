all: app.cpp
	make compile
	make run
compile: app.cpp
	g++ -std=c++11 unity.cpp -o build/app -lsfml-graphics -lsfml-window -lsfml-system -Wall
run:
	clear
	build/app