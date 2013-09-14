object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Modify Retrieval List'
  ClientHeight = 660
  ClientWidth = 1091
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  DesignSize = (
    1091
    660)
  PixelsPerInch = 96
  TextHeight = 13
  object LblAliquot1: TLabel
    Left = 8
    Top = 83
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 294
    Top = 83
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
    Left = 862
    Top = 33
    Width = 53
    Height = 13
    Caption = 'Sort order:'
  end
  object Label3: TLabel
    Left = 592
    Top = 33
    Width = 47
    Height = 13
    Caption = 'Box type:'
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 176
    Width = 1075
    Height = 461
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 9
    OnFixedCellClick = grdSamplesFixedCellClick
  end
  object btnAddFile: TButton
    Left = 8
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Add from file'
    Enabled = False
    TabOrder = 4
    OnClick = AddClick
  end
  object btnLocate: TButton
    Left = 313
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Set location'
    Enabled = False
    TabOrder = 6
    OnClick = btnLocateClick
  end
  object CmbAliquot1: TComboBox
    Left = 89
    Top = 80
    Width = 172
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 2
    OnDropDown = CmbAliquot1DropDown
  end
  object rgItemType: TRadioGroup
    Left = 294
    Top = 8
    Width = 268
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
    Left = 390
    Top = 80
    Width = 172
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 3
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 89
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
    Top = 643
    Width = 1091
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 10
  end
  object btnDestination: TButton
    Left = 466
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Set destination'
    Enabled = False
    TabOrder = 7
    OnClick = btnDestinationClick
  end
  object lbSortCols: TListBox
    Left = 928
    Top = 16
    Width = 154
    Height = 104
    Enabled = False
    ItemHeight = 13
    TabOrder = 11
  end
  object btnAddRecords: TButton
    Left = 160
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Add specimens'
    Enabled = False
    TabOrder = 5
  end
  object btnSaveList: TButton
    Left = 655
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Create list'
    Enabled = False
    TabOrder = 8
    OnClick = btnSaveListClick
  end
  object btnClrSort: TButton
    Left = 964
    Top = 128
    Width = 96
    Height = 34
    Caption = 'Unsort'
    TabOrder = 12
    OnClick = btnClrSortClick
  end
  object btnNewContent: TButton
    Left = 655
    Top = 74
    Width = 96
    Height = 34
    Caption = 'Add Box Type'
    Enabled = False
    TabOrder = 13
    OnClick = btnNewContentClick
  end
  object cbBoxType: TComboBox
    Left = 655
    Top = 30
    Width = 172
    Height = 21
    TabOrder = 14
  end
  object OpenDialog1: TOpenDialog
    Left = 114
    Top = 219
  end
end
