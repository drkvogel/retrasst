object frmLoginBase: TfrmLoginBase
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Labs System Login'
  ClientHeight = 310
  ClientWidth = 315
  Color = 12316364
  Constraints.MaxHeight = 400
  Constraints.MaxWidth = 400
  Constraints.MinHeight = 300
  Constraints.MinWidth = 300
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 52
    Top = 139
    Width = 55
    Height = 13
    Caption = 'User name:'
  end
  object version: TLabel
    Left = 14
    Top = 20
    Width = 60
    Height = 18
    Caption = 'Program'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Atlanta'
    Font.Style = [fsBold]
    ParentFont = False
    OnClick = versionClick
    OnMouseEnter = versionMouseEnter
    OnMouseLeave = versionMouseLeave
  end
  object Label3: TLabel
    Left = 52
    Top = 183
    Width = 50
    Height = 13
    Caption = 'Password:'
  end
  object exitButton: TBitBtn
    Left = 179
    Top = 240
    Width = 108
    Height = 38
    Caption = '&Close'
    Kind = bkAbort
    NumGlyphs = 2
    TabOrder = 4
    OnClick = exitButtonClick
  end
  object okButton: TBitBtn
    Left = 28
    Top = 240
    Width = 108
    Height = 38
    Caption = '&OK (log in)'
    Enabled = False
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 3
    OnClick = okButtonClick
  end
  object userList: TComboBox
    Left = 126
    Top = 135
    Width = 138
    Height = 21
    DropDownCount = 20
    Enabled = False
    Sorted = True
    TabOrder = 1
    OnChange = textChange
  end
  object rgDatabase: TRadioGroup
    Left = 28
    Top = 56
    Width = 259
    Height = 42
    Columns = 3
    Items.Strings = (
      'Test'
      'Live'
      'Mirror')
    TabOrder = 0
    TabStop = True
    OnClick = rgDatabaseClick
  end
  object ebPassword: TEdit
    Left = 126
    Top = 180
    Width = 138
    Height = 21
    Enabled = False
    PasswordChar = '*'
    TabOrder = 2
    OnChange = textChange
  end
  object timer: TTimer
    Interval = 1200
    OnTimer = initialise
    Left = 248
    Top = 16
  end
end
