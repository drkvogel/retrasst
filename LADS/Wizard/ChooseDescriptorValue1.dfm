object ChooseDescriptorValue: TChooseDescriptorValue
  Left = 192
  Top = 111
  Width = 302
  Height = 324
  BorderIcons = []
  Caption = 'ChooseDescriptorValue'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 16
    Width = 228
    Height = 13
    Caption = 'Click on the value you wish to use for descriptor:'
  end
  object lblDescriptor: TLabel
    Left = 96
    Top = 40
    Width = 3
    Height = 13
  end
  object lbxDescriptorValue: TListBox
    Left = 56
    Top = 112
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
  object memMapInfo: TMemo
    Left = 40
    Top = 64
    Width = 201
    Height = 41
    BorderStyle = bsNone
    Color = clScrollBar
    Lines.Strings = (
      '')
    TabOrder = 2
  end
end
