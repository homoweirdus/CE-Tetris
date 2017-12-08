#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <tice.h>

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "gfx/blockone.h"
#include <graphx.h>
/* Include the graphics */
#include "gfx/tiles_gfx.h"

/* Include the external map data */
extern uint8_t tilemap_map[];

/* Tilemap defines */
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define TILE_WIDTH          16
#define TILE_HEIGHT         16

#define TILEMAP_WIDTH       19
#define TILEMAP_HEIGHT		14

#define TILEMAP_DRAW_WIDTH  19
#define TILEMAP_DRAW_HEIGHT 14

#define Y_OFFSET            0
#define X_OFFSET            0
#define ONE_SECOND         32768/1
//defining functions
void piecedraw(signed int *xarr, signed int *yarr, int ex, int why);
void setp(signed int *xar, signed int *yar, int num);
void main(void) {
	//assiginign a bunch of variables
    sk_key_t key;
	int alive = 1;
    unsigned int x = 160;
    unsigned int y = 0;
	//b1x and b1y are a reminant of an older system of storing variables i used, so i repurposed them for 1 thing i could easly do with a function but im too lazy
	signed int b1x = 0;
	signed int b1y = 0;
	// i could make all of the y arrays only 3 elements long, and reduce every acess to them by 1 thing but i cant be bothered
	//i do use element 0 of the x array for some storage though
	//xlist and ylist are the current tile, hx, and hy are the current stored piece (i could only use 1 byte but that would slow it down
	//nxl and nyl are the next piece, nxlt and nylt are the 2nd next piece and nxll and nyll are the 3rd next piece
	signed int xlist[4];
	signed int ylist[4];
	signed int hx[] = { 8,0,0,0 };
	signed int hy[4];
	signed int nxl[4];
	signed int nyl[4];
	signed int nxlt[4];
	signed int nylt[4];
	signed int nxll[4];
	signed int nyll[4];

	//int debugnum;
	unsigned int score = 0;
	/*ok good lord theres a lot to say
	curline is used for reading from lines, so is current
	linenow and curx are for setting tiles
	er is a for loop variable
	hh and seconds are misc variables really
	temp is what i use whenever i need to temporarally assign a variable
	linelist is a list of lines to clear*/
	int curline = 13;
	uint8_t current = 0x01;
	int er = 0;
	int linenow;
	int curx;
	int hh = 0;
	int linelist[] = { 0,0,0,0 };
	unsigned int seconds = 0;
	signed int temp;
    gfx_tilemap_t tilemap;
	
    /* Initialize the tilemap structure */
    tilemap.map         = tilemap_map;
    tilemap.tiles       = set_tiles;
    tilemap.type_width  = gfx_tile_16_pixel;
    tilemap.type_height = gfx_tile_16_pixel;
    tilemap.tile_height = TILE_HEIGHT;
    tilemap.tile_width  = TILE_WIDTH;
    tilemap.draw_height = TILEMAP_DRAW_HEIGHT;
    tilemap.draw_width  = TILEMAP_DRAW_WIDTH;
    tilemap.height      = TILEMAP_HEIGHT;
    tilemap.width       = TILEMAP_WIDTH;
    tilemap.y_loc       = Y_OFFSET;
    tilemap.x_loc       = X_OFFSET;
	srand(rtc_Time());
	setp(xlist, ylist, 7);
	setp(nxll, nyll, 7);
	setp(nxlt, nylt, 7);
	setp(nxl, nyl, 7);
    /* Initialize the 8bpp graphics and timer stuff */
    gfx_Begin();
	timer_Control = TIMER1_DISABLE;
	timer_1_ReloadValue = timer_1_Counter = ONE_SECOND;
    /* Set up the palette */
    gfx_SetPalette(tiles_gfx_pal, sizeof_tiles_gfx_pal, 0);
    gfx_SetColor(0x04);
	gfx_SetTransparentColor(0x00);
    /* Draw to buffer to avoid tearing */
    gfx_SetDrawBuffer();

    /* Set monospace font with width of 8 */
    gfx_SetMonospaceFont(8);
	timer_Control = TIMER1_ENABLE | TIMER1_32K | TIMER1_0INT | TIMER1_DOWN;
    /* Wait for the enter key to quit */
    while ((key = os_GetCSC()) != sk_Yequ && alive == 1) {
		//when the timer goes off it moves the piece down, or locks it if needed
		if (timer_IntStatus & TIMER1_RELOADED)
		{
			if (gfx_GetTile(&tilemap, x, y + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1] + 16) != 0x01&& gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2] + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3] + 16) != 0x01 && y + 16 != 224 && ylist[1] + y + 16 != 224 && ylist[2] + y + 16 != 224 && ylist[3] + y + 16 != 224)
			{
				y += 16;
			}
			else
			{
				key = 0x3A;
			}
			timer_IntAcknowledge = TIMER1_RELOADED;
		}
        /* Draw tilemap and coords */
        gfx_Tilemap(&tilemap, 0, 0);
		//draw the score counter
		gfx_FillRectangle(0, 224, 320, 16);
		gfx_SetTextXY(0, 224);
		gfx_PrintInt(score, 6);
		gfx_SetTextXY(0, 0);
		/*gfx_FillRectangle(x, y, 16, 16);
		gfx_FillRectangle(x + xlist[1], y + ylist[1], 16, 16);
		gfx_FillRectangle(x + xlist[2], y + ylist[2], 16, 16);
		gfx_FillRectangle(x + xlist[3], y + ylist[3], 16, 16);
		gfx_FillRectangle(16, 16, 16, 16);
		gfx_FillRectangle(16 + nxl[1], 16 + nyl[1], 16, 16);
		gfx_FillRectangle(16 + nxl[2], 16 + nyl[2], 16, 16);
		gfx_FillRectangle(16 + nxl[3], 16 + nyl[3], 16, 16);*/
		//drawing stuff
		piecedraw(xlist, ylist, x, y);
		piecedraw(nxl, nyl, 16, 16);
		piecedraw(nxlt, nylt, 272, 16);
		piecedraw(nxll, nyll, 272, 96);
		//doesnt draw anything in the hold spot unless there is a piece in hold
		if (hx[0] != 8)
		{
			/*gfx_FillRectangle(16, 96, 16, 16);
			gfx_FillRectangle(16 + hx[1], 96 + hy[1], 16, 16);
			gfx_FillRectangle(16 + hx[2], 96 + hy[2], 16, 16);
			gfx_FillRectangle(16 + hx[3], 96 + hy[3], 16, 16);*/
			piecedraw(hx, hy, 16, 96);
		}
		//gfx_TransparentSprite(one, x, y);

        /* Do something based on the keypress */
        switch (key) {
		case sk_2:
		case sk_Down:
			//if there isnt a block in the way move down, otherwise lock
			if (gfx_GetTile(&tilemap, x, y + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1] + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2] + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3] + 16) != 0x01 && y + 16 != 224 && ylist[1] + y + 16 != 224 && ylist[2] + y + 16 != 224 && ylist[3] + y + 16 != 224)
			{
				y += 16;
				score++;
			}
			else
			{
				key = 0x3A;
			}
			break;
			case sk_4:
            case sk_Left:
				//if you can move left do it
				if (gfx_GetTile(&tilemap, x - 16, y) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1] - 16, y + ylist[1]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[2] - 16, y + ylist[2]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3] - 16, y + ylist[3]) != 0x01 && gfx_GetTile(&tilemap, x - 16, y) != 0x02 && gfx_GetTile(&tilemap, x + xlist[1] - 16, y + ylist[1]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[2] - 16, y + ylist[2]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[3] - 16, y + ylist[3]) != 0x02)
				{
					x -= 16;
				}
                break;
			case sk_6:
            case sk_Right:
				//right
				if (gfx_GetTile(&tilemap, x + 16, y) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1] + 16, y + ylist[1]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[2] + 16, y + ylist[2]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3] + 16, y + ylist[3]) != 0x01 && gfx_GetTile(&tilemap, x + 16, y) != 0x02 && gfx_GetTile(&tilemap, x + xlist[1] + 16, y + ylist[1]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[2] + 16, y + ylist[2]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[3] + 16, y + ylist[3]) != 0x02)
				{
					x += TILE_WIDTH;
				}
                break;
			case sk_8:
			case sk_7:
            case sk_Up:
				//rotation
				temp = ylist[1];
				ylist[1] = -1*xlist[1];
				xlist[1] = temp;
				temp = ylist[2];
				ylist[2] = -1*xlist[2];
				xlist[2] = temp;
				temp = ylist[3];
				ylist[3] = -1*xlist[3];
				xlist[3] = temp;
				//if nothing is in a wall after the rotation keep it otherwise change it back
				if (gfx_GetTile(&tilemap, x, y) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3]) != 0x01 && gfx_GetTile(&tilemap, x, y) != 0x02 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3]) != 0x02)
				{

				}
				else
				{
					temp = xlist[1];
					xlist[1] = -1 * ylist[1];
					ylist[1] = temp;
					temp = xlist[2];
					xlist[2] = -1 * ylist[2];
					ylist[2] = temp;
					temp = xlist[3];
					xlist[3] = -1 * ylist[3];
					ylist[3] = temp;
				}
                break;
			case sk_9:
				//same as other but the other way
				temp = xlist[1];
				xlist[1] = -1 * ylist[1];
				ylist[1] = temp;
				temp = xlist[2];
				xlist[2] = -1 * ylist[2];
				ylist[2] = temp;
				temp = xlist[3];
				xlist[3] = -1 * ylist[3];
				ylist[3] = temp;
				

				if (gfx_GetTile(&tilemap, x, y) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2]) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3]) != 0x01 && gfx_GetTile(&tilemap, x, y) != 0x02 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2]) != 0x02 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3]) != 0x02)
				{

				}
				else
				{
					temp = ylist[1];
					ylist[1] = -1 * xlist[1];
					xlist[1] = temp;
					temp = ylist[2];
					ylist[2] = -1 * xlist[2];
					xlist[2] = temp;
					temp = ylist[3];
					ylist[3] = -1 * xlist[3];
					xlist[3] = temp;
				}
				break;
			case sk_Enter:
				//keeps going down until it cant then locks
				while (gfx_GetTile(&tilemap, x, y + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[1], y + ylist[1] + 16) != 0x01&& gfx_GetTile(&tilemap, x + xlist[2], y + ylist[2] + 16) != 0x01 && gfx_GetTile(&tilemap, x + xlist[3], y + ylist[3] + 16) != 0x01 && y + 16 != 224 && ylist[1] + y + 16 != 224 && ylist[2] + y + 16 != 224 && ylist[3] + y + 16 != 224)
				{
					y += 16;
					score += 2;
				}
				key = 0x3A;
			case 0x3A:
				//the fat thign that i should probably out in a function but cant be bothered setting algorithm
				/*setting the block in place*/
				gfx_SetTile(&tilemap, x, y, 0x01);
				gfx_SetTile(&tilemap, x + xlist[1], y + ylist[1], 0x01);
				gfx_SetTile(&tilemap, x + xlist[2], y + ylist[2], 0x01);
				gfx_SetTile(&tilemap, x + xlist[3], y + ylist[3], 0x01);
				//finding the lowest tile
				b1x = MAX(0, ylist[1]);
				b1y = MAX(ylist[2], ylist[3]);
				temp = MAX(b1x, b1y);
				curline = (y + temp) / 16;
				current = 0x01;
				/*checks to see if you have cleared lines*/
				while (seconds < 4)
				{
					/*for loop makes sure that all the tiles are solid*/
					for (er = 4; er < 15 && current == 0x01; er++)
					{
						current = gfx_GetTile(&tilemap, er*16, curline*16);
							//stops the for loop if there i sa gap
							if (current != 0x01)
							{
								break;
							}
							//gfx_PrintInt(er, 4);
							//dbg_sprintf(dbgout, "for loop er var: %d, current value: %u\n", er, current);
					}
					
					//if the line was full than add it to the list
					if (er == 15)
					{
						//gfx_PrintStringXY("test", 0, 0);
							linelist[hh] = curline;
							hh++;
					}
					//dbg_sprintf(dbgout, "er: %d current: %u curline: %d\n", er, current, curline);
					er = 5;
					current = 0x01;
					//go up some more
					if (curline > 0 && curline != 14)
					{
						curline--;
					}
					else
					{
						curline = 14;
					}
					seconds++;
				}
				hh = 0;
				//resetting variables
				current = 0x01;
				curline = linelist[0];
				//if lines need to be cleared do this
				if (curline > 0)
				{
					//for loop runs through all the lines curline is the line being read from and linenow is the line being written to
					for (linenow = linelist[0]; linenow >= 0; linenow--)
					{
					//moves up the line being read from
						if (curline > 0 && curline != 14)
						{
							curline--;
						}
						//noves up even mroe if the line needs to be cleared
						while (curline > 0 && curline != 14 &&  (curline == linelist[0] || curline == linelist[1] || curline == linelist[2] || curline == linelist[3]))
						{
							curline--;
						}
						if (curline == 0)
						{
							curline = 14;
						}
						//for loops runs through all the x values on the board
						for (curx = 4; curx < 15; curx++)
						{
							//sets the tile at (curx,linenow) to the value of (curx,curline)
							gfx_SetTile(&tilemap, curx*16, linenow*16, (gfx_GetTile(&tilemap, curx*16, curline*16)));
						}
						
						
					}
					//score
					if (linelist[3] > 0)
					{
						score += 800;
					}
					else if (linelist[2] > 0)
					{
						score += 500;
					}
					else if (linelist[1] > 0)
					{
						score += 300;
					}
					else if (linelist[0] > 0)
					{
						score += 100;
					}
					//resetting vars
					linelist[0] = 0;
					linelist[1] = 0;
					linelist[2] = 0;
					linelist[3] = 0;
					seconds = 0;
					curline = 13;
					er = 5;
				}
				x = 160;
				y = 0;
				//moves on the current and next pieces
				setp(xlist, ylist, nxl[0]);
				setp(nxl, nyl, nxlt[0]);
				setp(nxlt, nylt, nxll[0]);
				setp(nxll, nyll, 7);
				seconds = 0;
				//if a tile reaches the top game over
				for (er = 4; er < 15 && alive == 1; er++)
				{
					current = gfx_GetTile(&tilemap, er * 16, 0);
					//stops the while loop if it finds a gap
					if (current == 0x01)
					{
						alive = 0;
					}
					//gfx_PrintInt(er, 4);
					//dbg_sprintf(dbgout, "for loop er var: %d, current value: %u\n", er, current);
				}
				break;
				case sk_Store:
					//storing pieces
					//stores the old value of the piece to temp
					temp = hx[0];
					//sets the hold piece to the current piece
					setp(hx, hy, xlist[0]);
					//if there is something stored currently, set it to the current piece, else next piece
					if (temp != 8)
					{
						setp(xlist, ylist, temp);
					}
					else
					{
						setp(xlist, ylist, nxl[0]);
						setp(nxl, nyl, nxlt[0]);
						setp(nxlt, nylt, nxll[0]);
						setp(nxll, nyll, 7);
					}
					x = 160;
					y = 0;
					break;
            default:
                break;
        }
        gfx_SwapDraw();
    }
	//game over
	gfx_FillScreen(0x02);
	gfx_PrintStringXY("game over. score:", 76, 112);
	gfx_PrintUInt(score, 6);
	gfx_PrintStringXY("press enter to exit", 152, 128);
	gfx_SwapDraw();
	while ((key = os_GetCSC()) != sk_Enter)
	{
	}
    /* Close the graphics and return to the OS */
    gfx_End();
}
void setp(signed int *xar, signed int *yar, int num)
{
	//if you set the piece no to 7 randomize it
	if (num == 7)
	{
		num = rand() % 7;
	}
	/*making the new tile from a random number*/
	switch (num) {
	case 0:
		/*2
		13
		4*/
		xar[1] = 0;
		yar[1] = -16;
		xar[2] = 16;
		yar[2] = 0;
		xar[3] = 0;
		yar[3] = 16;
		break;
	case 1:
		/*2
		1
		3
		4*/
		xar[1] = 0;
		xar[2] = 0;
		xar[3] = 0;
		yar[1] = -16;
		yar[2] = 16;
		yar[3] = 32;
		break;
	case 2:
		/*
		12
		34*/
		xar[1] = 16;
		xar[2] = 0;
		xar[3] = 16;
		yar[1] = 0;
		yar[2] = 16;
		yar[3] = 16;
		break;
	case 3:
		/*
		23
		1
		4*/
		xar[1] = 0;
		yar[1] = -16;
		yar[2] = -16;
		xar[2] = 16;
		xar[3] = 0;
		yar[3] = 16;
		break;
	case 4:
		/*
		32
		1
		4*/
		xar[1] = 0;
		xar[2] = -16;
		xar[3] = 0;
		yar[1] = -16;
		yar[2] = -16;
		yar[3] = 16;
		break;
	case 5:
		/*32
		14*/
		xar[1] = 0;
		xar[2] = -16;
		xar[3] = 16;
		yar[1] = -16;
		yar[2] = -16;
		yar[3] = 0;
		break;
	case 6:
		/*3
		12
		4*/
		xar[1] = 16;
		xar[2] = 0;
		xar[3] = 16;
		yar[1] = 0;
		yar[2] = -16;
		yar[3] = 16;
		break;
	default:
		break;
	}
	xar[0] = num;
}
//int setr(int arr[4], int cl,)
void piecedraw(signed int *xarr, signed int *yarr, int ex, int why)
{
	//draws the tile
	gfx_FillRectangle(ex, why, 16, 16);
	gfx_FillRectangle(ex + xarr[1], why + yarr[1], 16, 16);
	gfx_FillRectangle(ex + xarr[2], why + yarr[2], 16, 16);
	gfx_FillRectangle(ex + xarr[3], why + yarr[3], 16, 16);
}