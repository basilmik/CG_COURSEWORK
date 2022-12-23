#pragma once
#include "../libwinbgi/src/graphics.h"
#include <stdio.h>
#include "point.h"

#define X 0
#define Y 1
#define Z 2

class side
{
	float A, B, C, D;
	int N;
	point** v;
	point* mid;


	// from lab5 1 semester
	void colour(float _xs, float _ys, float _x2, float _y2, float _x3, float _y3, int color)
	{
		setcolor(color);
		float xs = 0, ys = 0, x2 = 0, y2 = 0, x3 = 0, y3 = 0;
		if (_x2 <= _x3)
		{
			if (_xs <= _x2) // xs x2 x3
			{
				xs = _xs; 
				ys = _ys; 
				x2 = _x2;  
				y2 = _y2; 
				x3 = _x3;  
				y3 = _y3;
			}
			else
				if (_xs <= _x3) // x2 xs x3
				{
					xs = _x2; 
					ys = _y2; 
					x2 = _xs;  
					y2 = _ys; 
					x3 = _x3;  
					y3 = _y3;
				}
				else // x2 x3 xs
				{
					xs = _x2; 
					ys = _y2; 
					x2 = _x3;  
					y2 = _y3; 
					x3 = _xs;  
					y3 = _ys;
				}
		}
		else // x3 x2
		{
			if (_xs <= _x3) // xs x3 x2
			{
				xs = _xs; 
				ys = _ys; 
				x2 = _x3;  
				y2 = _y3; 
				x3 = _x2;  
				y3 = _y2;
			}
			else
				if (_xs <= _x2) // x3 xs x2
				{
					xs = _x3; 
					ys = _y3; 
					x2 = _xs;  
					y2 = _ys; 
					x3 = _x2;  
					y3 = _y2;
				}
				else  // x3 x2 xs
				{
					xs = _x3; 
					ys = _y3; 
					x2 = _x2;  
					y2 = _y2; 
					x3 = _xs;  
					y3 = _ys;
				}
		}

		float y1d, y2d;
		float point = xs--;
		point = ceil(point);
		float k1 = ((x2 <= x3 ? y2 : y3) - ys) / ((x2 <= x3 ? x2 : x3) - xs),
			k2 = ((x2 > x3 ? y2 : y3) - ys) / ((x2 > x3 ? x2 : x3) - xs);
		float b1 = (ys - k1 * xs),
			b2 = (ys - k2 * xs);

		while (point <= (x2 < x3 ? x2 : x3))
		{
			y1d = k1 * point + b1;
			if (point == round(xs))
				y1d = ys;

			y2d = k2 * point + b2;


			if (toDrawTransparent)
			{
				if ((int)point % 2 == 0 && (int)point % 3 == 0)
					line(point, y1d, point, y2d);
			}
			else
				line(point, y1d, point, y2d);

			point++;
		}

		if (x2 != x3)
		{
			k1 = ((x2 >= x3 ? y2 : y3) - (x2 <= x3 ? y2 : y3)) / ((x2 >= x3 ? x2 : x3) - (x2 <= x3 ? x2 : x3));
			b1 = ((x2 < x3 ? y2 : y3) - k1 * (x2 < x3 ? x2 : x3));

			while (point <= (x2 > x3 ? x2 : x3))
			{
				y1d = k1 * (point)+b1;
				if (point == round(xs)) y1d = ys;

				y2d = k2 * point + b2;

				if (toDrawTransparent)
				{
					if ((int)point % 2 == 0 && (int)point % 3 == 0)
						line(point, y1d, point, y2d);
				}
				else
					line(point, y1d, point, y2d);

				point++;
			}
		}
	}

public:

	bool toDrawTransparent;
	bool toOutline;
	bool toNamePoints;
	side()
	{
		for (int i = 0; i < 3; i++)
			v[i] = new point;
		mid = new point;
	}

	side(int _n)
	{
		N = _n;
		v = new point * [N];
		for (int i = 0; i < N; i++)
			v[i] = new point;
		mid = new point;
	}

