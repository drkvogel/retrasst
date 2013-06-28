object frmNewJob: TfrmNewJob
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Create New Job'
  ClientHeight = 147
  ClientWidth = 428
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  OnShow = FormShow
  DesignSize = (
    428
    147)
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 16
    Top = 68
    Width = 57
    Height = 13
    Anchors = [akLeft]
    Caption = 'Description:'
  end
  object LblName: TLabel
    Left = 16
    Top = 33
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object LblWhy: TLabel
    Left = 16
    Top = 105
    Width = 40
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Reason:'
  end
  object BtnOK: TButton
    Left = 318
    Top = 25
    Width = 90
    Height = 28
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = SaveClick
  end
  object TxtFull: TEdit
    Left = 90
    Top = 65
    Width = 322
    Height = 21
    Anchors = [akLeft, akRight]
    TabOrder = 1
  end
  object TxtName: TEdit
    Left = 90
    Top = 29
    Width = 175
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
  end
  object TxtReason: TEdit
    Left = 90
    Top = 102
    Width = 322
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 2
  end
end
