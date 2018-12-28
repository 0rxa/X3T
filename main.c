#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/cursorfont.h>
#include "game.h"

int main(int argc, char* argv[])
{
	Display *dsp;
	int scr;
	Window w, chr;
	XWindowAttributes wa;
	XEvent xev;

	int par[3] = { 5, 500, 500 };
	dsp = XOpenDisplay(NULL);
	if( dsp == NULL )
	{
		fprintf( stderr, "Cannot open display\n" );
		exit(1);
	}
	scr = DefaultScreen( dsp );
	Window rwin = RootWindow( dsp, scr );

	Colormap cmap = DefaultColormap( dsp, scr );
	XColor line, p1, p2;
	XGCValues gcvLine, gcvP1, gcvP2;
	GC gcLine, gcP1, gcP2;

	XParseColor( dsp, cmap, "rgb:00/00/00", &line );
	XAllocColor( dsp, cmap, &line );
	gcvLine.foreground = line.pixel;
	gcvLine.background = line.pixel;
	gcLine = XCreateGC( dsp, rwin,
			GCForeground | GCBackground, &gcvLine );
	XSetLineAttributes( dsp, gcLine, 7, LineSolid, CapButt, JoinRound );

	w = XCreateSimpleWindow( dsp, rwin,
			0, 0, par[1], par[2],
			par[0], BlackPixel( dsp, scr ),
			WhitePixel( dsp, scr ) );
	XSelectInput( dsp, w, EnterWindowMask | KeyPressMask | ButtonPressMask );
	XMapWindow( dsp, w );

	int x = 0, y = 0;
	int x0 = (par[1]-(2*par[0]))/3;
	int y0 = (par[2] - (2*par[0]))/3;

	bool p = 0;
	while( 1 )
	{
		XNextEvent( dsp, &xev );

		// Get cursor coordinates
		if( xev.type == ButtonPress )
		{
        		XTranslateCoordinates( dsp, w, rwin, 0, 0, &x, &y, &chr );
  			x = xev.xbutton.x_root - x;
  			y = xev.xbutton.y_root - y;
			if( p )
			{
				drawO( dsp, w, gcLine, x, y, par[1]/4, x0, y0 );
			}
			else
			{
				drawX(par[1]/3, par[2]/3, dsp, w, gcLine, x, y, x0, y0);
			}
		}

      		if( xev.xkey.keycode == 9 )
      		{
      			break;
      		}

		drawBord( dsp, w, gcLine, par, x0, y0 );
		p = !p;
	}
}
