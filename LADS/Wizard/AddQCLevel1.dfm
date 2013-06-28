object AddQCLevel: TAddQCLevel
  Left = 192
  Top = 111
  Width = 403
  Height = 600
  BorderIcons = []
  Caption = 'AddQCLevel'
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
  object lblCode: TLabel
    Left = 24
    Top = 48
    Width = 29
    Height = 13
    Caption = 'Level:'
  end
  object lblDescrip: TLabel
    Left = 24
    Top = 77
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 40
    Height = 13
    Caption = 'Material:'
  end
  object lblMaterial: TLabel
    Left = 102
    Top = 16
    Width = 3
    Height = 13
  end
  object edtLevel: TEdit
    Left = 102
    Top = 45
    Width = 25
    Height = 21
    TabOrder = 0
  end
  object edtDescrip: TEdit
    Left = 102
    Top = 73
    Width = 185
    Height = 21
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 6
    Top = 108
    Width = 383
    Height = 41
    TabOrder = 2
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
  object QCLevelGrid: TColourStringGrid
    Left = 45
    Top = 264
    Width = 304
    Height = 249
    FixedCols = 0
    TabOrder = 3
    OnMouseDown = QCLevelGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel2: TPanel
    Left = 1
    Top = 152
    Width = 393
    Height = 97
    TabOrder = 4
    object Label3: TLabel
      Left = 9
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object btnAddLot: TButton
      Left = 159
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add Lot'
      TabOrder = 0
      OnClick = btnAddLotClick
    end
    object btnChangeLot: TButton
      Left = 148
      Top = 64
      Width = 97
      Height = 25
      Caption = 'Change Lot'
      TabOrder = 1
      OnClick = btnChangeLotClick
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 532
    Width = 395
    Height = 41
    Align = alBottom
    TabOrder = 5
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
    Left = 336
    Top = 344
  end
end
