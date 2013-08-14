object frmRetrieveMain: TfrmRetrieveMain
  Left = 0
  Top = 0
  Caption = 'Retrieve Sample'
  ClientHeight = 689
  ClientWidth = 791
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  DesignSize = (
    791
    689)
  PixelsPerInch = 96
  TextHeight = 13
  object LblCaption: TLabel
    Left = 213
    Top = 179
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
  object LblAliquot1: TLabel
    Left = 12
    Top = 116
    Width = 75
    Height = 13
    Caption = 'Primary aliquot:'
  end
  object LblAliquot2: TLabel
    Left = 306
    Top = 116
    Width = 90
    Height = 13
    Caption = 'Secondary aliquot:'
  end
  object Label1: TLabel
    Left = 12
    Top = 20
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 209
    Width = 775
    Height = 457
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
    Top = 156
    Width = 75
    Height = 25
    Caption = 'Add'
    TabOrder = 1
    OnClick = AddClick
  end
  object BtnRetrieve: TButton
    Left = 471
    Top = 156
    Width = 75
    Height = 25
    Caption = 'Retrieve'
    TabOrder = 2
    OnClick = Retrieve
  end
  object CmbAliquot1: TComboBox
    Left = 106
    Top = 113
    Width = 146
    Height = 21
    Sorted = True
    TabOrder = 3
    OnDropDown = cbaDropDown
  end
  object RadIDType: TRadioGroup
    Left = 10
    Top = 51
    Width = 240
    Height = 38
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
    Left = 410
    Top = 113
    Width = 136
    Height = 21
    Sorted = True
    TabOrder = 5
    OnDropDown = cbaDropDown
  end
  object RadioGroup1: TRadioGroup
    Left = 306
    Top = 51
    Width = 240
    Height = 38
    Caption = 'Source'
    Columns = 2
    ItemIndex = 0
    Items.Strings = (
      'Text file'
      'Specimen table')
    TabOrder = 6
  end
  object cbProject: TComboBox
    Left = 106
    Top = 17
    Width = 146
    Height = 21
    Sorted = True
    TabOrder = 7
    OnChange = cbProjectChange
    OnDropDown = cbProjectDropDown
  end
  object progress: TProgressBar
    Left = 0
    Top = 672
    Width = 791
    Height = 17
    Align = alBottom
    Step = 1
    TabOrder = 8
    ExplicitLeft = 688
    ExplicitTop = 688
    ExplicitWidth = 150
  end
  object BtnDest: TButton
    Left = 640
    Top = 72
    Width = 75
    Height = 25
    Caption = 'New Boxes'
    TabOrder = 9
    OnClick = BtnDestClick
  end
  object OpenDialog1: TOpenDialog
    Left = 130
    Top = 155
  end
end
