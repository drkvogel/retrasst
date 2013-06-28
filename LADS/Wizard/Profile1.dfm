object Profile: TProfile
  Left = 103
  Top = 235
  Width = 800
  Height = 600
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Test Profile'
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
    Left = 169
    Top = 6
    Width = 406
    Height = 97
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 153
      Top = 8
      Width = 100
      Height = 26
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 153
      Top = 63
      Width = 100
      Height = 26
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 532
    Width = 792
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
  object btnReport: TButton
    Left = 630
    Top = 16
    Width = 100
    Height = 26
    Caption = 'Report'
    TabOrder = 2
    OnClick = btnReportClick
  end
  object ProfileTable: TStringGrid
    Left = 1
    Top = 109
    Width = 790
    Height = 420
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    ScrollBars = ssVertical
    TabOrder = 3
    OnDrawCell = ProfileTableDrawCell
    OnMouseDown = ProfileTableMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 16
    Top = 24
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 56
    Top = 24
  end
  object QueryProjectDdb: TQuery
    DatabaseName = 'ddbData'
    Left = 96
    Top = 24
  end
end
