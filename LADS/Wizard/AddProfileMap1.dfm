object AddProfileMap: TAddProfileMap
  Left = 201
  Top = 115
  Width = 800
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Profile Map'
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
  object Panel3: TPanel
    Left = 0
    Top = 529
    Width = 792
    Height = 44
    Align = alBottom
    TabOrder = 0
    object btnAdd: TButton
      Left = 184
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = btnAddClick
    end
    object Cancel: TButton
      Left = 40
      Top = 9
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object AddProfileMapTable: TColourStringGrid
    Left = 8
    Top = 56
    Width = 761
    Height = 465
    FixedCols = 0
    ScrollBars = ssVertical
    TabOrder = 1
    OnMouseDown = AddProfileMapTableMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 120
    Top = 8
    Width = 305
    Height = 41
    TabOrder = 2
    object Label1: TLabel
      Left = 17
      Top = 14
      Width = 271
      Height = 13
      Caption = 'Click the rows you wish to add, then click the Add button.'
    end
  end
  object QueryProject: TQuery
    DatabaseName = 'project'
    Left = 416
    Top = 176
  end
end
