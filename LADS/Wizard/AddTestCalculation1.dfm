object AddTestCalculation: TAddTestCalculation
  Left = 55
  Top = 233
  Width = 488
  Height = 157
  BorderIcons = []
  Caption = 'Test Calculation'
  Color = clBtnFace
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
    Left = 93
    Top = 37
    Width = 5
    Height = 13
    Caption = 'x'
  end
  object Label2: TLabel
    Left = 360
    Top = 37
    Width = 5
    Height = 13
    Caption = 'x'
  end
  object cbxTest1: TComboBox
    Left = 108
    Top = 33
    Width = 82
    Height = 21
    ItemHeight = 13
    TabOrder = 1
  end
  object cbxCalc: TComboBox
    Left = 213
    Top = 33
    Width = 55
    Height = 21
    ItemHeight = 13
    TabOrder = 2
  end
  object cbxTest2: TComboBox
    Left = 376
    Top = 33
    Width = 82
    Height = 21
    ItemHeight = 13
    TabOrder = 4
  end
  object Panel1: TPanel
    Left = 0
    Top = 88
    Width = 480
    Height = 42
    Align = alBottom
    TabOrder = 5
    object btnCancel: TButton
      Left = 52
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
    end
    object btnSave: TButton
      Left = 352
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnSaveClick
    end
    object btnClear: TButton
      Left = 202
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Clear'
      TabOrder = 2
      OnClick = btnClearClick
    end
  end
  object edtTest1: TEdit
    Left = 24
    Top = 33
    Width = 60
    Height = 21
    TabOrder = 0
  end
  object edtTest2: TEdit
    Left = 292
    Top = 33
    Width = 60
    Height = 21
    TabOrder = 3
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 272
    Top = 56
  end
end
