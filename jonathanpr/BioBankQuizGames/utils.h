#ifndef utilsH
#define utilsH
//---------------------------------------------------------------------------
#include <Grids.hpp>
#include <string>
#include "xuc.h"
//---------------------------------------------------------------------------
bool	streq_nocase( char *a, char *b );
std::string	int2string( const int n );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int 	load_items( TStrings *it, char **txt );
void	set_rgindex( TRadioGroup *trg, int indx );
bool 	radio_inrange( TRadioGroup *r, int imin, int imax );
std::string 	get_cbtext( TComboBox *c );
std::string	get_rgtext( TRadioGroup *trg );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
char	*string_duplicate( char *in );
void 	string_tolower( char *s );
void	capitalise_words( std::string *s );
void	filepath_to_url( char *url, const char *path );
char	*text_fmode( int fmode, bool include_instance );
TDateTime	whole_date( TDateTime t );
bool 	file_exists( char *filename );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void 	centraliseForm( TForm *popup );
void 	fullScreen( TForm *target );
void 	overlay( TForm *current, TForm *replacement );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/* AUXILLARY ROUTINES FOR MANIPULATING STRING GRIDS */
void	sg_empty( TStringGrid *sg );
int	sg_remove_row( TStringGrid *sg, int delete_row, int numbered_col );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string 	file_size_md5( char *file_name );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool 	replace_cursor( int indx, char *name, bool from_res );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void 	form_to_clipboard( TCustomForm *f );
void 	form_to_printer( TCustomForm *f, std::string txt );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void 	qsort_integers( int n, int *array, bool ascending_order );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool 	are_digits( char *s, bool allow_leading_minus );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool 	getDiskUsage( const std::string path, double *avail, double *total );
//---------------------------------------------------------------------------
const	char *checkEmailFormat( const char *em );
bool	checkEmail( const char *em );
//---------------------------------------------------------------------------
void 	displayHelp( TForm *fm );
void 	displayHelpFile( TForm *f, const char *name );
//---------------------------------------------------------------------------
int	string2intDef( const std::string s, const int def );
double	string2double( const std::string s );
//---------------------------------------------------------------------------
#define	APM	Application->ProcessMessages();
//---------------------------------------------------------------------------
#define	MISSING_TOKEN	"#"
//---------------------------------------------------------------------------
#endif

