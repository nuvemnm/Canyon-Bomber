#include <stdio.h>
#include <time.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_image.h>


const float FPS = 100;  
const int SCREEN_W = 960;
const int SCREEN_H = 540;
const int NAVE_W = 50;
const int NAVE_H = 20;
const int RAIO_TIRO = 3;
const float ACEL_TIRO = 0.05;
const int MAT_W = 20;
const int MAT_H = 20;

typedef struct Tiro {
	float x, y;
	int ativo;
	int hit;
	float vel_y;
} Tiro;

typedef struct Nave {
	float x, y;
	ALLEGRO_COLOR cor;
	ALLEGRO_BITMAP *naveTipo;
	int dir;
	int vida;
	int pontos;
	int vitorias;
	float vel;
	Tiro tiro;
} Nave;

typedef struct Alvo{
	float x, y;
	int ativo;
	int valor;
	ALLEGRO_COLOR cor;
}Alvo;


void desenha_cenario() {
	ALLEGRO_COLOR cor = al_map_rgb(0, 0, 0);
	al_clear_to_color(cor);
}

void desenhaBloco(Alvo bloco){
	if(bloco.ativo){
		al_draw_filled_rectangle(bloco.x, bloco.y, bloco.x + MAT_W, bloco.y + MAT_H, bloco.cor); 
	}
}

void desenha_nave(Nave nave) {
	al_draw_bitmap(nave.naveTipo, nave.x, nave.y, 0);							
	//desenha tiro:
	if(nave.tiro.ativo)
		al_draw_filled_circle(nave.tiro.x, nave.tiro.y, RAIO_TIRO, al_map_rgb(255,255,255));
}

//funcao que troca as posiçoes com 50% de chance
void trocaPosicao(Nave *nave1, Nave *nave2){
	
	int random = rand()%2;
	//variavel auxiliar pra troca de posiçao das naves
	if(random == 1){
		int trocaP;
		trocaP = nave1->y;
		nave1->y = nave2->y;
		nave2->y = trocaP;
	}
}

void atualiza_nave01(Nave *nave, Nave *nave2, int troca) {
	nave->x += (nave->dir)*nave->vel;
	if(nave->x > SCREEN_W + NAVE_W){
		nave->dir *= -1;
		trocaPosicao(nave, nave2);
		//troca de tipo das naves
		if(troca == 0){
			nave->naveTipo = al_load_bitmap("nave1Modelo - esquerda.png");
		}else {
			nave->naveTipo = al_load_bitmap("nave1Modelo2 - esquerda.png");
		}
	}
	else if(nave->x < -NAVE_W){
		nave->dir *= -1;
		trocaPosicao(nave, nave2);
		//troca de tipo das naves
		if(troca == 0){
			nave->naveTipo = al_load_bitmap("nave1Modelo.png");
		}else {
			nave->naveTipo = al_load_bitmap("nave1Modelo2.png");
		}
	}
	
	//atualiza tiro
	if(nave->tiro.ativo) {
		nave->tiro.x = nave->x;
		nave->tiro.vel_y += ACEL_TIRO;
		nave->tiro.y += nave->tiro.vel_y;
	}

//reseto o tiro e diminuo uma vida com espaço limitado(caso nao acerte nenhum alvo)
	if((nave->tiro.y > SCREEN_H + RAIO_TIRO && nave->tiro.hit == 0) || 
			(nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 0) || 
					  (nave->tiro.x < -RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 0)){
						nave->tiro.ativo = 0;
		   				nave->tiro.x = 1;
		  				nave->tiro.vel_y = 0;
          				nave->tiro.y = 1;
						nave->vida--;
	}else if((nave->tiro.y > SCREEN_H + RAIO_TIRO && nave->tiro.hit == 1) || 
			(nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 1) || 
					  (nave->tiro.x < -RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 1)){
						nave->tiro.ativo = 0;
		   				nave->tiro.x = 1;
		  				nave->tiro.vel_y = 0;
          				nave->tiro.y = 1;
	}
	else if((nave->tiro.x < -RAIO_TIRO && nave->tiro.y < (SCREEN_H/2)+10) || (nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y < (SCREEN_H/2)+10)){
		nave->tiro.ativo = 0;
	}
}

