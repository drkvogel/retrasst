object frmAliquotTypes: TfrmAliquotTypes
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'List of Samples'
  ClientHeight = 486
  ClientWidth = 622
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
    Left = 263
    Top = 8
    Width = 96
    Height = 16
    Caption = 'List of samples'
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
    Width = 606
    Height = 411
    ColCount = 4
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    TabOrder = 0
  end
  object BtnOK: TButton
    Left = 226
    Top = 453
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = OKClicked
  end
  object BtnCancel: TButton
    Left = 321
    Top = 453
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 2
  end
end
