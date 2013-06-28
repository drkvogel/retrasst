object AddProject: TAddProject
  Left = 464
  Top = 114
  Width = 482
  Height = 558
  BorderIcons = []
  Caption = 'Add Project'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 18
    Top = 56
    Width = 95
    Height = 13
    Caption = 'Full name of project:'
  end
  object Label3: TLabel
    Left = 18
    Top = 144
    Width = 61
    Height = 13
    Caption = 'Valid dates --'
  end
  object Label4: TLabel
    Left = 72
    Top = 167
    Width = 26
    Height = 13
    Caption = 'From:'
  end
  object Label5: TLabel
    Left = 278
    Top = 167
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object Label6: TLabel
    Left = 18
    Top = 336
    Width = 78
    Height = 13
    Caption = 'Database name:'
  end
  object Label8: TLabel
    Left = 18
    Top = 210
    Width = 121
    Height = 13
    Caption = 'Address to send e-mail to:'
  end
  object Label9: TLabel
    Left = 312
    Top = 298
    Width = 33
    Height = 13
    Caption = 'Status:'
  end
  object Label1: TLabel
    Left = 18
    Top = 24
    Width = 65
    Height = 13
    Caption = 'Project name:'
  end
  object Label10: TLabel
    Left = 18
    Top = 298
    Width = 86
    Height = 13
    Caption = 'Database version:'
  end
  object Label13: TLabel
    Left = 96
    Top = 448
    Width = 282
    Height = 13
    Caption = 'Note that you can only use this button with LIVE databases.'
  end
  object Label14: TLabel
    Left = 12
    Top = 464
    Width = 451
    Height = 13
    Caption = 
      'For test databases, first create the live database, then ask Cor' +
      'e Prog to clone it with makemirror.'
  end
  object Panel1: TPanel
    Left = 0
    Top = 490
    Width = 474
    Height = 41
    Align = alBottom
    TabOrder = 5
    object Save: TButton
      Left = 311
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 95
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object ProjectDatabase: TEdit
    Left = 128
    Top = 332
    Width = 121
    Height = 21
    TabOrder = 3
  end
  object ProjectName: TEdit
    Left = 108
    Top = 20
    Width = 135
    Height = 21
    TabOrder = 0
  end
  object FullName: TMemo
    Left = 50
    Top = 75
    Width = 390
    Height = 49
    Lines.Strings = (
      '')
    TabOrder = 1
  end
  object memURL: TMemo
    Left = 50
    Top = 230
    Width = 390
    Height = 49
    Lines.Strings = (
      '')
    TabOrder = 2
  end
  object rgStatus: TRadioGroup
    Left = 359
    Top = 288
    Width = 81
    Height = 87
    ItemIndex = 1
    Items.Strings = (
      'live'
      'test'
      'obsolete')
    TabOrder = 4
  end
  object edtDbVersion: TEdit
    Left = 128
    Top = 294
    Width = 73
    Height = 21
    Enabled = False
    TabOrder = 6
  end
  object dtpFrom: TDateTimePicker
    Left = 108
    Top = 163
    Width = 137
    Height = 21
    CalAlignment = dtaLeft
    Date = 0.610780706017977
    Time = 0.610780706017977
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 7
  end
  object dtpTo: TDateTimePicker
    Left = 303
    Top = 163
    Width = 137
    Height = 21
    CalAlignment = dtaLeft
    Date = 0.610780706017977
    Time = 0.610780706017977
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 8
  end
  object btnCreateDatabase: TButton
    Left = 176
    Top = 408
    Width = 121
    Height = 33
    Caption = 'Create Database'
    TabOrder = 9
    OnClick = btnCreateDatabaseClick
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 56
    Top = 408
  end
  object testDatabase: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'projectTest'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 376
    Top = 408
  end
end
