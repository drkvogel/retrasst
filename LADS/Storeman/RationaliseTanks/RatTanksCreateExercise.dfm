object frmRatTanksRetrivalExercise: TfrmRatTanksRetrivalExercise
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Create retrieval exercise'
  ClientHeight = 201
  ClientWidth = 403
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 8
    Width = 39
    Height = 18
    Caption = 'Name'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 8
    Top = 35
    Width = 70
    Height = 18
    Caption = 'Description'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object NameEdit: TEdit
    Left = 84
    Top = 5
    Width = 301
    Height = 21
    MaxLength = 16
    TabOrder = 0
  end
  object Description_Memo: TMemo
    Left = 84
    Top = 32
    Width = 301
    Height = 121
    MaxLength = 64
    TabOrder = 1
  end
  object SaveButton: TButton
    Left = 84
    Top = 159
    Width = 75
    Height = 25
    Caption = 'Save'
    TabOrder = 2
    OnClick = SaveButtonClick
  end
  object CancelButton: TButton
    Left = 310
    Top = 159
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 3
    OnClick = CancelButtonClick
  end
  object BalloonHint1: TBalloonHint
    Left = 24
    Top = 72
  end
end
