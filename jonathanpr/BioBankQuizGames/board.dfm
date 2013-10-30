object fm_board: Tfm_board
  Left = 216
  Top = 447
  BorderStyle = bsNone
  Caption = 'fm_board'
  ClientHeight = 374
  ClientWidth = 1014
  Color = clBlack
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clSilver
  Font.Height = -24
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 29
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
  end
  object timer_preview: TTimer
    Enabled = False
    OnTimer = timer_previewTimer
    Left = 584
    Top = 24
  end
end
