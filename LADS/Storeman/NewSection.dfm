object frmNewSection: TfrmNewSection
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Configure Section'
  ClientHeight = 283
  ClientWidth = 244
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object LblFill: TLabel
    Left = 36
    Top = 68
    Width = 41
    Height = 13
    Caption = 'Position:'
  end
  object LblPrefix: TLabel
    Left = 36
    Top = 30
    Width = 32
    Height = 13
    Caption = 'Prefix:'
  end
  object LblFirst: TLabel
    Left = 36
    Top = 107
    Width = 25
    Height = 13
    Caption = 'First:'
  end
  object LblLast: TLabel
    Left = 36
    Top = 146
    Width = 24
    Height = 13
    Caption = 'Last:'
  end
  object LblCap: TLabel
    Left = 36
    Top = 186
    Width = 37
    Height = 13
    Caption = '# slots:'
  end
  object TxtCap: TEdit
    Left = 93
    Top = 183
    Width = 46
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 7
    Text = '1'
  end
  object TxtLast: TEdit
    Left = 93
    Top = 143
    Width = 46
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 5
    Text = '2'
  end
  object TxtFirst: TEdit
    Left = 93
    Top = 104
    Width = 46
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 3
    Text = '1'
  end
  object TxtFill: TEdit
    Left = 93
    Top = 65
    Width = 46
    Height = 21
    MaxLength = 3
    NumbersOnly = True
    TabOrder = 1
    Text = '1'
  end
  object BtnCancel: TButton
    Left = 132
    Top = 232
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 10
    OnClick = CancelClick
  end
  object BtnOK: TButton
    Left = 36
    Top = 232
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    TabOrder = 9
    OnClick = OKClick
  end
  object UpDownFill: TUpDown
    Left = 138
    Top = 65
    Width = 17
    Height = 21
    Associate = TxtFill
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 2
  end
  object TxtPrefix: TEdit
    Left = 93
    Top = 27
    Width = 114
    Height = 21
    CharCase = ecUpperCase
    MaxLength = 6
    TabOrder = 0
  end
  object UpDownFirst: TUpDown
    Left = 138
    Top = 104
    Width = 17
    Height = 21
    Associate = TxtFirst
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 4
  end
  object UpDownLast: TUpDown
    Left = 138
    Top = 143
    Width = 17
    Height = 21
    Associate = TxtLast
    Min = 1
    Max = 999
    Position = 2
    TabOrder = 6
  end
  object UpDownCap: TUpDown
    Left = 138
    Top = 183
    Width = 17
    Height = 21
    Associate = TxtCap
    Min = 1
    Max = 999
    Position = 1
    TabOrder = 8
  end
end
