#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#define BG 1
#define REDX 3
#define BLUEO 2

#define BOARD_SIZE 2
#define START_X 5
#define START_Y 1

typedef struct CanvasData {
	int max_x;
	int max_y;
	
} CanvasData;

typedef struct {
    int x1, y1;  // Esquina superior izquierda de la celda
    int x2, y2;  // Esquina inferior derecha de la celda
} Cell;

typedef struct {
	int x,y;
	char text[20];
	int color;
} Button;

void drawButton(Button* button)
{

	int len = strlen(button->text);
	attron(COLOR_PAIR(button->color));
	mvhline(button->y,button->x,ACS_BLOCK,len);
	mvprintw(button->y+1,button->x,"%s",button->text);
	mvhline(button->y+2,button->x,ACS_BLOCK,len);
	attroff(COLOR_PAIR(button->color));
	

}

int checkButton(Button button, MEVENT event)
{
	int inside_x = button.x >= event.x || event.x <= button.x+(int)strlen(button.text);
	int inside_y = button.y >= event.y || event.y <= button.y+3;
	
	return inside_x && inside_y;
		
}


void mapBoard(Cell boardMap[3][3], CanvasData canvas) {
    int cell_width = (canvas.max_x / BOARD_SIZE) / 3;
    int cell_height = (canvas.max_y / BOARD_SIZE) / 3;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            boardMap[i][j].x1 = START_X + j * cell_width;
            boardMap[i][j].y1 = START_Y + i * cell_height;
            boardMap[i][j].x2 = boardMap[i][j].x1 + cell_width;
            boardMap[i][j].y2 = boardMap[i][j].y1 + cell_height;
        }
    }
}

void deconfTerminal()
{
	endwin();      
    keypad(stdscr, FALSE);  // Deshabilitar teclas especiales
    mousemask(0, NULL);     // Deshabilitar el mouse
    echo();                 // Habilitar la impresión de teclas
    curs_set(1);            // Mostrar el cursor
    attroff(A_BOLD);        // Desactivar cualquier atributo especial
    attroff(A_REVERSE);
    attroff(A_UNDERLINE);
    
}

CanvasData confTerminal()
{
	CanvasData canvas;
	getmaxyx(stdscr,canvas.max_y,canvas.max_x);
	
	
	keypad(stdscr,TRUE);
	mousemask(BUTTON1_CLICKED,NULL);
	noecho();
	curs_set(0);

	if (!has_colors())
	{
		printw("Tu terminal no es compatible con el juego.");
		deconfTerminal();
		exit(1);
	}

	start_color();

	init_pair(BG,COLOR_GREEN,COLOR_BLACK);
	init_pair(BLUEO,COLOR_BLUE,COLOR_BLACK);
	init_pair(REDX,COLOR_RED,COLOR_BLACK);

	return canvas;
}

