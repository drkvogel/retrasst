#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <math.h>
#include <printers.hpp>
#include <clipbrd.hpp>
#include "xbasic.h"
#include "mess.h"
#include "utils.h"
#include "xmd5.h"
#include "help.h"
//---------------------------------------------------------------------------
				/* ARE STRINGS EQUAL DISREGARDING CASE */
bool streq_nocase( char *a, char *b )
{	while ( EOS != *a || EOS != *b )
		{if ( tolower( *a ) != tolower( *b ) )
			{return( false );
			}
		a++;
		b++;
		}
	return( true );
}
//---------------------------------------------------------------------------
std::string int2string( const int n )
{
	char	nbuf[30];
	sprintf( nbuf, "%d", n );
	return( std::string( nbuf ) );
}
//---------------------------------------------------------------------------
/*
int load_items( TStrings *it, char **txt )
{
	while ( *txt != NULL )
		{if ( *txt != '\0' )
			{it->Add( Translate( *txt ) );
			}
		txt++;
		}
	return( it->Count );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			// GET TEXT ASSOCIATED WITH A RADIOGROUP SELECTION
String get_cbtext( TComboBox *c )
{	int	indx = c->ItemIndex;
	return( ( indx >= 0 && indx < c->Items->Count )
		? Translate( c->Items->Strings[indx] ) : String("") );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			// GET TEXT ASSOCIATED WITH A RADIOGROUP SELECTION
String get_rgtext( TRadioGroup *r )
{	int	indx = r->ItemIndex;
	return( ( indx >= 0 && indx < r->Items->Count )
		? Translate( r->Items->Strings[indx] ) : String("") );
}
*/
//---------------------------------------------------------------------------
				/* CAPITALISE INITIAL LETTERS OF WORDS */
void capitalise_words( std::string *s )
{	int	i, len;
	char	last = ' ';
	const	char	*s1;
	char	*s2;
	s1 = s->c_str();
	len = strlen( s1 );
	s2 = (char *) malloc( len + 2 );
	for ( i = 0; i < len; i++ )
		{if ( ! ( isalpha( last ) || '\'' == last )
				|| ( 3 == i && 0 == strncmp( s2, "Mac", 3 ) )
				|| ( 2 == i && 0 == strncmp( s2, "Mc",  2 ) )
				)
			{s2[i] = (char) toupper( s1[i] );
			}
		else
			{s2[i] = (char) tolower( s1[i] );
			}
		last = s1[i];
		}
	s2[i] = EOS;
	*s = s2;
	XFREE( s2 );
}
//---------------------------------------------------------------------------
void string_tolower( char *s )
{	if ( NULL == s )
		{return;
		}
	while ( EOS != *s )
		{*s = (char) tolower( *s );
		s++;
		}
}
//---------------------------------------------------------------------------
					/* MAKE A DUPLICATE OF A STRING */
char *string_duplicate( char *in )
{
	if ( NULL == in )
		{return( NULL );
		}
	char	*stmp = (char *) malloc( strlen( in ) + 1 );
	strcpy( stmp, in );
	return( stmp );
}
//---------------------------------------------------------------------------
			/* CONVERT A DOS FILE-PATH TO A WEB FILE-URL */
void filepath_to_url( char *url, const char *path )
{
	int	i;
	char	c = '=';
	bool	found_dot = false;
	strcpy( url, "file:///" );
	int	ulen = strlen( url );
	int	plen = strlen( path );
	if ( plen > 100 )
		{std::string	tmsg = std::string( "Unable to convert Path->URL: " )
			+ std::string( path ) + std::string ( " - too long" );
		MESS::bug( tmsg.c_str() );
		}
	for ( i = 0; i < plen; i++ )
		{switch( path[i] )
			{case ':':
				c = VERT_BAR;
				break;
			case BSLASH:
				c = FSLASH;
				break;
			default:
				c = path[i];
				if ( DOT == c )
					{found_dot = true;
					}
				break;
			}
		url[ ulen + i ] = c;
		}
	if ( FSLASH != c && ! found_dot )
		{url[ ulen + plen ] = FSLASH;	/* ADD / IF NOT ACTUAL FILE */
		ulen++;
		}
	url[ ulen + plen ] = EOS;
}
//---------------------------------------------------------------------------
			/* SET RADIO GROUP TO A SPECIFIC INDEX VALUE */
