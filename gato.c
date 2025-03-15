#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#define BG 1
#define REDX 3
#define BLUEO 2

#define BOARD_SIZE 3
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
		getch();
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
	mvhline(START_Y+cell_y*4,START_X,ACS_BLOCK,canvas.max_x/BOARD_SIZE);

	mvvline(START_Y,START_X,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	mvvline(START_Y,START_X+cell_x*3,ACS_BLOCK,canvas.max_y/BOARD_SIZE);
	attroff(COLOR_PAIR(BG));
	refresh();
}

void printMove(MEVENT* event,CanvasData canvas, int game[3][3],Cell boardMap[3][3])
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
					
					game[i][j]=1;
					attron(COLOR_PAIR(REDX));
					mvprintw(event->y, event->x, "X");
					attroff(COLOR_PAIR(REDX));
				}
		}
	}
	
	for (int i = 0; i < 3; i++)
		for (int j = 0; j < 3; j++)
			mvprintw(5+i,30+j,"%d",game[i][j]);

	
}

/**
 * Logica del juego
 */




int main()
{
	initscr();
	
	CanvasData canvas = confTerminal();
	int game[3][3] = {0};
	Cell gameMap[3][3];
	mapBoard(gameMap,canvas);
		
	

	MEVENT event; //struct (id,int coordenadas,
		      // mmask_t vstate estado de los botones
	while (1)
	{
		printBackground(canvas);
		int ch = getch();
		if (ch == KEY_MOUSE && getmouse(&event) == OK) {
			printMove(&event,canvas,game,gameMap);
			refresh();
			}
		
		
	}

	
	endwin();

	return 0;
	
}