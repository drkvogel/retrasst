object ProfileMap: TProfileMap
  Left = 199
  Top = 115
  Width = 800
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Profile Map'
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
  object Label2: TLabel
    Left = 570
    Top = 6
    Width = 174
    Height = 13
    Caption = 'Descriptors with no choice of values:'
  end
  object Panel3: TPanel
    Left = 0
    Top = 528
    Width = 792
    Height = 45
    Align = alBottom
    TabOrder = 0
    object btnClose: TButton
      Left = 48
      Top = 8
      Width = 100
      Height = 30
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
    object btnInsertValues: TButton
      Left = 456
      Top = 8
      Width = 100
      Height = 30
      Hint = 'Insert a single value descriptor into the profile map'
      Caption = 'Insert Columns'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 1
      OnClick = btnInsertValuesClick
    end
    object btnClearValues: TButton
      Left = 600
      Top = 8
      Width = 100
      Height = 30
      Hint = 'Remove a single value descriptor from the profile map'
      Caption = 'Delete Columns'
      ParentShowHint = False
      ShowHint = True
      TabOrder = 2
      OnClick = btnClearValuesClick
    end
  end
  object Panel2: TPanel
    Left = 65
    Top = 4
    Width = 417
    Height = 109
    ParentShowHint = False
    ShowHint = False
    TabOrder = 1
    object Label1: TLabel
      Left = 21
      Top = 48
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object ChangeProfile: TButton
      Left = 66
      Top = 70
      Width = 100
      Height = 30
      Caption = 'Change Profile'
      TabOrder = 0
      OnClick = ChangeProfileClick
    end
    object DeleteRow: TButton
      Left = 251
      Top = 70
      Width = 100
      Height = 30
      Caption = 'Delete Row'
      TabOrder = 1
      OnClick = DeleteRowClick
    end
    object btnAddRowNew: TButton
      Left = 130
      Top = 8
      Width = 158
      Height = 30
      Caption = 'Add Row (individual values)'
      TabOrder = 2
      OnClick = btnAddRowNewClick
    end
  end
  object NoProfilesBox: TListBox
    Left = 570
    Top = 23
    Width = 174
    Height = 90
    ItemHeight = 13
    TabOrder = 2
  end
  object ProfileMapTable: TStringGrid
    Left = 2
    Top = 117
    Width = 789
    Height = 407
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
    TabOrder = 3
    OnDrawCell = ProfileMapTableDrawCell
    OnMouseDown = ProfileMapTableMouseDown
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 504
    Top = 8
  end
end
