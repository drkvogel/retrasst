//---------------------------------------------------------------------------

#ifndef pspageH
#define pspageH


/*
Author: Andrew Munday. initial relased into CVS 22-01-2009.

pspage.cpp,.h keeps a record of the printable page area, label details and gaps between
labels. a link to the current stamper is also kept here.

$id$

*/
class PSSize;
class PSRect;
class PSScaler;
class PSStamper;

class PSPageInterface
{
	 protected:
		  virtual PSRect*    getMargins(void )=0;
		  virtual PSStamper* getStamper(void )=0;
		  virtual PSSize*    getPageSize(void )=0;

		  virtual PSRect*    getUnprintable(void)=0;
		  virtual PSSize*    getColRows(void)=0;
		  virtual PSSize*	 getLabelSize( void )=0;
		  virtual PSSize*    getLabelGap(void)=0;
		  virtual PSRect*    getLabelArea(void)=0;
		  virtual PSSize*	 getLabelPitch(void)=0;

		  virtual void       calcLabelSize( void)=0;
		  virtual void		 calcLayout(void )=0;
		  virtual void       nextLabel(  void )=0;
		  virtual void       clear(void )=0;
};

class PSPageBase:public PSPageInterface
{
	 public:
			PSPageBase();
		  virtual PSRect*    getMargins(void );
		  virtual PSStamper* getStamper(void );
		  virtual PSSize*    getPageSize(void );

		  virtual PSRect*    getUnprintable(void);
		  virtual PSSize*    getColRows(void);
		  virtual PSSize*	 getLabelSize( void ){return NULL;}
		  virtual PSSize*    getLabelGap(void);
		  virtual PSRect*    getLabelArea(void);
		  virtual PSSize*	 getLabelPitch(void);
		  virtual void       calcLabelSize( void){};
		  virtual void		 calcLayout(void ){};
		  virtual void       nextLabel(  void ){};
		  virtual void       clear(void ){};
		  virtual ~PSPageBase();
	 protected:
		PSRect*    margins;
		PSRect*    unprintable;
		PSSize*    pageSize;
		PSSize*    colRows;
		PSSize*    labelGap;
		PSSize*    labelSize;
		PSRect*    labelArea;
		PSSize*	   labelPitch;
		PSStamper* stamper;

		int labelCountX;
		int labelCountY;
		int labelCountTotal;
};
class PSPage: public PSPageBase
{
		public:
		  virtual void       calcLabelSize( void);
		  virtual void		 calcLayout(void );
		  virtual void       nextLabel(  void );
		  virtual void       clear(void );

		  PSPage(void ):PSPageBase(){}

};
class PSMSPage: public PSPageBase
{
		public:

		  virtual void       calcLabelSize( void);
		  virtual void		 calcLayout(void );
		  virtual void       nextLabel(  void );
		  virtual void       clear(void );
		  virtual PSSize*	 getLabelSize( void ){return labelSize; }

		  PSMSPage(void ):PSPageBase(){}

};
//---------------------------------------------------------------------------
#endif
