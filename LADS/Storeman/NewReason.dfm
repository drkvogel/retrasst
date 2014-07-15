object frmNewReason: TfrmNewReason
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Add new reason for retrieval'
  ClientHeight = 171
  ClientWidth = 434
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 16
    Top = 127
    Width = 57
    Height = 13
    Caption = 'Description:'
  end
  object LblName: TLabel
    Left = 16
    Top = 79
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object lblReasonGroup: TLabel
    Left = 88
    Top = 32
    Width = 5
    Height = 13
    Caption = '?'
  end
  object Label2: TLabel
    Left = 16
    Top = 31
    Width = 28
    Height = 13
    Caption = 'Type:'
  end
  object BtnOK: TButton
    Left = 331
    Top = 73
    Width = 82
    Height = 28
    Caption = 'OK'
    Default = True
    TabOrder = 2
    OnClick = SaveClick
  end
  object TxtFull: TEdit
    Left = 88
    Top = 124
    Width = 325
    Height = 21
    TabOrder = 1
  end
  object TxtName: TEdit
    Left = 88
    Top = 76
    Width = 209
    Height = 21
    TabOrder = 0
  end
end
