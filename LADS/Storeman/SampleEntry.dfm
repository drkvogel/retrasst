object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Modify Retrieval List'
  ClientHeight = 515
  ClientWidth = 843
  Color = 12316364
  Constraints.MaxHeight = 1100
  Constraints.MaxWidth = 1700
  Constraints.MinHeight = 520
  Constraints.MinWidth = 840
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnResize = FormResize
  DesignSize = (
    843
    515)
  PixelsPerInch = 96
  TextHeight = 13
  object LblAliquot1: TLabel
    Left = 12
    Top = 73
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 328
    Top = 73
    Width = 55
    Height = 13
    Caption = 'Secondary:'
  end
  object Label1: TLabel
    Left = 12
    Top = 27
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object Label2: TLabel
    Left = 587
    Top = 27
    Width = 53
    Height = 13
    Caption = 'Sort order:'
  end
  object grdSamples: TStringGrid
    Left = 3
    Top = 240
    Width = 837
    Height = 255
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 12
    OnFixedCellClick = grdSamplesFixedCellClick
  end
  object btnAddFile: TButton
    Left = 27
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Add from file'
    Enabled = False
    TabOrder = 5
    OnClick = AddClick
  end
  object btnLocate: TButton
    Left = 305
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Check location'
    Enabled = False
    TabOrder = 7
    OnClick = btnLocateClick
  end
  object CmbAliquot1: TComboBox
    Left = 100
    Top = 70
    Width = 170
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 1
    OnChange = CmbAliquotChange
    OnDropDown = CmbAliquot1DropDown
  end
  object rgItemType: TRadioGroup
    Left = 352
    Top = 115
    Width = 215
    Height = 52
    Caption = 'Source type'
    Columns = 3
    ItemIndex = 1
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 4
    OnClick = rgItemTypeClick
  end
  object CmbAliquot2: TComboBox
    Left = 397
    Top = 70
    Width = 170
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 2
    OnChange = CmbAliquotChange
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 100
    Top = 24
    Width = 170
    Height = 21
    Sorted = True
    TabOrder = 0
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 498
    Width = 843
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 13
  end
  object lbSortCols: TListBox
    Left = 659
    Top = 24
    Width = 152
    Height = 143
    Enabled = False
    ItemHeight = 13
    TabOrder = 11
  end
  object btnAddRecords: TButton
    Left = 166
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Add specimens'
    Enabled = False
    TabOrder = 6
  end
  object btnSaveList: TButton
    Left = 583
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Save job'
    Enabled = False
    TabOrder = 9
    OnClick = btnSaveListClick
  end
  object btnClrSort: TButton
    Left = 715
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Undo sorting'
    Enabled = False
    TabOrder = 10
    OnClick = btnClrSortClick
  end
  object btnClearList: TButton
    Left = 444
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Clear list'
    TabOrder = 8
    OnClick = btnClearListClick
  end
  object rgDestOrder: TRadioGroup
    Left = 12
    Top = 115
    Width = 302
    Height = 52
    Caption = 'Destination order'
    Columns = 3
    ItemIndex = 1
    Items.Strings = (
      'Keep list order'
      'Allow sorting'
      'Included in file')
    TabOrder = 3
    OnClick = rgDestOrderClick
  end
  object OpenDialog1: TOpenDialog
    Left = 650
    Top = 283
  end
end
