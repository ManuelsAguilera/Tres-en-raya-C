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

CanvasData confTerminal()
{
	CanvasData canvas;
	getmaxyx(stdscr,canvas.max_y,canvas.max_x);
	printw("filas: %d, columnas: %d",canvas.max_x, canvas.max_y);
	
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

int main()
{
	initscr();
	
	CanvasData canvas = confTerminal();
	
	

	MEVENT event; //struct (id,int coordenadas,
		      // mmask_t vstate estado de los botones
	while (1)
	{
		printBackground(canvas);
		int ch = getch();
		if (ch == KEY_MOUSE && getmouse(&event) == OK) {
			mvprintw(event.y, event.x, "X");
			refresh();
			}
		
		
	}

	
	endwin();

	return 0;
	
}