object frmUtils: TfrmUtils
  Left = 227
  Top = 468
  Width = 1201
  Height = 284
  Caption = 'debug'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1193
    Height = 41
    Align = alTop
    Color = 16761056
    TabOrder = 0
    object Label1: TLabel
      Left = 520
      Top = 16
      Width = 26
      Height = 13
      Caption = 'errors'
    end
    object Label2: TLabel
      Left = 8
      Top = 16
      Width = 30
      Height = 13
      Caption = 'debug'
    end
  end
  object MemoDebug: TMemo
    Left = 0
    Top = 41
    Width = 686
    Height = 216
    Align = alLeft
    Anchors = [akLeft, akTop, akRight, akBottom]
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS SystemEx'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object MemoErr: TMemo
    Left = 686
    Top = 41
    Width = 507
    Height = 216
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS SystemEx'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 2
  end
end
