#include <stdbool.h>

void drawBord( Display *dsp, Window w, GC gcLine, int *par, int x0, int y0)
{
	XDrawLine( dsp, w, gcLine, x0, par[0], x0, par[2] - par[0] );
	XDrawLine( dsp, w, gcLine, 2*x0, par[0], 2*x0, par[2] - par[0] );

	XDrawLine( dsp, w, gcLine, par[0], y0, par[1]-par[0], y0 );
	XDrawLine( dsp, w, gcLine, par[0], 2*y0, par[1]-par[0], 2*y0 );
}

int getSquare(int x, int y, int x0, int y0)
{
	int ret = 0;

	if( y > y0 && y < 2*y0 )
	{
		ret += 3;
	}
	else if( y > y0 )
	{
		ret += 6;
	}

	if( x < x0 )
	{
		ret += 1;
	}
	else
	{
		if( x < 2*x0 )
		{
			ret += 2;
		}
		else
		{
			ret += 3;
		}
	}

	return ret;
}

void drawX(int xinc, int yinc, Display *dsp, Window w, GC gcLine, int cx, int cy, int x, int y)
{
	int xmin = 0, ymin = 0, xmax = xinc, ymax = yinc;

	if( cy > y && cy < 2*y )
	{
		ymin += (yinc)-5;
		ymax += (yinc)-5;
	}
	else if( cy > y )
	{
		ymin += (yinc*2)-10;
		ymax += (yinc*2)-10;
	}

	if( cx > x )
	{
		if( cx < 2*x )
		{
			xmin += (xinc)-5;
			xmax += (xinc)-5;
		}
		else
		{
			xmin += (xinc*2)-10;
			xmax += (xinc*2)-10;
		}
	}

	XDrawLine( dsp, w, gcLine, xmin+10, ymin+10, xmax-10, ymax-10 );
	XDrawLine( dsp, w, gcLine, xmax-10, ymin+10, xmin+10, ymax-10 );
}

void drawO( Display *dsp, Window w, GC gcLine, int cx, int cy, int par, int bx, int by )
{
	int x = bx/2, y = by/2;

	if( cy > by && cy < 2*by )
	{
		y += (by)-5;
	}
	else if( cy > by )
	{
		y += (by*2)-10;
	}

	if( cx > bx )
	{
		if( cx < 2*bx )
		{
			x += (bx)-5;
		}
		else
		{
			x += (bx*2)-10;
		}
	}

	XDrawArc( dsp, w, gcLine, x-(par/2), y-(par/2), par, par, 0, 360*64 );
}