void atualiza_nave02(Nave *nave, int troca) {
	nave->x += (nave->dir)*nave->vel;
	if(nave->x > SCREEN_W + NAVE_W){
		nave->dir *= -1;
		//troca de tipo das naves
		if(troca == 0){
			nave->naveTipo = al_load_bitmap("nave2Modelo - esquerda.png");
		}else {
			nave->naveTipo = al_load_bitmap("nave2Modelo2 - esquerda.png");
		}

	}
	else if(nave->x < -NAVE_W){
		nave->dir *= -1;
		//troca de tipo das naves
		if(troca == 0){
			nave->naveTipo = al_load_bitmap("nave2Modelo.png");
		}else {
			nave->naveTipo = al_load_bitmap("nave2Modelo2.png");
		}
	}
	//atualiza tiro
	if(nave->tiro.ativo) {
		nave->tiro.x = nave->x;
		nave->tiro.vel_y += ACEL_TIRO;
		nave->tiro.y += nave->tiro.vel_y;
	}
	
	//reseto o tiro e diminuo uma vida com espaço limitado(caso nao acerte nenhum alvo)
	if((nave->tiro.y > SCREEN_H + RAIO_TIRO && nave->tiro.hit == 0) || 
			(nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 0) || 
					  (nave->tiro.x < -RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 0)){
						nave->tiro.ativo = 0;
		   				nave->tiro.x = 1;
		  				nave->tiro.vel_y = 0;
          				nave->tiro.y = 1;
						nave->vida--;
	}else if((nave->tiro.y > SCREEN_H + RAIO_TIRO && nave->tiro.hit == 1) || 
			(nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 1) || 
					  (nave->tiro.x < -RAIO_TIRO && nave->tiro.y > SCREEN_H/2 && nave->tiro.hit == 1)){
						nave->tiro.ativo = 0;
		   				nave->tiro.x = 1;
		  				nave->tiro.vel_y = 0;
          				nave->tiro.y = 1;
	}
	else if((nave->tiro.x < -RAIO_TIRO && nave->tiro.y < (SCREEN_H/2)+10) || (nave->tiro.x > SCREEN_W + RAIO_TIRO && nave->tiro.y < (SCREEN_H/2)+10)){
		nave->tiro.ativo = 0;
	}
}

int colisao(Alvo bloco[13][48], Nave *nave){
	for(int i = 0; i < 13; i++){
		for(int j = 0; j < 48; j++){
			if(nave->tiro.x >= bloco[i][j].x && nave->tiro.y >= bloco[i][j].y && 
			   nave->tiro.x <= (bloco[i][j].x + MAT_W) && nave->tiro.y <= (bloco[i][j].y + MAT_H) && bloco[i][j].ativo == 1){
				nave->tiro.hit = 1;
				nave->pontos += bloco[i][j].valor;
				bloco[i][j].ativo = 0;
				return nave->pontos;
			   }
		}
	}
} 

