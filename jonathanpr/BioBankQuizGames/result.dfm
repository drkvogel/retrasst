object fm_result: Tfm_result
  Left = 414
  Top = 252
  BorderStyle = bsNone
  Caption = 'fm_result'
  ClientHeight = 67
  ClientWidth = 175
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object panel_outcome: TPanel
    Left = 0
    Top = 0
    Width = 175
    Height = 67
    Align = alClient
    BevelInner = bvRaised
    BevelOuter = bvLowered
    BorderWidth = 2
    Caption = 'panel_outcome'
    Color = clMaroon
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -48
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
  end
  object timer_result: TTimer
    Enabled = False
    OnTimer = timer_resultTimer
    Left = 32
    Top = 16
  end
end
