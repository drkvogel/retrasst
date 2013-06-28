object frmRetrievalAssistant: TfrmRetrievalAssistant
  Left = 0
  Top = 0
  Caption = 'Retrieval Assistant'
  ClientHeight = 504
  ClientWidth = 710
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object comboJob: TComboBox
    Left = 136
    Top = 56
    Width = 145
    Height = 21
    Style = csDropDownList
    TabOrder = 0
  end
  object sgBoxes: TStringGrid
    Left = 64
    Top = 160
    Width = 320
    Height = 120
    FixedCols = 0
    TabOrder = 1
  end
  object lbJobs: TListBox
    Left = 88
    Top = 104
    Width = 121
    Height = 97
    ItemHeight = 13
    TabOrder = 2
  end
end
