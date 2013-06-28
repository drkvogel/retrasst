object AddBoxContent: TAddBoxContent
  Left = 408
  Top = 251
  Width = 660
  Height = 267
  BorderIcons = []
  Caption = 'Add Box Content'
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
    Top = 32
    Width = 57
    Height = 13
    Caption = 'Label prefix:'
  end
  object Label2: TLabel
    Left = 360
    Top = 73
    Width = 42
    Height = 13
    Caption = 'Box size:'
  end
  object Label3: TLabel
    Left = 16
    Top = 113
    Width = 44
    Height = 13
    Caption = 'Aliquot 1:'
  end
  object Label4: TLabel
    Left = 250
    Top = 113
    Width = 44
    Height = 13
    Caption = 'Aliquot 2:'
  end
  object Label5: TLabel
    Left = 469
    Top = 113
    Width = 44
    Height = 13
    Caption = 'Aliquot 3:'
  end
  object Label8: TLabel
    Left = 16
    Top = 73
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label9: TLabel
    Left = 16
    Top = 154
    Width = 40
    Height = 13
    Caption = 'Box Set:'
  end
  object Label10: TLabel
    Left = 233
    Top = 154
    Width = 61
    Height = 13
    Caption = 'Box Position:'
  end
  object Label6: TLabel
    Left = 314
    Top = 32
    Width = 88
    Height = 13
    Caption = 'Example box label:'
  end
  object edtBoxName: TEdit
    Left = 85
    Top = 28
    Width = 132
    Height = 21
    TabOrder = 0
    OnExit = edtBoxNameExit
  end
  object Panel1: TPanel
    Left = 0
    Top = 199
    Width = 652
    Height = 41
    Align = alBottom
    TabOrder = 10
    object Save: TButton
      Left = 448
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object btnClose: TButton
      Left = 128
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Close'
      TabOrder = 1
      OnClick = btnCloseClick
    end
  end
  object cbxAliquot2: TComboBox
    Left = 306
    Top = 109
    Width = 110
    Height = 21
    ItemHeight = 13
    TabOrder = 5
  end
  object cbxAliquot3: TComboBox
    Left = 526
    Top = 109
    Width = 110
    Height = 21
    ItemHeight = 13
    TabOrder = 6
  end
  object cbxAliquot1: TComboBox
    Left = 85
    Top = 109
    Width = 110
    Height = 21
    ItemHeight = 13
    TabOrder = 4
  end
  object cbxBoxSize: TComboBox
    Left = 416
    Top = 69
    Width = 220
    Height = 21
    ItemHeight = 13
    TabOrder = 3
  end
  object edtBoxLabel: TEdit
    Left = 416
    Top = 28
    Width = 220
    Height = 21
    Enabled = False
    TabOrder = 1
  end
  object edtBoxSet: TEdit
    Left = 85
    Top = 150
    Width = 70
    Height = 21
    TabOrder = 7
  end
  object edtBoxOrder: TEdit
    Left = 306
    Top = 150
    Width = 70
    Height = 21
    TabOrder = 8
  end
  object cbxStatus: TCheckBox
    Left = 469
    Top = 152
    Width = 70
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 9
  end
  object edtDescription: TEdit
    Left = 85
    Top = 69
    Width = 188
    Height = 21
    TabOrder = 2
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 304
    Top = 56
  end
end
