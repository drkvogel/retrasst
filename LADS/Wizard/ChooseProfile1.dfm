object ChooseProfile: TChooseProfile
  Left = 196
  Top = 112
  Width = 350
  Height = 456
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Choose Profile'
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
  object Panel1: TPanel
    Left = 0
    Top = 388
    Width = 342
    Height = 41
    Align = alBottom
    TabOrder = 0
    object Cancel: TButton
      Left = 46
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = CancelClick
    end
    object Save: TButton
      Left = 222
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = SaveClick
    end
  end
  object ChooseProfileBox: TListBox
    Left = 2
    Top = 6
    Width = 338
    Height = 376
    ItemHeight = 13
    TabOrder = 1
  end
end
