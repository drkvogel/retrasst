object WizardLogin: TWizardLogin
  Left = 196
  Top = 120
  Width = 400
  Height = 224
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Wizard Login'
  Color = 13434828
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 120
    Top = 10
    Width = 120
    Height = 29
    Caption = 'Lab Wizard'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 38
    Top = 58
    Width = 56
    Height = 16
    Caption = 'Operator:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 38
    Top = 96
    Width = 63
    Height = 16
    Caption = 'Password:'
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
  end
  object SystemChoice: TRadioGroup
    Left = 285
    Top = 50
    Width = 70
    Height = 67
    Caption = 'System'
    ItemIndex = 1
    Items.Strings = (
      'Live '
      'Mirror')
    TabOrder = 0
    OnClick = SystemChoiceClick
  end
  object cbxOperator: TComboBox
    Left = 115
    Top = 56
    Width = 140
    Height = 21
    Style = csDropDownList
    ItemHeight = 13
    TabOrder = 1
    OnDropDown = cbxOperatorDropDown
  end
  object edtPassword: TEdit
    Left = 115
    Top = 94
    Width = 140
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
    Text = 'edtPassword'
  end
  object btnOperatorOK: TButton
    Left = 34
    Top = 148
    Width = 80
    Height = 28
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = btnOperatorOKClick
  end
  object btnExit: TButton
    Left = 282
    Top = 148
    Width = 80
    Height = 28
    Caption = 'Exit'
    TabOrder = 5
    OnClick = btnExitClick
  end
  object AboutButton: TButton
    Left = 158
    Top = 148
    Width = 80
    Height = 28
    Caption = 'About'
    TabOrder = 4
    OnClick = AboutButtonClick
  end
  object cDatabase: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'centralData'
    LoginPrompt = False
    Params.Strings = (
      'DATABASE NAME='#39't_ldbc'#39)
    SessionName = 'Default'
    Left = 24
    Top = 16
  end
  object cQuery: TQuery
    DatabaseName = 'centralData'
    UniDirectional = True
    Left = 64
    Top = 16
  end
end
