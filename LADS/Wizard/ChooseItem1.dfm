object ChooseItem: TChooseItem
  Left = 351
  Top = 122
  Width = 229
  Height = 600
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Choose Item'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesktopCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object ItemGrid: TColourStringGrid
    Left = 26
    Top = 8
    Width = 169
    Height = 513
    ColCount = 1
    DefaultColWidth = 145
    DefaultRowHeight = 18
    FixedCols = 0
    FixedRows = 0
    ScrollBars = ssVertical
    TabOrder = 0
    OnMouseDown = ItemGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel1: TPanel
    Left = 0
    Top = 532
    Width = 221
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btnClose: TButton
      Left = 16
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