	side(point* a, point* b, point* c)
	{
		N = 3;
		v = new point * [3];
		v[0] = a;
		v[1] = b;
		v[2] = c;
		mid = new point;
		toDrawTransparent = false;
		toOutline = false;
		toNamePoints = true;
	}
	side(point* a, point* b, point* c, point * d)
	{
		N = 4;
		v = new point * [4];
		v[0] = a;
		v[1] = b;
		v[2] = c;
		v[3] = d;
		mid = new point;
		toDrawTransparent = false;
		toOutline = false;
		toNamePoints = true;
	}
	void upd_mid()
	{
		mid->x = 0;mid->y = 0;mid->z = 0;
		for (int i = 0; i < N; i++)
		{
			mid->x += v[i]->x;
			mid->y += v[i]->y;
			mid->z += v[i]->z;
		}
		mid->x /= (float)N;
		mid->y /= (float)N;
		mid->z /= (float)N;

		mid->w = 1;
	}

	void reset(point* a, point* b, point* c)
	{
		v[0] = a;
		v[1] = b;
		v[2] = c;
	}
	void reset(point* a, point* b, point* c, point* d)
	{
		v[0] = a;
		v[1] = b;
		v[2] = c;
		v[3] = d;
	}

	void rotate()
	{	
		for (int i = 0; i < 3; i++)
			v[i]->rotate(30, X);
	}

	void color(int c)
	{
		setlinestyle(0, 1, 1);
		if (N == 3) {
			colour(v[0]->x, v[0]->y, v[1]->x, v[1]->y, v[2]->x, v[2]->y, c);

			if (toOutline)
			{
				setcolor(15);
				setlinestyle(0, 1, 3);
				line(v[0]->x, v[0]->y, v[1]->x, v[1]->y);
				line(v[0]->x, v[0]->y, v[2]->x, v[2]->y);
				line(v[2]->x, v[2]->y, v[1]->x, v[1]->y);
			}
		}

		if (N == 4)
		{
			colour(v[1]->x, v[1]->y, v[0]->x, v[0]->y, v[2]->x, v[2]->y, c);
			colour(v[3]->x, v[3]->y, v[0]->x, v[0]->y, v[2]->x, v[2]->y, c);

			if (toOutline)
			{
				setcolor(15);
				setlinestyle(0, 1, 3);
				line(v[0]->x, v[0]->y, v[1]->x, v[1]->y);
				line(v[1]->x, v[1]->y, v[2]->x, v[2]->y);
				line(v[2]->x, v[2]->y, v[3]->x, v[3]->y);
				line(v[3]->x, v[3]->y, v[0]->x, v[0]->y);
			}
		}
		if (toNamePoints)
		printNames();
	}

	void printNames()
	{
		for (int i = 0; i < N; i++)
		{
			v[i]->printName((v[i]->x > mid->x), (v[i]->y > mid->y));
		}
	}

	void calc_coefs(int i)
	{
		float M[3] = { v[i]->x, v[i]->y, v[i]->z };
		int ai = i - 1;
		if (ai < 0)	ai = 2;
		int bi = i + 1;
		if (bi > 2)	bi = 0;

		float a[3] = { v[ai]->x - v[i]->x, v[ai]->y - v[i]->y, v[ai]->z - v[i]->z };
		float b[3] = { v[bi]->x - v[i]->x, v[bi]->y - v[i]->y, v[bi]->z - v[i]->z };

		A = a[Y] * b[Z] - a[Z] * b[Y];
		B = a[X] * b[Z] - a[Z] * b[X];
		B = -B;
		C = a[X] * b[Y] - a[Y] * b[X];

		D = M[X] * A + M[Y] * B + M[Z] * C;
		D = -D;
	}


	int sign(point* p)
	{
		return (A * p->x + B * p->y + C * p->z + D <= 0);
	}

	void swap_sign()
	{
		A = -A;	B = -B;	C = -C;	D = -D;
	}

	int isSeen(point* p)
	{
		return (A * p->x + B * p->y + C * p->z <= 0);
	}
};