int main(int argc, char **argv){
	
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_BITMAP *image = NULL;
	ALLEGRO_BITMAP *image02 = NULL;
	ALLEGRO_BITMAP *image03 = NULL;
	FILE *historico = fopen("historico.txt", "w");
	srand((unsigned)time(NULL));
	
	//----------------------- rotinas de inicializacao ---------------------------------------
    
	//inicializa o Allegro
	if(!al_init()) {
		fprintf(stderr, "failed to initialize allegro!\n");
		return -1;
	}
	
    //inicializa o módulo de primitivas do Allegro
    if(!al_init_primitives_addon()){
		fprintf(stderr, "failed to initialize primitives!\n");
        return -1;
    }	
	
	//inicializa o modulo que permite carregar imagens no jogo
	if(!al_init_image_addon()){
		fprintf(stderr, "failed to initialize image module!\n");
		return -1;
	}
   
	//cria um temporizador que incrementa uma unidade a cada 1.0/FPS segundos
    timer = al_create_timer(1.0 / FPS);
    if(!timer) {
		fprintf(stderr, "failed to create timer!\n");
		return -1;
	}
 
	//cria uma tela com dimensoes de SCREEN_W, SCREEN_H pixels
	display = al_create_display(SCREEN_W, SCREEN_H);
	if(!display) {
		fprintf(stderr, "failed to create display!\n");
		al_destroy_timer(timer);
		return -1;
	}

	//instala o teclado
	if(!al_install_keyboard()) {
		fprintf(stderr, "failed to install keyboard!\n");
		return -1;
	}
	
	//inicializa o modulo allegro que carrega as fontes
	al_init_font_addon();

	//inicializa o modulo allegro que entende arquivos tff de fontes
	if(!al_init_ttf_addon()) {
		fprintf(stderr, "failed to load tff font module!\n");
		return -1;
	}
	
	//carrega o arquivo arial.ttf da fonte Arial e define que sera usado o tamanho 32 (segundo parametro)
    ALLEGRO_FONT *size_32 = al_load_font("arial.ttf", 32, 1);   
	if(size_32 == NULL) {
		fprintf(stderr, "font file does not exist or cannot be accessed!\n");
	}

 	//cria a fila de eventos
	event_queue = al_create_event_queue();
	if(!event_queue) {
		fprintf(stderr, "failed to create event_queue!\n");
		al_destroy_display(display);
		return -1;
	}

	//registra na fila os eventos de tela (ex: clicar no X na janela)
	al_register_event_source(event_queue, al_get_display_event_source(display));
	//registra na fila os eventos de tempo: quando o tempo altera de t para t+1
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	//registra na fila os eventos de teclado (ex: pressionar uma tecla)
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	
	al_init();
    al_init_font_addon();
    al_init_ttf_addon();
	ALLEGRO_FONT *font = al_load_ttf_font("arial.ttf", 18, 0);
	ALLEGRO_FONT *font2 = al_load_ttf_font("arial.ttf", 24, 0);
	//Criar a Nave 1:
	Nave p1;
	p1.x = -1;
	p1.y = NAVE_H*2;
	p1.cor = al_map_rgb(255, 255, 255);
	p1.dir = 1;
	p1.vel = 3;
	p1.vida = 3;
	p1.pontos = 0;
	p1.vitorias = 0; 
	p1.tiro.x = 0;
	p1.tiro.y = 0;
	p1.tiro.ativo = 0;
	p1.tiro.hit = 0;
	p1.tiro.vel_y = 0;
	
	//Criar a Nave 2:
	Nave p2;
	p2.x = 961;
	p2.y = NAVE_H*3;
	p2.cor = al_map_rgb(0, 0, 0);
	p2.dir = -1;
	p2.vel = 3;
	p2.vida = 3;
	p2.pontos = 0;
	p2.vitorias = 0;
	p2.tiro.x = 0;
	p2.tiro.y = 0;
	p2.tiro.ativo = 0;
	p2.tiro.hit = 0;
	p2.tiro.vel_y = 0;
	
	//Crio os blocos com seus valores
	Alvo bloco[13][48];
	for(int i = 0; i < 13; i++){
		for(int j = 0; j < 48; j++){
			bloco[i][j].x = j * MAT_W;
			bloco[i][j].y = (i * MAT_H) + 280;
			if(i % 2 == 0){
				bloco[i][j].cor = al_map_rgb(255, 140, 0);
			}else{
				bloco[i][j].cor = al_map_rgb(255, 255, 0);
			}
			bloco[i][j].valor = i + 7;
			bloco[i][j].ativo = 1;
		}
	}
	
	//criar os textos que aparecem na tela
	char pontuacao[25];
	char pontuacao1[25];
	char TEX1[50]; char TEX2[50]; char TEX3[50]; char TEX4[50]; char TEX5[50]; char TEX6[50]; char TEX7[50];

	//inicia o temporizador
	al_start_timer(timer);
	int jogo = 1;
	int menu = 1;
	int playing = 0;
	int gameover = 0;
	int troca = 0;
	int count = 0;
	int m = 0;
	int n = 0;
	//atriubui imagens as telas
	al_init_image_addon();
    image = al_load_bitmap("play.png");
	image02 = al_load_bitmap("logo.png");
	image03 = al_load_bitmap("gameover.png");
	p1.naveTipo = al_load_bitmap("nave1Modelo.png");
	p2.naveTipo = al_load_bitmap("nave2Modelo - esquerda.png");


	while(jogo){	
		
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if(ev.type == ALLEGRO_EVENT_TIMER){
			//cria o menu
			while(menu){
				ALLEGRO_EVENT ev;
				//espera por um evento e o armazena na variavel de evento ev
				al_wait_for_event(event_queue, &ev);
				//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
				if(ev.type == ALLEGRO_EVENT_TIMER) {
					//cria o menu com as imagens
					desenha_cenario();
					al_draw_bitmap(image, 380, 230, 0);
					al_draw_bitmap(image02, 340, 0, 0);
					sprintf(TEX6, "JOGADOR 01: PRESS SPACE");
					al_draw_text(font, p1.cor, 100, 400, ALLEGRO_ALIGN_LEFT, TEX6);
					sprintf(TEX7, "JOGADOR 02: PRESS ENTER");
					al_draw_text(font, p1.cor, 860, 400, ALLEGRO_ALIGN_RIGHT, TEX7);

					al_flip_display();
				}
				//se o tipo de evento for o fechamento da tela (clique no x da janela)
				else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					menu = 0;
					jogo = 0;
				}
				//se apertar enter o jogo começa
				else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
					
					switch(ev.keyboard.keycode) {
						
						case ALLEGRO_KEY_ENTER:
							menu = 0;
							playing = 1;
							gameover = 0;
						break;
					}
				}
			}
			
			while(playing) {
				ALLEGRO_EVENT ev;
				//espera por um evento e o armazena na variavel de evento ev
				al_wait_for_event(event_queue, &ev);
				//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
				if(ev.type == ALLEGRO_EVENT_TIMER) {
					//varia o tipo da nave
					troca = rand()%2;
					m = rand()%13;
					n = rand()%48;
					desenha_cenario();
					for(int i = 0; i < 13; i++){
						for(int j = 0; j < 48; j++){
							desenhaBloco(bloco[i][j]);
						}
					}
					atualiza_nave01(&p1, &p2, troca);
					atualiza_nave02(&p2, troca);
					desenha_nave(p1);
					desenha_nave(p2);
					
					//escreve os valores de vida e pontos na tela
					sprintf(pontuacao, "vidas: %d  pontuacao: %d", p1.vida, p1.pontos);
					al_draw_text(font, p1.cor, 10, 0, ALLEGRO_ALIGN_LEFT, pontuacao);
					sprintf(pontuacao1, "vidas: %d  pontuacao: %d", p2.vida, p2.pontos);
					al_draw_text(font, p1.cor, 950, 0, ALLEGRO_ALIGN_RIGHT, pontuacao1);

					colisao(bloco, &p1);
					colisao(bloco, &p2);

					if(p1.vida == 0 || p2.vida == 0){
						playing = 0;
						gameover = 1;
						
						if(p1.vida == 0){
							sprintf(TEX1, "JOGADOR 02 AMASSOU!!!");
							p2.vitorias++;
						}else{
							sprintf(TEX1, "JOGADOR 01 AMASSOU!!!");
							p1.vitorias++;
						}
					}

					for(int i = 0; i < 13; i++){
						for(int j = 0; j < 48; j++){
							if(bloco[i][j].ativo == 0){
								count++;
							}
						}
					}

					if(count == 624){
						playing = 0;
						gameover = 1;

						if(p1.pontos > p2.pontos){
							sprintf(TEX1, "JOGADOR 01 AMASSOU!!!");
							p1.vitorias++;
						}else{
							sprintf(TEX1, "JOGADOR 02 AMASSOU!!!");
							p2.vitorias++;
						}
					}
					//atualiza a tela (quando houver algo para mostrar)
					al_flip_display();

					if(al_get_timer_count(timer)%(int)FPS == 0)
						printf("\n%d segundos se passaram...", (int)(al_get_timer_count(timer)/FPS));
				}
				//se o tipo de evento for o fechamento da tela (clique no x da janela)
				else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					playing = 0;
					jogo = 0;
				}

				//se o tipo de evento for um pressionar de uma tecla
				else if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
					
					switch(ev.keyboard.keycode) {
						
						case ALLEGRO_KEY_SPACE:
							p1.tiro.ativo = 1;
							p1.tiro.hit = 0;
							p1.tiro.x = p1.x;
							p1.tiro.y = p1.y;
							p1.tiro.vel_y = 0;
						break;
						case ALLEGRO_KEY_ENTER:
							p2.tiro.ativo = 1;
							p2.tiro.hit = 0;
							p2.tiro.x = p2.x;
							p2.tiro.y = p2.y;
							p2.tiro.vel_y = 0;
						break;
						case ALLEGRO_KEY_R:
							bloco[m][n].ativo = 0;
							p1.pontos += bloco[m][n].valor;
					}
					
					//imprime qual tecla foi
					printf("\ncodigo tecla: %d", ev.keyboard.keycode);
				}
				
			} //fim do while

			while(gameover){
				ALLEGRO_EVENT ev;
				//espera por um evento e o armazena na variavel de evento ev
				al_wait_for_event(event_queue, &ev);
				//se o tipo de evento for um evento do temporizador, ou seja, se o tempo passou de t para t+1
				if(ev.type == ALLEGRO_EVENT_TIMER) {
					//cria a tela final com gameover
					desenha_cenario();
					al_draw_bitmap(image03, 380, 0, 0);


					al_draw_text(font2, p1.cor, 480, 200, ALLEGRO_ALIGN_CENTER, TEX1);
					sprintf(TEX2, "SCORE JO1: %d pontos", p1.pontos);
					al_draw_text(font, p1.cor, 480, 250, ALLEGRO_ALIGN_CENTER, TEX2);
					sprintf(TEX3, "SCORE JO2: %d pontos", p2.pontos);
					al_draw_text(font, p1.cor, 480, 270, ALLEGRO_ALIGN_CENTER, TEX3);
					sprintf(TEX4, "VITORIAS J01: %d", p1.vitorias);
					al_draw_text(font, p1.cor, 200, 270, ALLEGRO_ALIGN_CENTER, TEX4);
					sprintf(TEX5, "VITORIAS J02: %d", p2.vitorias);
					al_draw_text(font, p1.cor, 760, 270, ALLEGRO_ALIGN_CENTER, TEX5);
					
				
					al_flip_display();
				}

				//se o tipo de evento for o fechamento da tela (clique no x da janela)
				else if(ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
					gameover = 0;
					jogo = 0;
				}
				//se o evente for o pressionar do enter o jogo reinicia
				else if(ev.type == ALLEGRO_EVENT_KEY_DOWN){
					fprintf(historico, "SCORE J01: %d   ", p1.pontos);
					fprintf(historico, "SCORE J02: %d\n", p2.pontos);

					switch(ev.keyboard.keycode) {
						//reseta todos os atributos ao apertar lctrl
						case ALLEGRO_KEY_LCTRL:
							gameover = 0;
							menu = 1;

							p1.vida = 3;
							p1.x = -1;
							p1.pontos = 0;

							p2.vida = 3;
							p2.x = 961;
							p2.pontos = 0;

							for(int i = 0; i < 13; i++){
								for(int j = 0; j < 48; j++){
									bloco[i][j].ativo = 1;
								}
							}

						break;
					}

				}
				
			}
			
		}
	}
	fprintf(historico, "vitoria J01: %d   ", p1.vitorias);
	fprintf(historico, "vitoria J02: %d\n", p2.vitorias);
	fclose(historico);
	//procedimentos de fim de jogo (fecha a tela, limpa a memoria, etc)
	al_destroy_timer(timer);
	al_destroy_display(display);
	al_destroy_event_queue(event_queue);
	return 0;
}