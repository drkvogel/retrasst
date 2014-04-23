object frmNewJob: TfrmNewJob
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Create New Job'
  ClientHeight = 198
  ClientWidth = 438
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  Scaled = False
  DesignSize = (
    438
    198)
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 16
    Top = 71
    Width = 57
    Height = 13
    Anchors = [akLeft]
    Caption = 'Description:'
  end
  object LblName: TLabel
    Left = 16
    Top = 29
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object LblWhy: TLabel
    Left = 16
    Top = 155
    Width = 44
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Exercise:'
    ExplicitTop = 106
  end
  object LblReason: TLabel
    Left = 16
    Top = 113
    Width = 40
    Height = 13
    Anchors = [akLeft]
    Caption = 'Reason:'
  end
  object BtnOK: TButton
    Left = 328
    Top = 23
    Width = 90
    Height = 28
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    TabOrder = 5
    OnClick = SaveClick
  end
  object TxtFull: TEdit
    Left = 90
    Top = 68
    Width = 322
    Height = 21
    Anchors = [akTop, akRight]
    MaxLength = 120
    TabOrder = 1
  end
  object TxtName: TEdit
    Left = 90
    Top = 26
    Width = 210
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 30
    TabOrder = 0
  end
  object CbExercise: TComboBox
    Left = 90
    Top = 152
    Width = 210
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 3
    Text = '(none)'
  end
  object BtnNewEx: TButton
    Left = 328
    Top = 149
    Width = 90
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'New Exercise'
    TabOrder = 4
    OnClick = BtnNewExClick
  end
  object TxtReason: TEdit
    Left = 90
    Top = 110
    Width = 322
    Height = 21
    Anchors = [akTop, akRight]
    MaxLength = 120
    TabOrder = 2
  end
end
