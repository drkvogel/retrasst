object DataMod: TDataMod
  OldCreateOrder = False
  Left = 202
  Top = 118
  Height = 480
  Width = 696
  object QueryProject: TQuery
    DatabaseName = 'dbproject'
    Left = 128
    Top = 32
  end
  object QueryCentral: TQuery
    DatabaseName = 'dbcentral'
    Left = 128
    Top = 184
  end
  object QueryCentral2: TQuery
    DatabaseName = 'dbcentral'
    Left = 200
    Top = 184
  end
  object DatabaseProject: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'dbproject'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 48
    Top = 32
  end
  object DatabaseCentral: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'dbcentral'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 48
    Top = 184
  end
  object DatabaseProjectDdb: TDatabase
    AliasName = 'dsn_lab_star'
    DatabaseName = 'dbprojectddb'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 48
    Top = 112
  end
  object QueryProjectDdb: TQuery
    DatabaseName = 'dbprojectddb'
    Left = 160
    Top = 112
  end
end
