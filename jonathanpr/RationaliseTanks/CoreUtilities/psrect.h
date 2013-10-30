//---------------------------------------------------------------------------

#ifndef PSRectH
#define PSRectH

/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

PSRect - a general class to store and retrieve start and end co-ordinates
	 in ether pixels, millimeters or ratios in the labels.
	 
$id$

*/
class PSRect
{
		public:
				PSRect( void );
				virtual ~PSRect() {}

				virtual void  setStartX( int pStartX );
                virtual void  setStartY( int pStartY );
                virtual void  setEndX( int pEndX );
				virtual void  setEndY( int pEndY );

                virtual int getStartX( void );
                virtual int getStartY( void );
				virtual int getEndX( void );
                virtual int getEndY( void );


                virtual void  setStartXmm( float pStartXmm );
                virtual void  setStartYmm( float pStartYmm );
                virtual void  setEndXmm( float pEndXmm );
				virtual void  setEndYmm( float pEndYmm );

                virtual float getStartXmm( void );
                virtual float getStartYmm( void );
                virtual float getEndXmm( void );
                virtual float getEndYmm( void );


                virtual void  setStartXratio( float pStartXratio );
                virtual void  setStartYratio( float pStartYratio );
                virtual void  setEndXratio( float pEndXratio );
                virtual void  setEndYratio( float pEndYratio );

                virtual void  setDpcm( int pDpcm );
				virtual void  setDpi ( int pDpi );

                virtual void  copy( PSRect *source);
                virtual void  convMMByRatio( PSRect *Ratio ){};
//                virtual PSRect *  convDpi( void ){};
        protected:
				int startX;
				int startY;
				int endX;
				int endY;

				float startXmm;
				float startYmm;
				float endXmm;
				float endYmm;

				float startXratio;
				float startYratio;
				float endXratio;
				float endYratio;

                int dpi;
                int dpcm;

};
//---------------------------------------------------------------------------
#endif
