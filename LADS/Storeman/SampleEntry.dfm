object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Modift Retrieval List'
  ClientHeight = 628
  ClientWidth = 1022
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  DesignSize = (
    1022
    628)
  PixelsPerInch = 96
  TextHeight = 13
  object LblAliquot1: TLabel
    Left = 8
    Top = 88
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 342
    Top = 88
    Width = 90
    Height = 13
    Caption = 'Secondary aliquot:'
  end
  object Label1: TLabel
    Left = 8
    Top = 33
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object Label2: TLabel
    Left = 672
    Top = 10
    Width = 49
    Height = 13
    Caption = 'Sort order'
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 176
    Width = 1006
    Height = 429
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 9
    OnFixedCellClick = grdSamplesFixedCellClick
    ExplicitWidth = 821
  end
  object btnAddFile: TButton
    Left = 8
    Top = 130
    Width = 96
    Height = 34
    Caption = 'Add from file'
    Enabled = False
    TabOrder = 4
    OnClick = AddClick
  end
  object btnLocate: TButton
    Left = 265
    Top = 130
    Width = 96
    Height = 34
    Caption = 'Set location'
    Enabled = False
    TabOrder = 6
    OnClick = btnLocateClick
  end
  object CmbAliquot1: TComboBox
    Left = 96
    Top = 85
    Width = 172
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 2
    OnDropDown = CmbAliquot1DropDown
  end
  object RadIDType: TRadioGroup
    Left = 342
    Top = 10
    Width = 276
    Height = 52
    Caption = 'Item type'
    Columns = 3
    ItemIndex = 0
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 1
  end
  object CmbAliquot2: TComboBox
    Left = 446
    Top = 85
    Width = 172
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 3
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 96
    Top = 30
    Width = 172
    Height = 21
    Sorted = True
    TabOrder = 0
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 611
    Width = 1022
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 10
    ExplicitWidth = 837
  end
  object btnDestination: TButton
    Left = 393
    Top = 130
    Width = 96
    Height = 34
    Caption = 'Set destination'
    Enabled = False
    TabOrder = 7
    OnClick = btnDestinationClick
  end
  object lbSortCols: TListBox
    Left = 672
    Top = 29
    Width = 313
    Height = 124
    Anchors = [akLeft, akTop, akRight]
    Enabled = False
    ItemHeight = 13
    TabOrder = 11
  end
  object btnAddRecords: TButton
    Left = 136
    Top = 130
    Width = 96
    Height = 34
    Caption = 'Add specimens'
    Enabled = False
    TabOrder = 5
  end
  object btnSaveList: TButton
    Left = 522
    Top = 130
    Width = 96
    Height = 34
    Caption = 'Create list'
    Enabled = False
    TabOrder = 8
    OnClick = btnSaveListClick
  end
  object OpenDialog1: TOpenDialog
    Left = 122
    Top = 251
  end
end
