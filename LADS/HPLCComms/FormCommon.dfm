object frmCommon: TfrmCommon
  Left = 976
  Top = 236
  Caption = 'common'
  ClientHeight = 87
  ClientWidth = 206
  Color = 16761056
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object qryBuddyDB: TQuery
    DatabaseName = 'buddy_db'
    SessionName = 'Default'
    Left = 20
    Top = 15
  end
  object dbBuddy: TDatabase
    AliasName = 'alias_local_buddy'
    DatabaseName = 'buddy_db'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 65
    Top = 15
  end
  object dbCentral: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'labs_central_db'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 165
    Top = 15
  end
  object qryLabCentral: TQuery
    DatabaseName = 'labs_central_db'
    Left = 125
    Top = 12
  end
end
