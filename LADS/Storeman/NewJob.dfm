object frmNewJob: TfrmNewJob
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Create New Job'
  ClientHeight = 245
  ClientWidth = 425
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
    425
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
    Top = 71
    Width = 50
    Height = 13
    Caption = 'Job name:'
  end
  object LblReason: TLabel
    Left = 12
    Top = 159
    Width = 40
    Height = 13
    Caption = 'Reason:'
  end
  object Label1: TLabel
    Left = 296
    Top = 71
    Width = 40
    Height = 13
    Anchors = [akTop, akRight]
    Caption = 'Box set:'
    ExplicitLeft = 297
  end
  object Label2: TLabel
    Left = 12
    Top = 29
    Width = 44
    Height = 13
    Caption = 'Exercise:'
  end
  object lblExercise: TLabel
    Left = 80
    Top = 29
    Width = 16
    Height = 13
    Anchors = [akLeft, akTop, akRight]
    Caption = 'n/a'
  end
  object TxtFull: TEdit
    Left = 80
    Top = 112
    Width = 330
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 120
    TabOrder = 2
  end
  object TxtName: TEdit
    Left = 80
    Top = 68
    Width = 178
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    MaxLength = 30
    TabOrder = 0
  end
  object cmbReason: TComboBox
    Left = 80
    Top = 156
    Width = 233
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 12
    TabOrder = 3
  end
  object txtBoxSet: TEdit
    Left = 353
    Top = 68
    Width = 57
    Height = 21
    Anchors = [akTop, akRight]
    MaxLength = 30
    TabOrder = 1
  end
  object BitBtn1: TBitBtn
    Left = 80
    Top = 196
    Width = 92
    Height = 34
    Anchors = [akLeft, akBottom]
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 4
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 221
    Top = 196
    Width = 92
    Height = 34
    Anchors = [akLeft, akBottom]
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 5
  end
  object btnAddReason: TButton
    Left = 332
    Top = 153
    Width = 78
    Height = 28
    Anchors = [akTop, akRight]
    Caption = 'Add Reason'
    Enabled = False
    TabOrder = 6
    OnClick = btnAddReasonClick
  end
end
