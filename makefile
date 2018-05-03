tree: main.c menu.c tree.c StandLib.c
	gcc -g -o TreeMaker main.c menu.c tree.c StandLib.c  `pkg-config --libs --cflags  gtk+-2.0`


