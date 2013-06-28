object AddQCLot: TAddQCLot
  Left = 202
  Top = 111
  Width = 398
  Height = 600
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'AddQCLot'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 8
    Width = 40
    Height = 13
    Caption = 'Material:'
  end
  object lblMaterial: TLabel
    Left = 104
    Top = 8
    Width = 3
    Height = 13
  end
  object lblCode: TLabel
    Left = 192
    Top = 8
    Width = 29
    Height = 13
    Caption = 'Level:'
  end
  object lblDescrip: TLabel
    Left = 24
    Top = 72
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object lblLevel: TLabel
    Left = 240
    Top = 8
    Width = 3
    Height = 13
  end
  object Label2: TLabel
    Left = 24
    Top = 36
    Width = 18
    Height = 13
    Caption = 'Lot:'
  end
  object Label4: TLabel
    Left = 192
    Top = 36
    Width = 58
    Height = 13
    Caption = 'Arrival Date:'
  end
  object edtLot: TEdit
    Left = 104
    Top = 32
    Width = 33
    Height = 21
    TabOrder = 0
  end
  object edtDescrip: TEdit
    Left = 104
    Top = 64
    Width = 185
    Height = 21
    TabOrder = 1
  end
  object pnlTest: TPanel
    Left = 2
    Top = 160
    Width = 385
    Height = 97
    TabOrder = 2
    object Label3: TLabel
      Left = 5
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object btnAddTest: TButton
      Left = 54
      Top = 8
      Width = 100
      Height = 27
      Caption = 'Add Test'
      TabOrder = 0
      OnClick = btnAddTestClick
    end
    object btnChangeTest: TButton
      Left = 54
      Top = 62
      Width = 100
      Height = 27
      Caption = 'Change Test'
      TabOrder = 1
      OnClick = btnChangeTestClick
    end
    object btnCopyTests: TButton
      Left = 211
      Top = 8
      Width = 129
      Height = 27
      Caption = 'Copy Tests from Lot x'
      TabOrder = 2
      OnClick = btnCopyTestsClick
    end
    object btnDeleteTest: TButton
      Left = 225
      Top = 62
      Width = 100
      Height = 27
      Caption = 'Delete Test'
      TabOrder = 3
      OnClick = btnDeleteTestClick
    end
  end
  object QCLotGrid: TColourStringGrid
    Left = 46
    Top = 264
    Width = 297
    Height = 257
    FixedCols = 0
    TabOrder = 3
    OnMouseDown = QCLotGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 0
    Top = 100
    Width = 385
    Height = 41
    TabOrder = 4
    object btnCancel: TButton
      Left = 48
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
    end
    object btnSave: TButton
      Left = 240
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnSaveClick
    end
  end
  object edtArrivalDate: TEdit
    Left = 256
    Top = 32
    Width = 121
    Height = 21
    TabOrder = 5
  end
  object Panel2: TPanel
    Left = 0
    Top = 532
    Width = 390
    Height = 41
    Align = alBottom
    TabOrder = 6
    object btnClose: TButton
      Left = 48
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 344
    Top = 304
  end
end
