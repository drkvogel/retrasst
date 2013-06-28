object frmResolve: TfrmResolve
  Left = 0
  Top = 0
  Caption = 'BoxBridge: Resolve Errors'
  ClientHeight = 370
  ClientWidth = 496
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 496
    Height = 328
    Align = alClient
    Caption = 'No Errors.'
    TabOrder = 3
  end
  object sgErrors: TStringGrid
    Left = 0
    Top = 0
    Width = 496
    Height = 328
    Align = alClient
    ColCount = 15
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 328
    Width = 496
    Height = 42
    Align = alBottom
    TabOrder = 2
  end
  object btnExit: TButton
    Left = 192
    Top = 337
    Width = 75
    Height = 25
    Caption = 'Dismiss'
    TabOrder = 0
    OnClick = btnExitClick
  end
end
