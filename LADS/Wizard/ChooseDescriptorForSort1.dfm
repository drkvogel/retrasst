object ChooseDescriptorForSort: TChooseDescriptorForSort
  Left = 207
  Top = 112
  Width = 482
  Height = 329
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Choose Descriptor For Sort'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = num
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 24
    Width = 186
    Height = 13
    Caption = 'Click on the descriptor on which to sort:'
  end
  object Label2: TLabel
    Left = 240
    Top = 24
    Width = 91
    Height = 13
    Caption = 'Descriptors for sort:'
  end
  object Panel1: TPanel
    Left = 0
    Top = 261
    Width = 474
    Height = 41
    Align = alBottom
    TabOrder = 0
    object btnSort: TButton
      Left = 296
      Top = 8
      Width = 81
      Height = 25
      Caption = 'Sort'
      TabOrder = 0
      OnClick = btnSortClick
    end
    object Cancel: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object ChooseDescriptorBox: TListBox
    Left = 32
    Top = 56
    Width = 169
    Height = 97
    ItemHeight = 13
    TabOrder = 1
    OnMouseDown = ChooseDescriptorBoxMouseDown
  end
  object SortBox: TListBox
    Left = 264
    Top = 56
    Width = 169
    Height = 97
    ItemHeight = 13
    TabOrder = 2
  end
end
