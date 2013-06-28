object Operator: TOperator
  Left = 160
  Top = 143
  Width = 520
  Height = 565
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Operator'
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
    Left = 59
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
    Top = 497
    Width = 512
    Height = 41
    Align = alBottom
    TabOrder = 1
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
  object OperatorTable: TStringGrid
    Left = 1
    Top = 108
    Width = 510
    Height = 385
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 2
    OnDrawCell = OperatorTableDrawCell
    OnMouseDown = OperatorTableMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 24
    Top = 32
  end
end
