object frmNewJob: TfrmNewJob
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Create New Job'
  ClientHeight = 149
  ClientWidth = 428
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  DesignSize = (
    428
    149)
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 16
    Top = 68
    Width = 57
    Height = 13
    Anchors = [akLeft]
    Caption = 'Description:'
    ExplicitTop = 67
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
    Top = 106
    Width = 44
    Height = 13
    Anchors = [akLeft, akBottom]
    Caption = 'Exercise:'
  end
  object BtnOK: TButton
    Left = 318
    Top = 23
    Width = 90
    Height = 28
    Anchors = [akTop, akRight]
    Caption = 'OK'
    Default = True
    TabOrder = 3
    OnClick = SaveClick
  end
  object TxtFull: TEdit
    Left = 86
    Top = 65
    Width = 322
    Height = 21
    Anchors = [akTop, akRight]
    MaxLength = 120
    TabOrder = 1
  end
  object TxtName: TEdit
    Left = 86
    Top = 26
    Width = 200
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 30
    TabOrder = 0
  end
  object CbExercise: TComboBox
    Left = 86
    Top = 103
    Width = 200
    Height = 21
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 2
    Text = '(none)'
  end
  object BtnNewEx: TButton
    Left = 318
    Top = 100
    Width = 90
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'New Exercise'
    TabOrder = 4
    OnClick = BtnNewExClick
  end
end
