object frmNewJob: TfrmNewJob
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Create New Job'
  ClientHeight = 245
  ClientWidth = 426
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
    426
    245)
  PixelsPerInch = 96
  TextHeight = 13
  object LblFull: TLabel
    Left = 12
    Top = 115
    Width = 57
    Height = 13
    Caption = 'Description:'
  end
  object LblName: TLabel
    Left = 12
    Top = 29
    Width = 50
    Height = 13
    Caption = 'Job name:'
  end
  object LblWhy: TLabel
    Left = 12
    Top = 72
    Width = 47
    Height = 13
    Caption = 'Retrieval:'
  end
  object LblReason: TLabel
    Left = 12
    Top = 159
    Width = 40
    Height = 13
    Caption = 'Reason:'
  end
  object Label1: TLabel
    Left = 297
    Top = 29
    Width = 40
    Height = 13
    Anchors = [akTop, akRight]
    Caption = 'Box set:'
    ExplicitLeft = 319
  end
  object TxtFull: TEdit
    Left = 80
    Top = 112
    Width = 327
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 120
    TabOrder = 4
    ExplicitWidth = 350
  end
  object TxtName: TEdit
    Left = 80
    Top = 26
    Width = 178
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 30
    TabOrder = 0
    ExplicitWidth = 201
  end
  object CbExercise: TComboBox
    Left = 80
    Top = 69
    Width = 219
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    Text = '(none)'
    ExplicitWidth = 241
  end
  object BtnNewEx: TButton
    Left = 320
    Top = 64
    Width = 90
    Height = 32
    Anchors = [akTop, akRight]
    Caption = 'New Retrieval'
    TabOrder = 3
    OnClick = BtnNewExClick
    ExplicitLeft = 342
  end
  object cmbReason: TComboBox
    Left = 80
    Top = 156
    Width = 327
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 5
    ExplicitWidth = 350
  end
  object txtBoxSet: TEdit
    Left = 350
    Top = 26
    Width = 57
    Height = 21
    Anchors = [akTop, akRight]
    MaxLength = 30
    TabOrder = 1
    ExplicitLeft = 372
  end
  object BitBtn1: TBitBtn
    Left = 97
    Top = 196
    Width = 92
    Height = 34
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 6
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 237
    Top = 196
    Width = 92
    Height = 34
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 7
  end
end
