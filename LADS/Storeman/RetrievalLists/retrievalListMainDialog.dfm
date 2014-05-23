object frmRetrievalListViewListMain: TfrmRetrievalListViewListMain
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Sample Finder'
  ClientHeight = 546
  ClientWidth = 584
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 8
    Width = 566
    Height = 169
    TabOrder = 0
    object Label5: TLabel
      Left = 16
      Top = 8
      Width = 170
      Height = 19
      Caption = 'Search by retrieval job'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object Label1: TLabel
      Left = 16
      Top = 86
      Width = 69
      Height = 17
      Caption = 'Job details'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label7: TLabel
      Left = 16
      Top = 36
      Width = 234
      Height = 17
      Caption = 'Retrieval/Disposal/Storage Exercise'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object retrivalListComboBox: TComboBox
      Left = 16
      Top = 109
      Width = 529
      Height = 21
      TabOrder = 0
    end
    object OpenListByNameButton: TButton
      Left = 232
      Top = 136
      Width = 87
      Height = 25
      Caption = 'Open List'
      TabOrder = 1
      OnClick = OpenListByNameButtonClick
    end
    object exerciseListComboBox: TComboBox
      Left = 16
      Top = 59
      Width = 530
      Height = 21
      TabOrder = 2
      Text = 'exerciseListComboBox'
      OnChange = exerciseListComboBoxChange
    end
  end
  object Panel2: TPanel
    Left = 8
    Top = 186
    Width = 566
    Height = 194
    TabOrder = 1
    object barCodeFileLabel: TLabel
      Left = 16
      Top = 142
      Width = 195
      Height = 16
      Caption = 'Select cryovial barcode text file'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label3: TLabel
      Left = 16
      Top = 33
      Width = 83
      Height = 16
      Caption = 'Project name'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 16
      Top = 87
      Width = 74
      Height = 16
      Caption = 'Aliquot type'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 16
      Top = 8
      Width = 367
      Height = 19
      Caption = 'Search by project, aliquot and cryovial barcodes'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object findBarCodesButton: TButton
      Left = 502
      Top = 142
      Width = 43
      Height = 25
      Caption = '...'
      TabOrder = 0
      OnClick = findBarCodesButtonClick
    end
    object ProjectNamesComboBox: TComboBox
      Left = 16
      Top = 55
      Width = 526
      Height = 21
      TabOrder = 1
      Text = 'ProjectNames'
    end
    object OpenList2Button: TButton
      Left = 232
      Top = 164
      Width = 87
      Height = 25
      Caption = 'Open List'
      TabOrder = 2
      OnClick = OpenList2ButtonClick
    end
    object aliquitListComboBox: TComboBox
      Left = 16
      Top = 109
      Width = 526
      Height = 21
      TabOrder = 3
      Text = 'Aliquot types'
    end
  end
  object Panel3: TPanel
    Left = 8
    Top = 390
    Width = 566
    Height = 146
    TabOrder = 2
    object Label6: TLabel
      Left = 16
      Top = 8
      Width = 302
      Height = 19
      Caption = 'Search by project and sample barcodes'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = [fsBold]
      ParentFont = False
    end
    object barCode3FileLabel: TLabel
      Left = 16
      Top = 95
      Width = 192
      Height = 16
      Caption = 'Select sample barcode text file'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 16
      Top = 33
      Width = 83
      Height = 16
      Caption = 'Project name'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -14
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
    end
    object findBarCodesButton3: TButton
      Left = 502
      Top = 95
      Width = 43
      Height = 25
      Caption = '...'
      TabOrder = 0
      OnClick = findBarCodesButton3Click
    end
    object OpenList3Button: TButton
      Left = 232
      Top = 113
      Width = 87
      Height = 25
      Caption = 'Open List'
      TabOrder = 1
      OnClick = OpenList3ButtonClick
    end
    object ProjectNamesComboBox3: TComboBox
      Left = 16
      Top = 55
      Width = 526
      Height = 21
      TabOrder = 2
      Text = 'ProjectNamesComboBox3'
    end
  end
  object OpenTextFileDialog1: TOpenTextFileDialog
    Left = 504
    Top = 16
  end
end
