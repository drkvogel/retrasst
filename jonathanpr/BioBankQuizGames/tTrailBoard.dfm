object TrailBoard: TTrailBoard
  Left = 216
  Top = 447
  BorderStyle = bsNone
  Caption = 'TrailBoard'
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
  object panel_play: TPanel
    Left = 16
    Top = 16
    Width = 553
    Height = 265
    BevelOuter = bvLowered
    Color = clGray
    TabOrder = 0
    object image_play: TImage
      Left = 8
      Top = 8
      Width = 329
      Height = 169
      OnMouseDown = image_playMouseDown
    end
    object NextButton: TButton
      Left = 312
      Top = 216
      Width = 75
      Height = 25
      Caption = 'Next'
      TabOrder = 0
      OnClick = NextButtonClick
    end
  end
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
    TabOrder = 1
    OnClick = panel_abandonClick
    OnMouseDown = image_playMouseDown
  end
  object bgFlashTimer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = bgFlashTimerTimer
    Left = 680
    Top = 288
  end
  object SaveTextFileDialog: TSaveTextFileDialog
    Left = 760
    Top = 312
  end
end
