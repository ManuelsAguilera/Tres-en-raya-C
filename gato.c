#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int main()
{
	initscr();
	keypad(stdscr, TRUE);
	
	mousemask(ALL_MOUSE_EVENTS,NULL); // Para detectar eventos de mouse
	refresh();
	
	MEVENT event; //struct (id,int coordenadas,
		      // mmask_t vstate estado de los botones
	//Test
	
	int ch = getch();
	if (ch == KEY_MOUSE && getmouse(&event) == OK) {
		mvprintw(event.y, event.x, "X");
		refresh();
		}
	
	getch();	
	endwin();

	return 0;
	
}