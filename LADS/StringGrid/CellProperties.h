#ifndef CELLPROPERTIES
#define CELLPROPERTIES

#include <stdlib.h>
#include <vector>

class CellProperties
{
   TColor CellColour;
   bool CellColourSet;
   TColor FontColour;
   bool FontColourSet;
   bool Underline, UnderlineSet;

   typedef std::pair< String, Graphics::TBitmap* > BitMapData;
   std::vector< BitMapData > bitmaps;
   typedef std::vector< BitMapData >::iterator BMDiter;

   BMDiter findBitmap( String fileName );

   static int theCount;

public:

   CellProperties();
   ~CellProperties();

   void SetCellColour(TColor tc);
   TColor GetCellColour() { return CellColour; }
   void DelCellColour();

   void SetFontColour(TColor tc);
   TColor GetFontColour() { return FontColour; }
   void DelFontColour();

   void AddBitmap(Graphics::TBitmap* b, AnsiString bitmap_file);
   void RemoveBitmap(AnsiString bitmap_file);
   bool IsBitmapDisplayed(AnsiString bitmap_file);

   struct BitMapRange
   {
       BMDiter current, last;
       BitMapRange( std::vector< BitMapData > & vec )
        : current( vec.begin() ), last( vec.end() )
       {}
       Graphics::TBitmap* operator*() { return current -> second; }
       operator bool() const { return current != last; }
       BitMapRange & operator++() { ++ current; return *this; }
   };

   void SetFontUnderline();
   bool GetFontUnderline() const { return Underline; }
   void DelFontUnderline();

   bool GetCellColourSet() const { return CellColourSet; }
   bool GetFontColourSet() const { return FontColourSet; }
   bool GetFontUnderlineSet() const { return UnderlineSet; }

   int GetNumBitmaps() const { return bitmaps.size(); }
   BitMapRange GetBitmaps() { return bitmaps; }

   bool CanDelete();
   void RemoveBitmaps();
};


int CellProperties::theCount = 0;

CellProperties::CellProperties()
 : CellColourSet(false), FontColourSet(false), UnderlineSet(false)
{
   theCount++;
}

CellProperties::~CellProperties()
{
   theCount--;
}

bool CellProperties::CanDelete()
{
   return !CellColourSet && !FontColourSet && bitmaps.empty() && !UnderlineSet;
}

void CellProperties::SetCellColour(TColor tc)
{
   CellColour = tc;
   CellColourSet = true;
}

void CellProperties::DelCellColour()
{
   CellColour = clNone;
   CellColourSet = false;
}

void CellProperties::SetFontColour(TColor tc)
{
   FontColour = tc;
   FontColourSet = true;
}

void CellProperties::DelFontColour()
{
   FontColour = clNone;
   FontColourSet = false;
}

void CellProperties::SetFontUnderline()
{
   Underline = true;
   UnderlineSet = true;
}

void CellProperties::DelFontUnderline()
{
   Underline = false;
   UnderlineSet = false;
}

bool CellProperties::IsBitmapDisplayed(AnsiString bitmap_file)
{
    return findBitmap( bitmap_file ) != bitmaps.end();
}

void CellProperties::AddBitmap(Graphics::TBitmap* b, AnsiString bitmap_file)
{
    if( findBitmap( bitmap_file ) == bitmaps.end() )
        bitmaps.push_back( BitMapData( bitmap_file, b ) );
}

void CellProperties::RemoveBitmap(AnsiString bitmap_file)
{
    BMDiter bmi = findBitmap( bitmap_file );
    if( bmi != bitmaps.end() )
        bitmaps.erase( bmi );
}

void CellProperties::RemoveBitmaps()
{
    bitmaps.clear();
}

CellProperties::BMDiter CellProperties::findBitmap(AnsiString bitmap_file)
{
    BMDiter bmi = bitmaps.begin();
    while( bmi != bitmaps.end() && bmi -> first != bitmap_file )
        bmi ++;
    return bmi;
}

#endif



