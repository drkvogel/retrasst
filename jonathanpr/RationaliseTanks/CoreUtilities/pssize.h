//---------------------------------------------------------------------------

#ifndef pssizeH
#define pssizeH

/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

PSSize - a general class to store and retrieve point co-ordinates
	 in ether pixels or millimeters.

$id$

*/
class PSSize
{
		public:
				PSSize( void );
                void setSizeX( int x );
				void setSizeY( int y );
				int getSizeX( void );
				int getSizeY( void );

				void setSizeXmm( float x );
				void setSizeYmm( float y );
				float getSizeXmm( void );
				float getSizeYmm( void );
				void copy(PSSize* source);

		private:
				int sizeX;
				int sizeY;
				float sizeXmm;
				float sizeYmm;
};

//---------------------------------------------------------------------------
#endif