void printBackground(CanvasData canvas)
{
	int cell_x = canvas.max_x/BOARD_SIZE/3;
	int cell_y = canvas.max_y/BOARD_SIZE/3;

	mvhline(START_Y,START_X,ACS_HLINE,canvas.max_y/BOARD_SIZE);
	for (int i = 1; i <= 2 ; i++)
		mvhline(START_Y+cell_y*i, START_X ,ACS_HLINE,canvas.max_x/BOARD_SIZE);

	for (int i = 1; i <= 2; i++)	
		mvvline(START_Y, START_X + cell_x*i,ACS_VLINE,(canvas.max_y)/BOARD_SIZE);
	
	//Borde
	attron(COLOR_PAIR(BG));
	mvhline(START_Y,START_X,ACS_BLOCK,canvas.max_x/BOARD_SIZE);
	mvhline(START_Y+cell_y*3,START_X,ACS_BLOCK,canvas.max_x/BOARD_SIZE);

	mvvline(START_Y,START_X,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	mvvline(START_Y,START_X-1,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	mvvline(START_Y,START_X+cell_x*3,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	mvvline(START_Y,START_X+cell_x*3+1,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	attroff(COLOR_PAIR(BG));
	refresh();
}

void printMove(MEVENT* event,CanvasData canvas, int game[3][3],Cell boardMap[3][3],int* turn)
{
	
	if (event->y < START_Y || event->y > START_Y+ canvas.max_y/BOARD_SIZE )
		return;
	if (event->x < START_X || event->x > START_X+ canvas.max_x/BOARD_SIZE )
		return;


	for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            // Si el clic está dentro de esta celda
            if (event->x > boardMap[i][j].x1 && event->x < boardMap[i][j].x2 &&
                event->y > boardMap[i][j].y1 && event->y < boardMap[i][j].y2) {

					if (game[i][j] != 0)
						return; //Ha sido seleccionado.
					
					

					
					int player  = *turn %2 == 0? BLUEO: REDX;
					
					*turn = *turn+1;

					game[i][j] = player;

					//imprimir en pantalla
					attron(COLOR_PAIR(player));
					if (player == REDX)
						mvprintw(event->y, event->x, "X");
					if (player == BLUEO)
						mvprintw(event->y, event->x,"O");
					
					attroff(COLOR_PAIR(player));
				}
		}
	}
	

	
}

void printGameOver(CanvasData canvas, int state)
{
	//Si es un jugador, es porque gano, sino es empate.
	int centerx = START_X+(canvas.max_x/BOARD_SIZE)/2;
	int centery = START_Y+(canvas.max_y/BOARD_SIZE)/2;
	switch (state)
	{
		case REDX:
			attron(COLOR_PAIR(REDX));
			mvprintw(centery,centerx-11,"¡GANADOR JUGADOR ROJO!");
			attroff(COLOR_PAIR(REDX));
			break;
		case BLUEO:
			attron(COLOR_PAIR(BLUEO));
			mvprintw(centery,centerx-11,"¡GANADOR JUGADOR AZUL!");
			attroff(COLOR_PAIR(BLUEO)); //se le resta la mitad de caracteres del titulo
			break;
		default:
			attron(COLOR_PAIR(BG));
			mvprintw(centery,centerx-8,"TERMINA EN EMPATE");
			attroff(COLOR_PAIR(BG));
			break;
		
	}
}

/**
 * Logica del juego
 */

bool findEndGame(int game[3][3], int turn)
{
	int player = turn % 2 == 0? REDX: BLUEO;

	//Revisar en el grafo unicamente si player gano.
	int winCount; // si es 3 significa que gano.
	for (int i = 0; i<3; i++)
	{
		winCount=0;
		for (int j = 0; j < 3; j++)
			if (player == game[i][j])
				winCount++;
			else
				break;
		if (winCount == 3)
			return TRUE;
	}

	for (int i = 0; i<3; i++)
	{
		winCount=0;
		for (int j = 0; j < 3; j++)
			if (player == game[j][i])
				winCount++;
			else
				break;
		if (winCount == 3)
			return TRUE;
	}

	//Revisar diagonal principal
	winCount=0;
	for (int i = 0; i < 3; i++)
		if (player == game[i][i])
			winCount++;
		else
			break;
	if (winCount == 3)
			return TRUE;
	
	//Revisar otra diagonal
	winCount=0;
	for (int i = 0; i < 3; i++)
		if (player == game[i][2-i])
			winCount++;
		else
			break;
	if (winCount == 3)
			return TRUE;
	return FALSE;	
}


void startGameLoop(CanvasData canvas)
{
	int game[3][3] = {0};
	Cell gameMap[3][3];
	mapBoard(gameMap,canvas);
		
	

	MEVENT event; //struct (id,int coordenadas,
		      // mmask_t vstate estado de los botones

	int turn = 1;
	while (1)
	{
		printBackground(canvas);
		int ch = getch();
		if (ch == KEY_MOUSE && getmouse(&event) == OK) {
			printMove(&event,canvas,game,gameMap,&turn);
			//if (gameEnded())
			refresh();
		}

		if (turn >5 && findEndGame(game,turn))
		{
			printGameOver(canvas, turn%2==0?REDX:BLUEO);
			break;
		}
			

	
		if (turn==10)
		{
			printGameOver(canvas, 0);
			break;
		}
			
		
	}

	
	
	refresh();
}


int main()
{
	initscr();
		
	CanvasData canvas = confTerminal();

	//Hacer botones
	
	Button retryBtn = {((canvas.max_x/BOARD_SIZE)/2)-8+START_X,
						(canvas.max_y/BOARD_SIZE)+1+START_Y,	"Intentar de nuevo",REDX};
	Button quitBtn = {((canvas.max_x/BOARD_SIZE)/2)-3+START_X,
						(canvas.max_y/BOARD_SIZE)+5+START_Y,	"Salir",BLUEO};
	while (1)
	{
		clear();
		startGameLoop(canvas);
		drawButton(&retryBtn);
		drawButton(&quitBtn);
		refresh();
		while (1) //Esperar respuesta.
		{
			MEVENT event;

			int ch = getch();
			if (ch == KEY_MOUSE && getmouse(&event) == OK) {
				
				if (checkButton(retryBtn,event))
					break;
				
				if (checkButton(quitBtn,event))
				{
					printw("Quiting..\n");
					deconfTerminal();
					exit(0);
				}
			}
			
		}
	}
	

	endwin();

	return 0;
	
}