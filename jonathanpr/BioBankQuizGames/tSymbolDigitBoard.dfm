object symbolDigitBoard: TsymbolDigitBoard
  Left = 216
  Top = 447
  BorderStyle = bsNone
  Caption = 'symbolDigitBoard'
  ClientHeight = 374
  ClientWidth = 1014
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object panel_abandon: TPanel
    Left = 616
    Top = 144
    Width = 97
    Height = 41
    Caption = 'Abandon'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clSilver
    Font.Height = -19
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    OnClick = panel_abandonClick
  end
  object panel_play: TPanel
    Left = 16
    Top = 16
    Width = 553
    Height = 265
    BevelOuter = bvLowered
    Color = clGray
    TabOrder = 1
    object image_play: TImage
      Left = 8
      Top = 8
      Width = 329
      Height = 169
      OnMouseDown = image_playMouseDown
    end
    object NextButton: TButton
      Left = 224
      Top = 208
      Width = 89
      Height = 41
      Caption = 'Next'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -24
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = NextButtonClick
    end
  end
  object gameTimer: TTimer
    Enabled = False
    OnTimer = gameTimerTimer
    Left = 696
    Top = 240
  end
  object flashTimer: TTimer
    Enabled = False
    OnTimer = flashTimerTimer
    Left = 696
    Top = 304
  end
  object SaveTextFileDialog: TSaveTextFileDialog
    Left = 824
    Top = 184
  end
end
