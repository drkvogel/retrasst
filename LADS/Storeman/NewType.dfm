object frmNewStoreType: TfrmNewStoreType
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Configure Storage Type'
  ClientHeight = 124
  ClientWidth = 454
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poOwnerFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 16
    Top = 80
    Width = 57
    Height = 13
    Caption = 'Description:'
  end
  object LblName: TLabel
    Left = 16
    Top = 32
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object BtnOK: TButton
    Left = 340
    Top = 26
    Width = 82
    Height = 28
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = SaveClick
  end
  object TxtFull: TEdit
    Left = 88
    Top = 76
    Width = 334
    Height = 21
    TabOrder = 1
  end
  object TxtName: TEdit
    Left = 88
    Top = 29
    Width = 185
    Height = 21
    TabOrder = 0
  end
end
