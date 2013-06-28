object frmPrompt: TfrmPrompt
  Left = 192
  Top = 107
  Width = 338
  Height = 168
  BorderIcons = []
  Caption = 'Prompt'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object lblPrompt: TLabel
    Left = 48
    Top = 48
    Width = 3
    Height = 13
  end
  object edtValue: TEdit
    Left = 160
    Top = 40
    Width = 121
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 100
    Width = 330
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btnClose: TButton
      Left = 40
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
