#include <stdbool.h>
#include "logic.h"

int board[3][3] = { 0 };
struct defEnv {
	Display *dsp;
	Window w;
	int par[3];
	int x0;
	int y0;
	GC gcLine;
} defenv;


void drawBoard()
{
	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			defenv.x0, defenv.par[0], defenv.x0, defenv.par[2] - defenv.par[0] );
	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			2*defenv.x0, defenv.par[0], 2*defenv.x0, defenv.par[2] - defenv.par[0] );

	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			defenv.par[0], defenv.y0, defenv.par[1]-defenv.par[0], defenv.y0 );
	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			defenv.par[0], 2*defenv.y0, defenv.par[1]-defenv.par[0], 2*defenv.y0 );
}

int getRow( int cx, int cy )
{
	int ret = 0;
	int offset = defenv.par[0];
	int yinc = defenv.par[2]/3;

	if( cy > defenv.y0 && cy < 2*defenv.y0 )
	{
		ret += (yinc)-offset;
	}
	else if( cy > defenv.y0 )
	{
		ret += (yinc*2)-offset*2;
	}

	return ret;
}

int getCol( int cx, int cy )
{
	int ret = 0;
	int offset = defenv.par[0];
	int xinc = defenv.par[1]/3;

	if( cx > defenv.x0 )
	{
		if( cx < 2*defenv.x0 )
		{
			ret += (xinc)-offset;
		}
		else
		{
			ret += (xinc*2)-offset*2;
		}
	}

	return ret;
}

void drawX( int row, int col )
{
	int offset = defenv.par[0]*2;
	int xmin = col;
	int xmax = col + defenv.x0;
	int ymin = row;
	int ymax = row + defenv.y0;

	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			xmin+offset, ymin+offset, xmax-offset, ymax-offset );
	XDrawLine( defenv.dsp, defenv.w, defenv.gcLine,
			xmax-offset, ymin+offset, xmin+offset, ymax-offset );
}

void drawO( int row, int col )
{
	XDrawArc( defenv.dsp, defenv.w, defenv.gcLine,
			col+20, row+20, defenv.par[1]/4, defenv.par[1]/4, 0, 360*64 );
}

void init()
{
	int offsetC;
	int offsetR;
	for( int c = 0; c < 3; c++ )
	{
		offsetR = defenv.par[1]/3;
		for( int g = 0; g < 3; g++ )
		{
			offsetC = defenv.par[1]/3;
			if( board[g][c] == 1 )
			{
				drawO( g*offsetC, c*offsetR );
			}

			if ( board[g][c] == 2 )
			{
				drawX( g*offsetC, c*offsetR );
			}
			offsetC -= defenv.par[0];
		}
		offsetR -= defenv.par[0];
	}
}

void dbgBoard()
{
	for( int c = 0; c < 3; c++ )
	{
		for( int g = 0; g < 3; g++ )
		{
			printf("%d ", board[c][g]);
		}
		puts("");
	}
}

bool check()
{
	bool r = false;
	for( int c = 0; c < 3; c++ )
	{
		if( board[c][0] != 0 && board[c][0] == board[c][1] && board[c][0] == board[c][2] )
		{
			printf("Win\n");
			r = true;
			break;
		}
		else if( board[0][c] != 0 && board[0][c] == board[1][c] && board[0][c] == board[2][c] )
		{
			printf("Win\n");
			r = true;
			break;
		}
	}

	if( board[0][0] != 0 && board[0][0] == board[1][1] && board[0][0] == board[2][2] )
	{
		printf("Win\n");
		r = true;
	}

	if( board[0][2] != 0 && board[0][2] == board[1][1] && board[0][2] == board[2][0] )
	{
		printf("Win\n");
		r = true;
	}

	return r;
}