void set_rgindex( TRadioGroup *trg, int indx )
{
	if ( indx >= -1 && indx < trg->Items->Count )
		{trg->ItemIndex = indx;
		}
}
//---------------------------------------------------------------------------
		/* IS RADIO-GROUP INDEX IN A PARTICULAR RANGE (INCLUSIVE) */
bool radio_inrange( TRadioGroup *r, int imin, int imax )
{	return( r->ItemIndex >= imin && r->ItemIndex <= imax );
}
//---------------------------------------------------------------------------
				/* RETURN WHOLE-DAYS PART OF DATETIME */
TDateTime whole_date( TDateTime t )
{
	double	d = floor( (double) t );
	return( (TDateTime) d );
}
//---------------------------------------------------------------------------
bool file_exists( char *filename )
{
	return( ( NULL != filename ) && 0 == access( filename, 0) );
}
//---------------------------------------------------------------------------
				/* PUT NEXT FORM IN POSITION OF CURRENT ONE */
void overlay( TForm *current, TForm *replacement )
{
	if ( current != replacement )
		{replacement->Top = current->Top;
		replacement->Left = current->Left;
		replacement->Show();
		current->Hide();
		}
	Application->ProcessMessages();
}
//---------------------------------------------------------------------------
					/* EMPTY AND BLANK A STRING GRID */
void sg_empty( TStringGrid *sg )
{
	int	fr = sg->FixedRows;
	sg->RowCount = fr + 1;
	sg->Rows[fr]->Clear();
}
//---------------------------------------------------------------------------
					/* REMOVE A ROW FROM A STRING GRID */
