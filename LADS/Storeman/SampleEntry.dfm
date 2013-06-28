object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Retrieve Sample'
  ClientHeight = 571
  ClientWidth = 620
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LblCaption: TLabel
    Left = 211
    Top = 168
    Width = 199
    Height = 16
    Caption = 'List of samples to be retrieved'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LblSample: TLabel
    Left = 10
    Top = 57
    Width = 39
    Height = 13
    Caption = 'Barcode'
  end
  object LblAliquot1: TLabel
    Left = 10
    Top = 89
    Width = 77
    Height = 13
    Caption = 'Aliquot Choice 1'
  end
  object LblAliquot2: TLabel
    Left = 256
    Top = 89
    Width = 77
    Height = 13
    Caption = 'Aliquot Choice 2'
  end
  object grdSamples: TStringGrid
    Left = 10
    Top = 200
    Width = 600
    Height = 321
    ColCount = 6
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
  end
  object TxtBarcode: TEdit
    Left = 93
    Top = 54
    Width = 102
    Height = 21
    TabOrder = 1
  end
  object BtnAdd: TButton
    Left = 10
    Top = 129
    Width = 75
    Height = 25
    Caption = 'Add'
    TabOrder = 2
    OnClick = AddClick
  end
  object BtnRetrieve: TButton
    Left = 535
    Top = 538
    Width = 75
    Height = 25
    Caption = 'Retrieve'
    TabOrder = 3
    OnClick = Retrieve
  end
  object CmbAliquot1: TComboBox
    Left = 93
    Top = 86
    Width = 145
    Height = 21
    TabOrder = 4
  end
  object RadIDType: TRadioGroup
    Left = 10
    Top = 2
    Width = 185
    Height = 38
    Caption = 'Select Item Type'
    Columns = 3
    ItemIndex = 0
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 5
  end
  object CmbAliquot2: TComboBox
    Left = 339
    Top = 86
    Width = 145
    Height = 21
    TabOrder = 6
  end
end
