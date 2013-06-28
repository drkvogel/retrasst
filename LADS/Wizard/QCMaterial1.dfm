object QCMaterial: TQCMaterial
  Left = 192
  Top = 111
  Width = 518
  Height = 600
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'QCMaterial'
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
  object QCMaterialGrid: TColourStringGrid
    Left = 8
    Top = 104
    Width = 497
    Height = 417
    FixedCols = 0
    TabOrder = 0
    OnMouseDown = QCMaterialGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 34
    Top = 0
    Width = 441
    Height = 97
    TabOrder = 1
    object Label3: TLabel
      Left = 33
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object AddMaterial: TButton
      Left = 32
      Top = 8
      Width = 100
      Height = 28
      Caption = 'Add Material'
      TabOrder = 0
      OnClick = AddMaterialClick
    end
    object ChangeMaterial: TButton
      Left = 32
      Top = 63
      Width = 100
      Height = 28
      Caption = 'Change Material'
      TabOrder = 1
      OnClick = ChangeMaterialClick
    end
    object AddLevel: TButton
      Left = 176
      Top = 63
      Width = 95
      Height = 28
      Caption = 'Add Level'
      TabOrder = 2
      OnClick = AddLevelClick
    end
    object ChangeLevel: TButton
      Left = 280
      Top = 63
      Width = 137
      Height = 28
      Caption = 'Change Level/Add Lot'
      TabOrder = 3
      OnClick = ChangeLevelClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 532
    Width = 510
    Height = 41
    Align = alBottom
    TabOrder = 2
    object btnClose: TButton
      Left = 16
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
    Left = 96
    Top = 308
  end
end
