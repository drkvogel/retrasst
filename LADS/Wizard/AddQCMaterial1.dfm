object AddQCMaterial: TAddQCMaterial
  Left = 192
  Top = 111
  Width = 323
  Height = 169
  BorderIcons = []
  Caption = 'AddQCMaterial'
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
  object lblCode: TLabel
    Left = 24
    Top = 24
    Width = 40
    Height = 13
    Caption = 'Material:'
  end
  object lblDescrip: TLabel
    Left = 24
    Top = 64
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object edtMaterial: TEdit
    Left = 104
    Top = 16
    Width = 25
    Height = 21
    TabOrder = 0
  end
  object edtDescrip: TEdit
    Left = 104
    Top = 56
    Width = 185
    Height = 21
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 101
    Width = 315
    Height = 41
    Align = alBottom
    TabOrder = 2
    object btnCancel: TButton
      Left = 48
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
    end
    object btnSave: TButton
      Left = 192
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnSaveClick
    end
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 208
    Top = 16
  end
end
