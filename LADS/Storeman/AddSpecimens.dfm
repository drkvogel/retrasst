object frmAddSpecimens: TfrmAddSpecimens
  Left = 0
  Top = 0
  Caption = 'Add Samples for Analysis'
  ClientHeight = 742
  ClientWidth = 1071
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  OnHide = FormHide
  OnShow = FormShow
  DesignSize = (
    1071
    742)
  PixelsPerInch = 96
  TextHeight = 13
  inline selector: TselectorFrame
    Left = 8
    Top = 14
    Width = 430
    Height = 120
    TabOrder = 0
    ExplicitLeft = 8
    ExplicitTop = 14
  end
  object sgSpecimens: TStringGrid
    Left = 8
    Top = 152
    Width = 1055
    Height = 582
    Anchors = [akLeft, akTop, akRight, akBottom]
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing]
    TabOrder = 1
  end
  object BitBtn2: TBitBtn
    Left = 464
    Top = 24
    Width = 102
    Height = 38
    Caption = 'Save'
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 2
    OnClick = BitBtn2Click
  end
  object BitBtn1: TBitBtn
    Left = 464
    Top = 80
    Width = 102
    Height = 38
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 3
  end
  object sgProfiles: TStringGrid
    Left = 590
    Top = 16
    Width = 470
    Height = 120
    Anchors = [akLeft, akTop, akRight]
    ColCount = 2
    DefaultColWidth = 72
    FixedCols = 0
    TabOrder = 4
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 5000
    OnTimer = Timer1Timer
    Left = 776
    Top = 312
  end
end
