#ifndef XBASIC_H_INCLUDED
#define XBASIC_H_INCLUDED
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	XFREE(x)	{if ( NULL != (x)){free(x);x=NULL;}}
#define	XDELETE(x)	{delete x; x=NULL;}
#define	XFCLOSE(x)	{if ( NULL != (x)){fclose(x);x=NULL;}}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	APOSTROPHE	'\''
#define	ASTERISK	'*'
#define	BSLASH		'\\'	// BACK SLASH
#define	COMAT		'@'	// COMMERCIAL AT
#define	DOLLAR		'$'
#define	DOT		'.'
#define	EOS		'\0'
#define	EQUALS		'='
#define	FSLASH		'/'	// FORWARD SLASH
#define	LSB		'['	// LEFT SQUARE BRACKET
#define	MINUS		'-'
#define	NEWLINE		'\n'
#define	PLUS		'+'
#define	POUND		'£'
#define	RSB		']'
#define	SHRIEK		'!'
#define	SPACE		' '
#define	TAB_CHAR	'\t'
#define	TILDA		'~'
#define	UNDER_SCORE	'_'
#define	VERT_BAR	'|'
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif


