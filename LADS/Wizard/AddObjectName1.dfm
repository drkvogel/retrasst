object AddObjectName: TAddObjectName
  Left = 192
  Top = 111
  Width = 356
  Height = 420
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Objects'
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
  object Label11: TLabel
    Left = 12
    Top = 60
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label1: TLabel
    Left = 12
    Top = 24
    Width = 63
    Height = 13
    Caption = 'Object name:'
  end
  object edtObjectName: TEdit
    Left = 90
    Top = 18
    Width = 121
    Height = 21
    TabOrder = 0
    OnExit = edtObjectNameExit
  end
  object edtFullName: TEdit
    Left = 90
    Top = 56
    Width = 245
    Height = 21
    MaxLength = 56
    TabOrder = 2
  end
  object Panel1: TPanel
    Left = 19
    Top = 95
    Width = 310
    Height = 41
    TabOrder = 3
    object Save: TButton
      Left = 185
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 49
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object Panel2: TPanel
    Left = 41
    Top = 145
    Width = 267
    Height = 200
    TabOrder = 4
    object Add: TButton
      Left = 44
      Top = 10
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Delete: TButton
      Left = 148
      Top = 10
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 1
      OnClick = DeleteClick
    end
    object linkedGrid: TColourStringGrid
      Left = 54
      Top = 45
      Width = 158
      Height = 145
      ScrollBars = ssVertical
      TabOrder = 2
      OnMouseDown = linkedGridMouseDown
      FirstBitmapLeft = 92
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 352
    Width = 348
    Height = 41
    Align = alBottom
    TabOrder = 5
    object btnClose: TButton
      Left = 32
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object cbActive: TCheckBox
    Left = 251
    Top = 20
    Width = 60
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 1
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 248
    Top = 336
  end
end
