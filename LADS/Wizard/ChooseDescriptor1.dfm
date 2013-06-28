object ChooseDescriptor: TChooseDescriptor
  Left = 196
  Top = 112
  Width = 302
  Height = 324
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'ChooseDescriptor'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 254
    Height = 13
    Caption = 'Click on the descriptor whose value you wish to enter.'
  end
  object ChooseDescriptorBox: TListBox
    Left = 56
    Top = 40
    Width = 169
    Height = 97
    ItemHeight = 13
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 256
    Width = 294
    Height = 41
    Align = alBottom
    TabOrder = 1
    object OK: TButton
      Left = 176
      Top = 8
      Width = 75
      Height = 25
      Caption = 'OK'
      TabOrder = 0
      OnClick = OKClick
    end
    object Cancel: TButton
      Left = 40
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
end
