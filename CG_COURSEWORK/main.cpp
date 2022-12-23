#include "../libwinbgi/src/graphics.h"
#include <stdio.h>
#include "piramid.h"

#define INSTR_X 400
#define INSTR_Y_ST 0
void menu()
{
	char message[150];
	setcolor(15);
	sprintf_s(message, "movement w a s d q e + same with SHIFT");
	outtextxy(INSTR_X, INSTR_Y_ST+20, message);
	sprintf_s(message, "rotations r f t g y h + same with SHIFT");
	outtextxy(INSTR_X, INSTR_Y_ST + 40, message);
	sprintf_s(message, "scaling z x + same with SHIFT");
	outtextxy(INSTR_X, INSTR_Y_ST + 60, message);

	sprintf_s(message, "move LIGHT left and right on 1 2");
	outtextxy(INSTR_X, INSTR_Y_ST + 80, message);
	sprintf_s(message, "turn on/off semi-transparent mode 5");
	outtextxy(INSTR_X, INSTR_Y_ST + 100, message);
	sprintf_s(message, "turn on/off drawing of edges 6");
	outtextxy(INSTR_X, INSTR_Y_ST + 120, message);
	sprintf_s(message, "turn on/off LIGHT lining 7 (only for semi-transparent mode)");
	outtextxy(INSTR_X, INSTR_Y_ST + 140, message);
	sprintf_s(message, "turn on/off letters 8");
	outtextxy(INSTR_X, INSTR_Y_ST + 160, message);

	sprintf_s(message, "EXIT 0");
	outtextxy(INSTR_X, INSTR_Y_ST + 190, message);

}



int main()
{
	initwindow(1300, 700);

	piramid* p = new piramid(5);
	piramid* p2 = new piramid(4);
	
	
	menu();
	p->paint();
	p2->paint();

	char message[32];
	
	sprintf_s(message, "start");
	setcolor(15);
	outtextxy(1200, 650, message);
	bool unknownMetFlag = false;
	bool ruleVialationMet = false;
	char c;
	while (true)
	{
		c = getch();
		switch (c)
		{
		case '1':
			p->move_light(-20, 0, 0);
			p2->move_light(-20, 0, 0);
			break;
		case '2':
			p->move_light(20, 0, 0);
			p2->move_light(20, 0, 0);
			break;

		case '5':
			p->changeDrawMode();
			p2->changeDrawMode();
			break;
		case '6':
			p->changeOutlineMode();
			p2->changeOutlineMode();
			break;
		case '7':
			if (p->changetoLineLight() || p2->changetoLineLight()) ruleVialationMet = true;
			break;
		case '8':
			p->changeNamingMode();
			p2->changeNamingMode();
			break;
		case 'z': p->scaleS(); break;
		case 'x': p->scaleB(); break;			  
		case 'w': p->moveUP(); break;
		case 's': p->moveDW(); break;
		case 'a': p->moveLE(); break;
		case 'd': p->moveRT(); break;
		case 'q': p->moveFR(); break;
		case 'e': p->moveTO(); break;
		case 'r': p->rotXpl(); break;
		case 'f': p->rotXmi(); break;
		case 't': p->rotYpl(); break;
		case 'g': p->rotYmi(); break;
		case 'y': p->rotZpl(); break;
		case 'h': p->rotZmi(); break;

		case 'R': p2->rotXpl(); break;
		case 'F': p2->rotXmi(); break;
		case 'T': p2->rotYpl(); break;
		case 'G': p2->rotYmi(); break;
		case 'Y': p2->rotZpl(); break;
		case 'H': p2->rotZmi(); break;
		case 'Z': p2->scaleS(); break;
		case 'X': p2->scaleB(); break;
		case 'W': p2->moveUP(); break;
		case 'S': p2->moveDW(); break;
		case 'A': p2->moveLE(); break;
		case 'D': p2->moveRT(); break;
		case 'Q': p2->moveFR(); break;
		case 'E': p2->moveTO(); break;


		case '0':
			return 0;
		default:
			unknownMetFlag = true;
		}
		clearviewport();
		menu();
		p->paint();
		p2->paint();
		if (ruleVialationMet)
		{
			char message[50];
			sprintf_s(message, "cannot line light in non-transparent mode!");
			setcolor(15);
			outtextxy(1000, 20, message);
		}
		else
		if (unknownMetFlag)
		{
			sprintf_s(message, "unknown");
			unknownMetFlag = false;
		}
		else
			sprintf_s(message, "%c", c);
			
		setcolor(15);
		outtextxy(1200, 650, message);
	}

	getch();

	closegraph();

	return 0;
}