object fmCommon: TfmCommon
  Left = 212
  Top = 636
  Caption = 'debug - common'
  ClientHeight = 274
  ClientWidth = 810
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object MemoDebug: TMemo
    Left = 0
    Top = 41
    Width = 545
    Height = 233
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 810
    Height = 41
    Align = alTop
    TabOrder = 1
    object btnMark: TButton
      Left = 85
      Top = 10
      Width = 75
      Height = 25
      Caption = 'mark'
      TabOrder = 0
    end
  end
  object MemoErr: TMemo
    Left = 545
    Top = 41
    Width = 265
    Height = 233
    Align = alRight
    ScrollBars = ssVertical
    TabOrder = 2
  end
end
