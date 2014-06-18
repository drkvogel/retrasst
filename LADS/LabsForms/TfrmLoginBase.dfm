object frmLoginBase: TfrmLoginBase
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Labs System Login'
  ClientHeight = 299
  ClientWidth = 330
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
    Left = 47
    Top = 123
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
    OnMouseEnter = hyperlinkEnter
    OnMouseLeave = hyperlinkLeave
  end
  object Label3: TLabel
    Left = 47
    Top = 170
    Width = 50
    Height = 13
    Caption = 'Password:'
  end
  object lblSystem: TLabel
    Left = 15
    Top = 51
    Width = 51
    Height = 13
    Caption = 'Database?'
    OnClick = lblSystemClick
    OnMouseEnter = hyperlinkEnter
    OnMouseLeave = hyperlinkLeave
  end
  object exitButton: TBitBtn
    Left = 175
    Top = 228
    Width = 108
    Height = 38
    Caption = '&Close'
    Kind = bkAbort
    NumGlyphs = 2
    TabOrder = 3
    OnClick = exitButtonClick
  end
  object okButton: TBitBtn
    Left = 43
    Top = 228
    Width = 108
    Height = 38
    Caption = '&OK (log in)'
    Enabled = False
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 2
    OnClick = okButtonClick
  end
  object userList: TComboBox
    Left = 120
    Top = 120
    Width = 160
    Height = 21
    DropDownCount = 20
    Enabled = False
    Sorted = True
    TabOrder = 0
    OnChange = textChange
  end
  object ebPassword: TEdit
    Left = 120
    Top = 167
    Width = 160
    Height = 21
    Enabled = False
    PasswordChar = '*'
    TabOrder = 1
    OnChange = textChange
  end
  object timer: TTimer
    Interval = 1200
    OnTimer = initialise
    Left = 280
    Top = 16
  end
end
