#ifndef OPERATORCLASS
#define OPERATORCLASS

class OperatorClass : public TObject{

public:
    OperatorClass();
    OperatorClass(AnsiString aName);
    virtual __fastcall ~OperatorClass();

    AnsiString GetName(){return theName;}
    void SetName(AnsiString name){theName = name;}

    //set get OperatorID
    int GetOperatorID(){ return OperatorID;}
    void SetOperatorID(int id){ OperatorID = id; }

    // set get Description
    AnsiString GetDescription(){ return Description; }
    void SetDescription(AnsiString desc){ Description = desc; }

    // set get ValidFrom
    AnsiString GetValidFrom() { return DateToStr(ValidFrom); }
    void SetValidFrom(AnsiString vf) { ValidFrom = StrToDate(vf); }

    // set get ValidTo
    AnsiString GetValidTo() { return DateToStr(ValidTo); }
    void SetValidTo(AnsiString vt) { ValidTo = StrToDate(vt); }

    // set get Status
    short GetStatus() { return Status; }
    void SetStatus(short st) { Status = st; }

    // set get Password
    AnsiString GetPassword() { return Password; }
    void SetPassword(AnsiString pw);
    bool ValidatePassword( AnsiString typedPassword );

    // set get Abilities
    void ClearAbilities() { Abilities = 0; }
    void AddAbility( int abil );
    bool HasAbility( int abil );
    int GetAbilities() { return Abilities; }

    void LoadFromDatabase(TQuery* query, int recno);
    bool InsertToDatabase(TQuery* query);
    bool UpdateToDatabase(TQuery* query);
    void LogRecord( AnsiString Text );

    static const int OPER_ABIL_VIEW;
    static const int OPER_ABIL_VALIDATE1;
    static const int OPER_ABIL_VALIDATE2;
    static const int OPER_ABIL_PLOTQC;
    static const int OPER_ABIL_WIZARD;
    static const int NUM_ABILITIES;

    static const int OPER_STATUS_ACTIVE;
    static const int OPER_STATUS_INACTIVE;
    static const AnsiString STR_OPER_STATUS_ACTIVE;
    static const AnsiString STR_OPER_STATUS_INACTIVE;
    static const int NUM_OPER_STATUS;

private:
    void FormatSQL( TQuery* query );

    AnsiString theName;

    // persistent parameters
    int OperatorID;
    AnsiString Description;
    TDate ValidFrom;
    TDate ValidTo;
    short Status;
    AnsiString Password;
    AnsiString Password_md5;
    int Abilities;
};
#endif
