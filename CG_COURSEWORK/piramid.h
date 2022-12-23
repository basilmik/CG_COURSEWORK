#pragma once
#include "stdio.h"
#include "side.h"

#define MOV_DELTA 20
#define FLAG 0
#define BR 1
#define DR 2
#define VS 3

#define FLAG_NUMBER 4

class piramid
{
	int n;

	side** telo_sides;
	side** shad_sides;
	point** v;
	point* mid;
	point* cam;
	point* light;

	int** clr;

	bool toLineLight;

	void rotate(double _phi, int axis)
	{
		upd_mid();
		move(-mid->x, -mid->y, -mid->z);
		for (int i = 0; i < n; i++)
			v[i]->rotate(_phi, axis);
		move(mid->x, mid->y, mid->z);
	}


	void scale_each(double _x, double _y, double _z)
	{
		for (int i = 0; i < n; i++)
			v[i]->scale(_x, _y, _z);
	}

	void upd_shadow()
	{
		point* ref = light;
		double y = 600;

		double t = (y - v[0]->y) / (ref->y - v[0]->y);
		double x = (ref->x - v[0]->x) * t + v[0]->x;
		double z = (ref->z - v[0]->z) * t + v[0]->z;

		point* A = new point(x, y, z);

		t = (y - v[1]->y) / (ref->y - v[1]->y);
		x = (ref->x - v[1]->x) * t + v[1]->x;
		z = (ref->z - v[1]->z) * t + v[1]->z;
		point* B = new point(x, y, z);

		t = (y - v[2]->y) / (ref->y - v[2]->y);
		x = (ref->x - v[2]->x) * t + v[2]->x;
		z = (ref->z - v[2]->z) * t + v[2]->z;
		point* C = new point(x, y, z);

		t = (y - v[3]->y) / (ref->y - v[3]->y);
		x = (ref->x - v[3]->x) * t + v[3]->x;
		z = (ref->z - v[3]->z) * t + v[3]->z;
		point* D = new point(x, y, z);

		A->rotate(30, X);
		B->rotate(30, X);
		C->rotate(30, X);
		D->rotate(30, X);
		if (n == 4)
		{
			shad_sides[0]->reset(A, B, C);
			shad_sides[1]->reset(A, D, C);
			shad_sides[2]->reset(D, B, C);
			shad_sides[3]->reset(A, D, B);

			if (toLineLight)
			{
				setlinestyle(0, 1, 3);
				setcolor(15);
				line(light->x, light->y, A->x, A->y);
				setcolor(15);
				line(light->x, light->y, B->x, B->y);
				setcolor(15);
				line(light->x, light->y, C->x, C->y);
				setcolor(15);
				line(light->x, light->y, D->x, D->y);
			}
		}
		if (n == 5)
		{
			t = (y - v[4]->y) / (ref->y - v[4]->y);
			x = (ref->x - v[4]->x) * t + v[4]->x;
			z = (ref->z - v[4]->z) * t + v[4]->z;
			point* E = new point(x, y, z);
			E->rotate(30, X);

			shad_sides[0]->reset(D, C, B, A);
			shad_sides[1]->reset(B, C, E);
			shad_sides[2]->reset(C, D, E);
			shad_sides[3]->reset(D, A, E);
			shad_sides[4]->reset(A, B, E);
			if (toLineLight)
			{
				setlinestyle(0, 1, 3);
				setcolor(15);
				line(light->x, light->y, A->x, A->y);
				line(light->x, light->y, B->x, B->y);
				line(light->x, light->y, C->x, C->y);
				line(light->x, light->y, D->x, D->y);
				line(light->x, light->y, E->x, E->y);
			}

		}


	}

	void project()
	{
		rotate(30, X);
	}

	void unproject()
	{
		rotate(-30, X);
	}

	void upd_mid()
	{
		mid->x = 0; mid->y = 0; mid->z = 0;
		for (int i = 0; i < n; i++)
		{
			mid->x += v[i]->x;
			mid->y += v[i]->y;
			mid->z += v[i]->z;
		}
		mid->x /= (float)n;
		mid->y /= (float)n;
		mid->z /= (float)n;

		mid->w = 1;
	}

	void move(double _x, double _y, double _z)
	{
		for (int i = 0; i < n; i++)
			v[i]->move(_x, _y, _z);
	}

	void upd_sides()
	{
		for (int i = 0; i < n; i++)
		{
			telo_sides[i]->calc_coefs(0);
		}
	}

	void check_signs()
	{
		for (int i = 0; i < n; i++)
		{
			if (telo_sides[i]->sign(mid) == 1)
				telo_sides[i]->swap_sign();
		}
	}

	void check_colors()
	{
		point* ref = new point(light->x - mid->x, light->y - mid->y, light->z - mid->z);
		for (int i = 0; i < n; i++)
		{
			if (telo_sides[i]->isSeen(ref) == 1)
				clr[FLAG][i] = BR;
			else
				clr[FLAG][i] = DR;
		}
	}

	void check_visibility()
	{
		point* ref = cam;
		for (int i = 0; i < n; i++)
		{
			if (telo_sides[i]->isSeen(ref) == 1)
				clr[VS][i] = 1;
			else
				clr[VS][i] = 0;
		}
	}

	void draw_fig()
	{
		for (int i = 0; i < n; i++)
		{
			if (clr[VS][i] == 1)
			{
				int l = clr[FLAG][i];
				telo_sides[i]->color(clr[l][i]);
			}
		}
	}

	void draw_shadow()
	{
		for (int i = 0; i < n; i++)
			shad_sides[i]->color(8);
	}



public:

