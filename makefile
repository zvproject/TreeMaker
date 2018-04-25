tree: main.c menu.c tree.c StandLib.c
	gcc -g -o TreeMaker main.c menu.c tree.c StandLib.c -std=c11 `pkg-config --libs --cflags  gtk+-2.0`


