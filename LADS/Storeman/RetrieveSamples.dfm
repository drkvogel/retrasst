object frmRetrieved: TfrmRetrieved
  Left = 0
  Top = 0
  Caption = 'List of retrieved samples'
  ClientHeight = 490
  ClientWidth = 636
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object LblCaption: TLabel
    Left = 237
    Top = 8
    Width = 161
    Height = 16
    Caption = 'List of retrieved samples'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object grdSamples: TStringGrid
    Left = 8
    Top = 30
    Width = 620
    Height = 420
    ColCount = 6
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
    TabOrder = 0
  end
  object btnOK: TButton
    Left = 280
    Top = 457
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 1
  end
end