	piramid(int _n)
	{
		mid = new point;
		cam = new point(0, -500, 1000);
		light = new point(500, -500, 0);
		toLineLight = false;
		if (_n == 4)
		{
			
			n = 4;
			v = new point * [n];
			telo_sides = new side * [n];
			shad_sides = new side * [n];

			for (int i = 0; i < 4; i++)
				shad_sides[i] = new side(3);

			point* A = new point(0, 150, 0, (char*)"A");
			point* B = new point(150, 0, 200, (char*)"B");
			point* C = new point(250, 150, 200, (char*)"C");
			point* D = new point(150, 50, 0, (char*)"D");

			v[0] = A;
			v[1] = B;
			v[2] = C;
			v[3] = D;

			telo_sides[0] = new side(A, B, C);
			telo_sides[1] = new side(A, D, C);
			telo_sides[2] = new side(D, B, C);
			telo_sides[3] = new side(A, D, B);
			
			clr = new int* [n];
			for (int i = 0; i < n; i++)
			{
				clr[i] = new int[4];
			}

			for (int i = 0; i < n; i++)
				clr[FLAG][i] = 0;

			clr[BR][0] = 9; 
			clr[BR][1] = 10;
			clr[BR][2] = 11; 
			clr[BR][3] = 12; 

			clr[DR][0] = 1;
			clr[DR][1] = 2;
			clr[DR][2] = 3;
			clr[DR][3] = 4;

			for (int i = 0; i < n; i++)
				clr[VS][i] = 1;
			move(700, 0, 0);
		}



		if (_n == 5)
		{

			n = 5;
			v = new point * [n];
			telo_sides = new side * [n];
			shad_sides = new side * [n];

			
			shad_sides[0] = new side(4);
			shad_sides[1] = new side(3);
			shad_sides[2] = new side(3);
			shad_sides[3] = new side(3);
			shad_sides[4] = new side(3);

			point* A = new point(100, 200, 300, (char*)"A");
			point* B = new point(200, 300, 300, (char*)"B");
			point* C = new point(300, 200, 300, (char*)"C");
			point* D = new point(200, 100, 300, (char*)"D");
			point* E = new point(200, 200, 500, (char*)"E");

			v[0] = A;
			v[1] = B;
			v[2] = C;
			v[3] = D;
			v[4] = E;

			
			telo_sides[0] = new side(D, C, B, A);
			telo_sides[1] = new side(B, C, E);
			telo_sides[2] = new side(C, D, E);
			telo_sides[3] = new side(D, A, E);
			telo_sides[4] = new side(A, B, E);
			
			clr = new int* [n+1];
			for (int i = 0; i < n; i++)
			{
				clr[i] = new int[4];
			}

			for (int i = 0; i < n; i++)
				clr[FLAG][i] = 0;

			clr[BR][0] = 9;
			clr[BR][1] = 10;
			clr[BR][2] = 11;
			clr[BR][3] = 12;
			clr[BR][4] = 13;

			clr[DR][0] = 1;
			clr[DR][1] = 2;
			clr[DR][2] = 3;
			clr[DR][3] = 4;
			clr[DR][4] = 5;

			for (int i = 0; i < n; i++)
				clr[VS][i] = 1;
			move(100, 0, -200);
			rotate(30, Y);
		}
	}



	void moveUP()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(0, -MOV_DELTA, 0);
	}
	void moveDW()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(0, MOV_DELTA, 0);
	}

	void moveLE()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(-MOV_DELTA, 0, 0);
	}

	void moveRT()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(MOV_DELTA, 0, 0);
	}

	void moveFR()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(0, 0, -MOV_DELTA);
	}

	void moveTO()
	{
		for (int i = 0; i < n; i++)
			v[i]->move(0, 0, MOV_DELTA);
	}

	void scaleB()
	{
		upd_mid();
		move(-mid->x, -mid->y, -mid->z);
		scale_each(2, 2, 2);
		move(mid->x, mid->y, mid->z);
	}

	void scaleS()
	{
		upd_mid();
		move(-mid->x, -mid->y, -mid->z);
		scale_each(0.5, 0.5, 0.5);
		move(mid->x, mid->y, mid->z);
	}




	void rotXpl()	{
		rotate(10, X);
	}

	void rotXmi()	{
		rotate(-10, X);
	}

	void rotYpl()	{
		rotate(10, Y);
	}

	void rotYmi()	{
		rotate(-10, Y);
	}

	void rotZpl()	{
		rotate(10, Z);
	}

	void rotZmi()	{
		rotate(010, Z);
	}

	void move_light(double _x, double _y, double _z)	{
		light->move(_x, _y, _z);
	}





	void paint()
	{
		upd_mid();
		upd_sides();

		upd_shadow();
		draw_shadow();

		check_signs();
		check_colors();
		check_visibility();

		project();
		draw_fig();
		unproject();
	}

	void changeDrawMode()
	{
		for (int i = 0; i < n; i++)
		{
			shad_sides[i]->toDrawTransparent = !shad_sides[i]->toDrawTransparent;
			telo_sides[i]->toDrawTransparent = !telo_sides[i]->toDrawTransparent;

		}
		if (!shad_sides[0]->toDrawTransparent) toLineLight = false;

	}

	void changeOutlineMode()
	{
		for (int i = 0; i < n; i++)
		{
			telo_sides[i]->toOutline = !telo_sides[i]->toOutline;
		}
	}
	void changeNamingMode()
	{
		for (int i = 0; i < n; i++)
		{
			telo_sides[i]->toNamePoints = !telo_sides[i]->toNamePoints;
		}
	}

	int changetoLineLight()
	{
		if (shad_sides[0]->toDrawTransparent)
		{
			toLineLight = !toLineLight;
			return 0;
		}
		else
		{
			
			toLineLight = false;
			return 1;
		}
	}


};