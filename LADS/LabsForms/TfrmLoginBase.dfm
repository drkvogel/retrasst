object frmLoginBase: TfrmLoginBase
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Labs System Login'
  ClientHeight = 320
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
  DesignSize = (
    330
    320)
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 48
    Top = 147
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
    Left = 48
    Top = 191
    Width = 50
    Height = 13
    Caption = 'Password:'
  end
  object exitButton: TBitBtn
    Left = 180
    Top = 252
    Width = 108
    Height = 38
    Caption = '&Close'
    Kind = bkAbort
    NumGlyphs = 2
    TabOrder = 4
    OnClick = exitButtonClick
  end
  object okButton: TBitBtn
    Left = 41
    Top = 252
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
    Left = 124
    Top = 143
    Width = 160
    Height = 21
    DropDownCount = 20
    Enabled = False
    Sorted = True
    TabOrder = 1
    OnChange = textChange
  end
  object rgDatabase: TRadioGroup
    Left = 16
    Top = 56
    Width = 298
    Height = 49
    Anchors = [akLeft, akTop, akRight]
    Columns = 5
    Ctl3D = True
    Items.Strings = (
      'Live'
      'Test'
      'Mirror'
      'Dev'
      'DbTest')
    ParentCtl3D = False
    TabOrder = 0
    TabStop = True
    OnClick = rgDatabaseClick
  end
  object ebPassword: TEdit
    Left = 124
    Top = 187
    Width = 160
    Height = 21
    Enabled = False
    PasswordChar = '*'
    TabOrder = 2
    OnChange = textChange
  end
  object timer: TTimer
    Interval = 1200
    OnTimer = initialise
    Left = 280
    Top = 16
  end
end
