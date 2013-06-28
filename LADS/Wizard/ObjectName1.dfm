object ObjectName: TObjectName
  Left = 415
  Top = 197
  Width = 459
  Height = 522
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Objects'
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
  object Panel2: TPanel
    Left = 29
    Top = 8
    Width = 393
    Height = 97
    Caption = 'Panel2'
    TabOrder = 0
    object Label1: TLabel
      Left = 9
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 159
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 159
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
  end
  object ObjectGrid: TColourStringGrid
    Left = 8
    Top = 114
    Width = 435
    Height = 332
    ColCount = 12
    FixedCols = 0
    RowCount = 2
    ScrollBars = ssVertical
    TabOrder = 1
    OnMouseDown = ObjectGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 0
    Top = 454
    Width = 451
    Height = 41
    Align = alBottom
    TabOrder = 2
    object btnClose: TButton
      Left = 40
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
    Left = 128
    Top = 336
  end
end
