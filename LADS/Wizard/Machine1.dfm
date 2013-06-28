object Machine: TMachine
  Left = 244
  Top = 112
  Width = 772
  Height = 600
  BorderIcons = []
  Caption = 'Machine'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 182
    Top = 4
    Width = 400
    Height = 100
    TabOrder = 0
    object Label1: TLabel
      Left = 13
      Top = 43
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 233
      Top = 8
      Width = 100
      Height = 30
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 150
      Top = 63
      Width = 100
      Height = 30
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
    object PreviewReport: TButton
      Left = 67
      Top = 7
      Width = 100
      Height = 30
      Caption = 'Preview Report'
      TabOrder = 2
      OnClick = PreviewReportClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 532
    Width = 764
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
  object MachineTable: TStringGrid
    Left = 6
    Top = 109
    Width = 753
    Height = 420
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 2
    OnDrawCell = MachineTableDrawCell
    OnMouseDown = MachineTableMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 32
    Top = 32
  end
end
