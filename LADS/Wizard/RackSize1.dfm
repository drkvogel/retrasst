object RackSize: TRackSize
  Left = 197
  Top = 115
  Width = 511
  Height = 439
  Caption = 'Rack Size'
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
    Left = 55
    Top = 6
    Width = 393
    Height = 97
    TabOrder = 0
    object Label1: TLabel
      Left = 9
      Top = 42
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
  object Panel1: TPanel
    Left = 0
    Top = 371
    Width = 503
    Height = 41
    Align = alBottom
    TabOrder = 1
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
  object RackSizeGrid: TStringGrid
    Left = 3
    Top = 110
    Width = 497
    Height = 257
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 2
    OnDrawCell = RackSizeGridDrawCell
    OnMouseDown = RackSizeGridMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 56
    Top = 16
  end
end
