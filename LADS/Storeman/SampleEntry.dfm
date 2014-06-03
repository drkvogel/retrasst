object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Modify Retrieval List'
  ClientHeight = 515
  ClientWidth = 841
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
    841
    515)
  PixelsPerInch = 96
  TextHeight = 13
  object LblAliquot1: TLabel
    Left = 10
    Top = 73
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 315
    Top = 73
    Width = 55
    Height = 13
    Caption = 'Secondary:'
  end
  object Label1: TLabel
    Left = 10
    Top = 27
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object Label2: TLabel
    Left = 595
    Top = 27
    Width = 53
    Height = 13
    Caption = 'Sort order:'
  end
  object grdSamples: TStringGrid
    Left = 3
    Top = 240
    Width = 835
    Height = 255
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 11
    OnFixedCellClick = grdSamplesFixedCellClick
  end
  object btnAddFile: TButton
    Left = 23
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Add from file'
    Enabled = False
    TabOrder = 4
    OnClick = AddClick
  end
  object btnLocate: TButton
    Left = 301
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Check location'
    Enabled = False
    TabOrder = 6
    OnClick = btnLocateClick
  end
  object CmbAliquot1: TComboBox
    Left = 107
    Top = 70
    Width = 174
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 1
    OnDropDown = CmbAliquot1DropDown
  end
  object rgItemType: TRadioGroup
    Left = 8
    Top = 115
    Width = 269
    Height = 52
    Caption = 'Source type'
    Columns = 3
    Enabled = False
    ItemIndex = 1
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 3
  end
  object CmbAliquot2: TComboBox
    Left = 387
    Top = 70
    Width = 174
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 2
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 107
    Top = 24
    Width = 174
    Height = 21
    Sorted = True
    TabOrder = 0
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 498
    Width = 841
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 12
    ExplicitTop = 491
    ExplicitWidth = 830
  end
  object lbSortCols: TListBox
    Left = 667
    Top = 19
    Width = 152
    Height = 148
    Enabled = False
    ItemHeight = 13
    TabOrder = 10
  end
  object btnAddRecords: TButton
    Left = 162
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Add specimens'
    Enabled = False
    TabOrder = 5
  end
  object btnSaveList: TButton
    Left = 579
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Save job'
    Enabled = False
    TabOrder = 8
    OnClick = btnSaveListClick
  end
  object btnClrSort: TButton
    Left = 718
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Undo sorting'
    Enabled = False
    TabOrder = 9
    OnClick = btnClrSortClick
  end
  object btnClearList: TButton
    Left = 440
    Top = 192
    Width = 100
    Height = 32
    Caption = 'Clear list'
    TabOrder = 7
    OnClick = btnClearListClick
  end
  object OpenDialog1: TOpenDialog
    Left = 650
    Top = 283
  end
end
