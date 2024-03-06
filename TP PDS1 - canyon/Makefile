ALLEGRO_VERSION=5.0.10
MINGW_VERSION=4.7.0
FOLDER=C:

FOLDER_NAME=\allegro-$(ALLEGRO_VERSION)-mingw-$(MINGW_VERSION)
PATH_ALLEGRO=$(FOLDER)$(FOLDER_NAME)
LIB_ALLEGRO=\lib\liballegro-$(ALLEGRO_VERSION)-monolith-mt.a
INCLUDE_ALLEGRO=\include

all: teclado.exe tela.exe canyon.exe GAME_NAVE.exe

canyon.exe: canyon.o
	gcc -o canyon.exe canyon.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

canyon.o: canyon.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c canyon.c

GAME_NAVE.exe: GAME_NAVE.o
	gcc -o GAME_NAVE.exe GAME_NAVE.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

GAME_NAVE.o: GAME_NAVE.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c GAME_NAVE.c

teclado.exe: teclado.o
	gcc -o teclado.exe teclado.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

teclado.o: teclado.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c teclado.c	

tela.exe: tela.o
	gcc -o tela.exe tela.o $(PATH_ALLEGRO)$(LIB_ALLEGRO)

tela.o: tela.c
	gcc -I $(PATH_ALLEGRO)$(INCLUDE_ALLEGRO) -c tela.c	
	
	
clean:
	del teclado.o 
	del teclado.exe
	del tela.o
	del tela.exe	
	del canyon.o
	del canyon.exe
	del GAME_NAVE.o
	del GAME_NAVE.exe


