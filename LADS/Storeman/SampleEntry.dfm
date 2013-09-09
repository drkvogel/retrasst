object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Retrieve Sample'
  ClientHeight = 628
  ClientWidth = 837
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  DesignSize = (
    837
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
    Left = 306
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
    Left = 624
    Top = 12
    Width = 49
    Height = 13
    Caption = 'Sort order'
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 176
    Width = 821
    Height = 429
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    TabOrder = 0
    OnFixedCellClick = grdSamplesFixedCellClick
  end
  object btnAddFile: TButton
    Left = 8
    Top = 130
    Width = 100
    Height = 34
    Caption = 'Add from file'
    Enabled = False
    TabOrder = 1
    OnClick = AddClick
  end
  object btnLocate: TButton
    Left = 316
    Top = 130
    Width = 100
    Height = 34
    Caption = 'Add location'
    Enabled = False
    TabOrder = 2
    OnClick = btnLocateClick
  end
  object CmbAliquot1: TComboBox
    Left = 102
    Top = 85
    Width = 160
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 3
    OnDropDown = CmbAliquot1DropDown
  end
  object RadIDType: TRadioGroup
    Left = 306
    Top = 12
    Width = 264
    Height = 50
    Caption = 'Item type'
    Columns = 3
    ItemIndex = 0
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 4
  end
  object CmbAliquot2: TComboBox
    Left = 410
    Top = 85
    Width = 160
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 5
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 102
    Top = 30
    Width = 160
    Height = 21
    Sorted = True
    TabOrder = 6
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 611
    Width = 837
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 7
  end
  object btnDestination: TButton
    Left = 470
    Top = 130
    Width = 100
    Height = 34
    Caption = 'New Boxes'
    Enabled = False
    TabOrder = 8
    OnClick = btnDestinationClick
  end
  object btnAddRecords: TButton
    Left = 162
    Top = 130
    Width = 100
    Height = 34
    Caption = 'Add specimens'
    Enabled = False
    TabOrder = 9
  end
  object lbSortCols: TListBox
    Left = 624
    Top = 29
    Width = 185
    Height = 124
    Anchors = [akLeft, akTop, akRight]
    Enabled = False
    ItemHeight = 13
    TabOrder = 10
  end
  object OpenDialog1: TOpenDialog
    Left = 122
    Top = 251
  end
end
