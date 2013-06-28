object BoxContent: TBoxContent
  Left = 190
  Top = 327
  Width = 795
  Height = 560
  Caption = 'Box Content'
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
    Left = 197
    Top = 7
    Width = 393
    Height = 97
    TabOrder = 0
    object Label1: TLabel
      Left = 8
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
  object Panel1: TPanel
    Left = 0
    Top = 492
    Width = 787
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
  object BoxContentGrid: TStringGrid
    Left = 4
    Top = 112
    Width = 780
    Height = 377
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 2
    OnDrawCell = BoxContentGridDrawCell
    OnMouseDown = BoxContentGridMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 72
    Top = 48
  end
end
