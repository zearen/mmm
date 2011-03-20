/*
Copyright (C) 2003  Promit Roy

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _ENDIAN_H
#define _ENDIAN_H


//this file contains definitions for endian functionality
//NOTE: This only fixes endians, and is not complete for all
//architectures. It'll work where short is 2 bytes and int is 4 bytes
//If that's not the case, it'll probably explode.

//a BIG thanks to the Quake 2 source code here

extern bool BigEndianSystem;

void InitEndian( void );		//makes use of a clever trick in Quake 2

short ShortSwap( short s );
short ShortNoSwap( short s );

int LongSwap( int i );
int LongNoSwap( int i );

float FloatSwap( float f );
float FloatNoSwap( float f );


double DoubleSwap( double f );
double DoubleNoSwap( double f );



//Use these functions
extern short (*BigShort) ( short s );
extern short (*LittleShort) ( short s );
extern int (*BigLong) ( int i );
extern int (*LittleLong) ( int i );
extern float (*BigFloat) ( float f );
extern float (*LittleFloat) ( float f );
extern double (*BigDouble) ( double f );
extern double (*LittleDouble) ( double f );



#endif
