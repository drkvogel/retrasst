object frmNewSection: TfrmNewSection
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Configure Section'
  ClientHeight = 273
  ClientWidth = 234
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LblFill: TLabel
    Left = 31
    Top = 68
    Width = 41
    Height = 13
    Caption = 'Position:'
  end
  object LblPrefix: TLabel
    Left = 31
    Top = 29
    Width = 32
    Height = 13
    Caption = 'Prefix:'
  end
  object LblFirst: TLabel
    Left = 31
    Top = 107
    Width = 25
    Height = 13
    Caption = 'First:'
  end
  object LblLast: TLabel
    Left = 31
    Top = 146
    Width = 24
    Height = 13
    Caption = 'Last:'
  end
  object LblCap: TLabel
    Left = 31
    Top = 186
    Width = 37
    Height = 13
    Caption = '# slots:'
  end
  object TxtCap: TEdit
    Left = 96
    Top = 182
    Width = 48
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 7
    Text = '1'
  end
  object TxtLast: TEdit
    Left = 96
    Top = 142
    Width = 48
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 5
    Text = '2'
  end
  object TxtFirst: TEdit
    Left = 96
    Top = 103
    Width = 48
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 3
    Text = '1'
  end
  object TxtFill: TEdit
    Left = 96
    Top = 64
    Width = 48
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 1
    Text = '1'
  end
  object BtnCancel: TButton
    Left = 127
    Top = 232
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 10
    OnClick = CancelClick
  end
  object BtnOK: TButton
    Left = 31
    Top = 232
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 9
    OnClick = OKClick
  end
  object UpDownFill: TUpDown
    Left = 144
    Top = 64
    Width = 17
    Height = 21
    Associate = TxtFill
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 2
  end
  object TxtPrefix: TEdit
    Left = 96
    Top = 26
    Width = 121
    Height = 21
    CharCase = ecUpperCase
    MaxLength = 6
    TabOrder = 0
  end
  object UpDownFirst: TUpDown
    Left = 144
    Top = 103
    Width = 17
    Height = 21
    Associate = TxtFirst
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 4
  end
  object UpDownLast: TUpDown
    Left = 144
    Top = 142
    Width = 17
    Height = 21
    Associate = TxtLast
    Min = 1
    Max = 999
    Position = 2
    TabOrder = 6
  end
  object UpDownCap: TUpDown
    Left = 144
    Top = 182
    Width = 17
    Height = 21
    Associate = TxtCap
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 8
  end
end
