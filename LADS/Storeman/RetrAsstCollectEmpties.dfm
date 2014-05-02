object frmCollectEmpties: TfrmCollectEmpties
  Left = 0
  Top = 0
  Caption = 'Collect Empty Boxes'
  ClientHeight = 446
  ClientWidth = 810
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object groupBoxes: TGroupBox
    Left = 0
    Top = 0
    Width = 810
    Height = 405
    Align = alClient
    Caption = 'Boxes'
    TabOrder = 0
    ExplicitLeft = 208
    ExplicitTop = 248
    ExplicitWidth = 185
    ExplicitHeight = 105
    object StringGrid1: TStringGrid
      Left = 2
      Top = 15
      Width = 806
      Height = 388
      Align = alClient
      FixedCols = 0
      TabOrder = 0
      ExplicitLeft = 184
      ExplicitTop = 96
      ExplicitWidth = 320
      ExplicitHeight = 120
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 405
    Width = 810
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 496
    ExplicitTop = 104
    ExplicitWidth = 185
    object btnClose: TButton
      Left = 592
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
