//---------------------------------------------------------------------------
#ifndef QCMaterialClassH
#define QCMaterialClassH

#include "dbtables.hpp"

//---------------------------------------------------------------------------

class QCLevelClass;

class QCMaterialClass
{
	void FormatSQL( TQuery* query );

	int numQCLevels;
	QCLevelClass** theQCLevels;

	// persistent parameters
	AnsiString Name;
	AnsiString Description;

public:

	QCMaterialClass();
	QCMaterialClass(AnsiString aName);
	virtual __fastcall ~QCMaterialClass();

	//set get Name
	AnsiString GetName(){ return Name;}
	void SetName(AnsiString name){ Name = name; }

	// set get Description
	AnsiString GetDescription(){ return Description; }
	void SetDescription(AnsiString desc){ Description = desc; }

	void LoadFromDatabase(TQuery* query, int recno);
	void InsertToDatabase(TQuery* query);
	void UpdateToDatabase(TQuery* query);

	//get numQCLevels
	int GetNumQCLevels(){ return numQCLevels;}

	void LoadQCLevelsFromDatabase(TQuery* query);
	void DeleteQCLevelClass();

	QCLevelClass* GetQCLevel( int index );
	QCLevelClass* GetQCLevel( AnsiString QCLevelName );
};

#endif
 