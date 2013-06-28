object Project: TProject
  Left = 192
  Top = 111
  Width = 800
  Height = 600
  BorderIcons = []
  Caption = 'Project'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 199
    Top = 4
    Width = 393
    Height = 100
    TabOrder = 0
    object Label1: TLabel
      Left = 9
      Top = 43
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 159
      Top = 10
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 159
      Top = 65
      Width = 75
      Height = 25
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
  end
  object ProjectTable: TColourStringGrid
    Left = 5
    Top = 108
    Width = 782
    Height = 420
    ColCount = 12
    FixedCols = 0
    RowCount = 2
    ScrollBars = ssVertical
    TabOrder = 1
    OnMouseDown = ProjectTableMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 0
    Top = 532
    Width = 792
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
    Left = 80
    Top = 32
  end
end
