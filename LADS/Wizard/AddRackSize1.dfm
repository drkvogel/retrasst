object AddRackSize: TAddRackSize
  Left = 735
  Top = 233
  Width = 376
  Height = 228
  Caption = 'Add Rack Size'
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
    Left = 16
    Top = 22
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label2: TLabel
    Left = 16
    Top = 68
    Width = 44
    Height = 13
    Caption = 'Capacity:'
  end
  object Label6: TLabel
    Left = 216
    Top = 68
    Width = 46
    Height = 13
    Caption = 'First rack:'
  end
  object Label3: TLabel
    Left = 16
    Top = 114
    Width = 44
    Height = 13
    Caption = 'Box Size:'
  end
  object edtDescription: TEdit
    Left = 82
    Top = 18
    Width = 265
    Height = 21
    TabOrder = 0
  end
  object edtCapacity: TEdit
    Left = 82
    Top = 64
    Width = 69
    Height = 21
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 160
    Width = 368
    Height = 41
    Align = alBottom
    TabOrder = 4
    object Save: TButton
      Left = 231
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 63
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object edtFirstRack: TEdit
    Left = 277
    Top = 64
    Width = 68
    Height = 21
    TabOrder = 2
  end
  object cbBoxSize: TComboBox
    Left = 82
    Top = 110
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 3
  end
  object cbActive: TCheckBox
    Left = 280
    Top = 112
    Width = 65
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 5
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 168
    Top = 48
  end
end
