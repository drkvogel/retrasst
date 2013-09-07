object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Retrieve Sample'
  ClientHeight = 689
  ClientWidth = 1019
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  DesignSize = (
    1019
    689)
  PixelsPerInch = 96
  TextHeight = 13
  object LblAliquot1: TLabel
    Left = 12
    Top = 83
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 330
    Top = 83
    Width = 90
    Height = 13
    Caption = 'Secondary aliquot:'
  end
  object Label1: TLabel
    Left = 12
    Top = 26
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 192
    Width = 1003
    Height = 474
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 6
    DefaultColWidth = 90
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goFixedRowClick]
    TabOrder = 0
    OnFixedCellClick = grdSamplesFixedCellClick
  end
  object BtnAdd: TButton
    Left = 12
    Top = 140
    Width = 90
    Height = 28
    Caption = 'Add from file'
    TabOrder = 1
    OnClick = AddClick
  end
  object BtnRetrieve: TButton
    Left = 330
    Top = 140
    Width = 90
    Height = 28
    Caption = 'Retrieve'
    TabOrder = 2
    OnClick = Retrieve
  end
  object CmbAliquot1: TComboBox
    Left = 100
    Top = 79
    Width = 160
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 3
    OnDropDown = CmbAliquot1DropDown
  end
  object RadIDType: TRadioGroup
    Left = 330
    Top = 8
    Width = 264
    Height = 49
    Caption = 'Item Type'
    Columns = 3
    ItemIndex = 0
    Items.Strings = (
      'Sample'
      'Cryovial'
      'Box')
    TabOrder = 4
  end
  object CmbAliquot2: TComboBox
    Left = 434
    Top = 79
    Width = 160
    Height = 21
    Enabled = False
    Sorted = True
    TabOrder = 5
    OnDropDown = CmbAliquot2DropDown
  end
  object cbProject: TComboBox
    Left = 100
    Top = 23
    Width = 160
    Height = 21
    Sorted = True
    TabOrder = 6
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 672
    Width = 1019
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 7
    ExplicitWidth = 791
  end
  object BtnDest: TButton
    Left = 504
    Top = 140
    Width = 90
    Height = 28
    Caption = 'New Boxes'
    TabOrder = 8
    OnClick = BtnDestClick
  end
  object Button1: TButton
    Left = 170
    Top = 140
    Width = 90
    Height = 28
    Caption = 'Add specimens'
    TabOrder = 9
  end
  object OpenDialog1: TOpenDialog
    Left = 650
    Top = 27
  end
end
