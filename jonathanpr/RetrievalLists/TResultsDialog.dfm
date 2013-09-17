object ResultsDialog: TResultsDialog
  Left = 0
  Top = 0
  Caption = 'Analyses on the sample'
  ClientHeight = 282
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object ResultStringGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 418
    Height = 282
    Align = alClient
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goFixedRowClick]
    TabOrder = 0
    OnMouseDown = ResultStringGridMouseDown
    OnSelectCell = ResultStringGridSelectCell
  end
end
