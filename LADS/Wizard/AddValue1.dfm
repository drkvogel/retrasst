object AddValue: TAddValue
  Left = 361
  Top = 118
  Width = 385
  Height = 199
  BorderIcons = []
  Caption = 'Add Value'
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
    Left = 26
    Top = 76
    Width = 44
    Height = 13
    Caption = 'Minimum:'
  end
  object Label3: TLabel
    Left = 216
    Top = 76
    Width = 47
    Height = 13
    Caption = 'Maximum:'
  end
  object Label7: TLabel
    Left = 26
    Top = 28
    Width = 109
    Height = 13
    Caption = 'Descriptor value name:'
  end
  object Minimum: TEdit
    Left = 80
    Top = 72
    Width = 89
    Height = 21
    TabOrder = 1
    OnExit = MinimumExit
  end
  object Maximum: TEdit
    Left = 272
    Top = 72
    Width = 89
    Height = 21
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 0
    Top = 123
    Width = 377
    Height = 49
    Align = alBottom
    TabOrder = 3
    object btnSave: TButton
      Left = 223
      Top = 12
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = btnSaveClick
    end
    object btnCancel: TButton
      Left = 79
      Top = 12
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
    end
  end
  object ValueName: TEdit
    Left = 154
    Top = 24
    Width = 207
    Height = 21
    TabOrder = 0
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 432
    Top = 48
  end
end
