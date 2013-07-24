object frmRetrievalAssistant: TfrmRetrievalAssistant
  Left = 0
  Top = 0
  Caption = 'Retrieval Assistant'
  ClientHeight = 504
  ClientWidth = 710
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    710
    504)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 702
    Height = 354
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Jobs'
    TabOrder = 0
    object sgJobs: TStringGrid
      Left = 2
      Top = 15
      Width = 698
      Height = 337
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      ColCount = 8
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goColSizing, goRowSelect]
      TabOrder = 0
      OnDrawCell = sgJobsDrawCell
      ColWidths = (
        196
        64
        64
        64
        64
        64
        64
        64)
    end
  end
  object groupStatusCheck: TGroupBox
    Left = 40
    Top = 392
    Width = 185
    Height = 105
    Caption = 'Status'
    TabOrder = 1
  end
  object groupRetrievalTypeCheck: TGroupBox
    Left = 360
    Top = 392
    Width = 185
    Height = 105
    Caption = 'Retrieval Type'
    TabOrder = 2
    object cbBox: TCheckBox
      Left = 32
      Top = 24
      Width = 153
      Height = 17
      Caption = 'Box'
      TabOrder = 0
    end
    object cbSample: TCheckBox
      Left = 32
      Top = 56
      Width = 97
      Height = 17
      Caption = 'Sample'
      TabOrder = 1
    end
  end
end
