object Login: TLogin
  Left = 233
  Top = 280
  Width = 341
  Height = 233
  BorderIcons = []
  Caption = 'Database Choice'
  Color = 13434828
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
  object ProjectPanel: TPanel
    Left = 21
    Top = 80
    Width = 290
    Height = 57
    TabOrder = 0
    object Label2: TLabel
      Left = 21
      Top = 20
      Width = 82
      Height = 16
      Caption = 'Project name:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
    end
    object cbProjectName: TComboBox
      Left = 125
      Top = 18
      Width = 145
      Height = 21
      DropDownCount = 12
      ItemHeight = 13
      Sorted = True
      TabOrder = 0
      OnChange = cbProjectNameChange
    end
  end
  object BtnCentralDb: TButton
    Left = 21
    Top = 25
    Width = 128
    Height = 34
    Caption = 'Central Database'
    TabOrder = 1
    OnClick = BtnCentralDbClick
  end
  object BtnProjectDb: TButton
    Left = 183
    Top = 25
    Width = 128
    Height = 34
    Caption = 'Project Database'
    TabOrder = 2
    OnClick = BtnProjectDbClick
  end
  object Panel1: TPanel
    Left = 0
    Top = 158
    Width = 333
    Height = 48
    Align = alBottom
    TabOrder = 3
    object Button1: TButton
      Left = 30
      Top = 10
      Width = 80
      Height = 28
      Caption = 'Close'
      Default = True
      TabOrder = 0
      OnClick = Button1Click
    end
  end
  object pDatabase: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'projectData'
    LoginPrompt = False
    Params.Strings = (
      'DATABASE NAME=t_ldb1')
    SessionName = 'Default'
    Left = 136
    Top = 168
  end
  object dDatabase: TDatabase
    AliasName = 'dsn_lab_star'
    DatabaseName = 'ddbData'
    LoginPrompt = False
    Params.Strings = (
      'DATABASE NAME=t_ldb1_ddb')
    SessionName = 'Default'
    Left = 216
    Top = 168
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    UniDirectional = True
    Left = 173
    Top = 168
  end
end