int sg_remove_row( TStringGrid *sg, int delete_row, int numbered_col )
{	int	i, j, k;
	int	ndata_rows;
	int	nr = sg->RowCount;
	int	nc = sg->ColCount;
	int	fr = sg->FixedRows;
	if ( delete_row < fr || delete_row >= nr )
		{MESS::bug( "sg_remove_row" );
		return( 0 );
		}
	nr--;
	for ( i = delete_row; i < nr; i++ )
		{j = i+1;
		for ( k = 0; k < nc; k++ )	// SHUFFLE UP
			{sg->Cells[k][i] = sg->Cells[k][j];
			}
		}
	sg->Rows[nr]->Clear();   		// CLEAR LAST LINE
	ndata_rows = nr - fr;
	if ( ndata_rows > 0 )
		{
		if ( numbered_col >= 0 )	// RE-LABEL NUMBERING COLUMN
			{for ( i = fr; i < nr; i++ )
				{sg->Cells[numbered_col][i] = i + 1 - fr;
				}
			}
		sg->RowCount = nr;	          /* TRIM EXTRA SPACE */
		}
	else
		{sg->RowCount = fr + 1;
		}
	return( ndata_rows );
}
//---------------------------------------------------------------------------
bool replace_cursor( int indx, char *name, bool from_res )
{
	HCURSOR cur_handle = NULL;
	try
		{if ( from_res )
			{cur_handle = LoadCursor( HInstance, name );
			}
		else					// USE FOR ANIMATED
			{cur_handle = LoadCursorFromFile( name );
			}
		}
	catch( Exception &e )
		{return( false );
		}
	if ( NULL != cur_handle )
		{Screen->Cursors[indx] = cur_handle;
		return( true );
		}
	return( false );
}
//---------------------------------------------------------------------------
std::string file_size_md5( char *file_name )
{
	int	c;
	int	ns = 0;
	char	*buf = NULL;
	std::string	sm = "";
	FILE	*f = fopen( file_name, "rb" );
	if ( NULL == f )
		{return( sm );
		}
	try
		{
		if ( NULL == ( buf = (char *) malloc( 5000002 ) ) )
			{fclose( f );
			return( sm );
			}
		Screen->Cursor = crHourGlass;
		while( EOF != ( c = fgetc( f ) ) )
			{buf[ns] = (char) c;
			if ( 0 == ++ns % 5000000 )
				{void *ptr = realloc( buf, ns + 5000002 );
				if ( NULL == ptr )
					{ns = 0;
					break;
					}
				buf = (char *) ptr;
				}
			}
		sm = int2string( ns );
		if ( ns > 0 )
			{
			XMD5_DIGEST	m;
			XMD5::calc( &m, buf, ns );
			sm += std::string( "-" ) + std::string( m.hex )
				+ std::string( "." );
			}
		}
	__finally
		{
		Screen->Cursor = crDefault;
		if ( NULL != buf )
			{free( buf );
			}
		fclose( f );
		}
	return( sm );
}
//---------------------------------------------------------------------------
void form_to_clipboard( TCustomForm *f )
{
	Graphics::TBitmap *im = f->GetFormImage();
	Clipboard()->Assign( im );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void form_to_printer( TCustomForm *f, std::string txt )
{
	Graphics::TBitmap *im = f->GetFormImage();
	int	offset = 150;
	int	hi = im->Height;
	int	wi = im->Width;

	TPrinter *p = Printer();
	p->BeginDoc();
	int	f1 = ( ( p->PageWidth  - ( 2 * offset ) ) / wi );
	int	f2 = ( ( p->PageHeight - ( 2 * offset ) ) / hi );
	int	scale = ( f1 < f2 ) ? f1 : f2;
	if ( scale < 1 )
		{scale = 1;
		}
	TRect	r;
	r.left = offset;
	r.right = r.left + ( wi * scale );
	r.top = offset;
	r.bottom = r.top + ( hi * scale );
	p->Canvas->StretchDraw( r, im );
	int	spacing = ( 3 * p->Canvas->Font->Height ) / 2;
	if ( spacing < 0 )
		{spacing = -spacing;
		}
	p->Canvas->TextOut( offset, r.bottom + ( 2 * spacing ),
		String( "Prog  : " ) + Application->Title );
	p->Canvas->TextOut( offset, r.bottom + ( 3 * spacing ),
		String("Time : ")+TDateTime::CurrentDateTime().DateTimeString());
	p->Canvas->TextOut( offset, r.bottom + ( 4 * spacing ),
		String( "Form : " ) + f->Name + ", " + f->Caption );
	if ( txt != "" )
		{p->Canvas->TextOut( offset, r.bottom + ( 5 * spacing ),
			XUC::stds2cbs( txt ) );
		}
	p->EndDoc();
	delete im;
}
//---------------------------------------------------------------------------
static int qsort_integers_cmp_ordering = 1;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static int qsort_integers_cmp( const void *a, const void *b )
{	int	*ia = (int *) a;
	int	*ib = (int *) b;
	return( ( *ia - *ib ) * qsort_integers_cmp_ordering );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* SORT ARRAY OF INTEGERS */
void qsort_integers( int n, int *array, bool ascending_order )
{
	qsort_integers_cmp_ordering = ascending_order ? 1 : (-1);
	qsort( array, n, sizeof(int), qsort_integers_cmp );
}
//---------------------------------------------------------------------------
			/* DOES STRING CONTAIN SEQUENCE LIKE "234200234" ? */
bool are_digits( char *s, bool allow_leading_minus )
{
	if ( NULL == s )
		{return( false );
		}
	if ( *s == '-' )
		{if ( allow_leading_minus )
			{s++;
			}
		else
			{return( false );
			}
		}
	do			// MUST HAVE AT LEAST ONE DIGIT
		{if ( ! isdigit( *s ) )
			{return( false );
			}
		}
		while ( '\0' != *(++s) );
	return( true );
}
//---------------------------------------------------------------------------
		/* CENTRE NEW FORM RELATIVE TO CURRENTLY ACTIVE ONE */
void centraliseForm( TForm *popup )
{
	TForm	*fm = Screen->ActiveForm;
	int	left = fm->Left + ( ( fm->Width  - popup->Width  ) / 2 );
	int	top  = fm->Top  + ( ( fm->Height - popup->Height ) / 2 );
	if ( left + popup->Width > Screen->Width )	// ENSURE ON SCREEN
		{left = Screen->Width - popup->Width;
		}
	if ( top + popup->Height > Screen->Height )
		{top = Screen->Height - popup->Height;
		}
	popup->Left = ( left >= 0 ) ? left : 0;
	popup->Top  = ( top  >= 0 ) ? top : 0;
}
//---------------------------------------------------------------------------
void fullScreen( TForm *target )
{
	target->Top  = 0;
	target->Left = 0;
	target->Height = Screen->Height;
	target->Width = Screen->Width;
}
//---------------------------------------------------------------------------
bool getDiskUsage( const std::string path, double *avail, double *total )
{
	__int64	bfree, btotal;		// PASSED AS REFERENCES I ASSUME
#pragma warn -8111
	System::WideChar directory = *(String( XUC::stds2cbs(path) ).t_str());
#pragma warn +8111
	Sysutils::GetDiskFreeSpaceEx( &directory, bfree, btotal,
		NULL );
/*
	Sysutils::GetDiskFreeSpaceEx( (char *) path.c_str(), bfree, btotal,
		NULL );
*/
	*avail = (double) bfree;
	*total = (double) btotal;
	return( true );
}
//---------------------------------------------------------------------------
			/* CHECK EMAIL ADDRESS TO SEE IF FORMAT IS X@Y.Z */
const char *checkEmailFormat( const char *em )
{
	int	dots_after = 0, at_pos = -1;
	char	last = 0;
	int	len = strlen( em );
	int	i;
	for ( i = 0; i < len; i++ )
		{if ( COMAT == em[i] )
			{if ( at_pos < 0 )
				{at_pos = i;
				}
			else
				{return( "More than one `@' character present");
				}
			if ( 0 == i )
				{return(
				"Email cannot begin with '@' character" );
				}
			if ( len-1 == i )
				{return(
				"Email cannot end with '@' character" );
				}
			if ( DOT == last )
				{return(
		"Cannot have '@' (dot) immediately after '.' (dot)" );
				};
			}
		if ( DOT == em[i] )
			{if ( 0 == i )
				{return(
		"Email cannot begin with '.' character" );
				}
			if ( len-1 == i )
				{return(
		"Email cannot end with '.' character" );
				}
			if ( at_pos >= 0 )
				{dots_after++;
				}
			if ( DOT == last )
				{return(
		"Cannot have 2 consecutive '.' (dot) characters" );
				}
			if ( COMAT == last )
				{return(
		"Cannot have '.' (dot) immediately after '@'" );
				}
			}
		last = em[i];
		}
	if ( at_pos < 0 )
		{return( "There must be an '@' character" );
		}
	if ( dots_after < 1 )
		{return(
		"There must be at least one `.' (dot) after the '@' character");
		}
	return( NULL );
}
//---------------------------------------------------------------------------
bool checkEmail( const char *em )
{
	const char *err = checkEmailFormat( em );
	if ( NULL == err )
		{return( true );
		}
	std::string	emsg = std::string( "Email address " ) + std::string(em)
		+ " is invalid\n\nReason:" + err;
	MESS::error( emsg, MB_OK );
	return( false );
}
//---------------------------------------------------------------------------
void displayHelp( TForm *fm )
{
	fm_help->load_form( fm );
}
//---------------------------------------------------------------------------
void displayHelpFile( TForm *f, const char *name )
{
	fm_help->load_file( f, name );
}
//---------------------------------------------------------------------------
int string2intDef( const std::string s, const int def )
{
	const	char	*c = s.c_str();
	while ( isspace( *c ) )
		{c++;
		}
	if ( '-' == *c )
		{c++;
		}
	if ( ! isdigit( *c ) ) 		// TREAT EMPTY STRING AS ERROR
		{return( def );
		}
	int	ndigit = 0;
	while ( isdigit( *c ) )
		{ndigit++;
		c++;
		}
	if ( ndigit > 10 )		// OBVIOUS OVERFLOW
		{return( def );
		}
	while ( isspace( *c ) )		// CHECK FOR TRAILING PROBLEMS
		{c++;
		}
	if ( 0 != *c )
		{return( def );
		}
	int	n = def;
	try
		{n = atoi( s.c_str() );
		}
	catch ( ... )
		{n = def;
		}
	return( n );
}
//---------------------------------------------------------------------------
double string2double( const std::string s )
{
	double	d;
	try
		{d = atof( s.c_str() );
		}
	catch ( ... )
		{throw( "invalid string2double" );
		}
	return( d );
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

