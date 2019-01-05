#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/cursorfont.h>
#include <string.h>
#include "game.h"

int main(int argc, char* argv[])
{
	int scr;
	Window chr, cw;
	XEvent xev;

	int *tmp = malloc(3*sizeof(int));
	tmp[0] = 5;
	tmp[1] = 500;
	tmp[2] = 500;
	memcpy( defenv.par, tmp, 3*sizeof(int) );
	free(tmp);

	defenv.x0 = (defenv.par[1] - (2*defenv.par[0]))/3; //Specifies x value for vertical board bars
	defenv.y0 = (defenv.par[2] - (2*defenv.par[0]))/3; //Specifies y value for horizontal board bars

	defenv.dsp = XOpenDisplay(getenv("Display"));
	if( defenv.dsp == NULL )
	{
		fprintf( stderr, "Cannot open display\n" );
		exit(1);
	}
	scr = DefaultScreen( defenv.dsp );
	Window rwin = RootWindow( defenv.dsp, scr );

	Colormap cmap = DefaultColormap( defenv.dsp, scr );
	XColor line;
	XGCValues gcvLine;

	XParseColor( defenv.dsp, cmap, "rgb:00/00/00", &line );
	XAllocColor( defenv.dsp, cmap, &line );
	gcvLine.foreground = line.pixel;
	gcvLine.background = line.pixel;
	defenv.gcLine = XCreateGC( defenv.dsp, rwin,
			GCForeground | GCBackground, &gcvLine );
	XSetLineAttributes( defenv.dsp, defenv.gcLine, 7, LineSolid, CapButt, JoinRound );

	defenv.w = XCreateSimpleWindow( defenv.dsp, rwin,
			0, 0, defenv.par[1], defenv.par[2] + defenv.par[2]/5,
			defenv.par[0], BlackPixel( defenv.dsp, scr ),
			WhitePixel( defenv.dsp, scr ) );
	XSelectInput( defenv.dsp, defenv.w, ExposureMask | StructureNotifyMask | KeyPressMask | ButtonPressMask );
	XMapWindow( defenv.dsp, defenv.w );

	cw = XCreateSimpleWindow( defenv.dsp, defenv.w,
			0, defenv.par[1], defenv.par[1], defenv.par[1]/5,
			1, BlackPixel( defenv.dsp, scr ),
			BlackPixel( defenv.dsp, scr ) );
	XSelectInput( defenv.dsp, defenv.w, ExposureMask | StructureNotifyMask | KeyPressMask | ButtonPressMask );
	XMapWindow( defenv.dsp, cw );

	int x = 0, y = 0, r, c; //Stores the mouse coordinates
	int div = defenv.par[1]/3-defenv.par[0];

	int rx, ry, rwidth, rheight, rbwidth, rdepth;

	bool p = 0;
	int n = 0;
	while( 1 )
	{
		XNextEvent( defenv.dsp, &xev );

		if( xev.type == ButtonPress )
		{
        		XTranslateCoordinates( defenv.dsp, defenv.w, rwin, 0, 0, &x, &y, &chr );
  			x = xev.xbutton.x_root - x;
  			y = xev.xbutton.y_root - y;

			r = getRow( x, y );
			c = getCol( x, y );

			if( board[r/div][c/div] )
			{
				continue;
			}

			if( p )
			{
				drawX( r, c );
			}
			else
			{
				drawO( r, c );
			}

			board[r/div][c/div] = p+1;
			if( check() )
			{
				fprintf( stdout, "Player %d Wins!\n", p+1 );
				break;
			}
			p = !p;
			n++;
			if( n == 9 )
			{
				printf("Draw\n");
				break;
			}
		}

		if( xev.type == Expose )
		{
			system("clear");
			init();
		}

      		if( xev.xkey.keycode == 9 )
      		{
      			break;
      		}

		drawBoard();
	}
}
