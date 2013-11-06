object CreateRetrievalJobForm: TCreateRetrievalJobForm
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Create Retrieval Job'
  ClientHeight = 225
  ClientWidth = 461
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object label: TLabel
    Left = 8
    Top = 8
    Width = 102
    Height = 18
    Caption = 'New task name'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 8
    Top = 44
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
  object external_nameEdit: TEdit
    Left = 116
    Top = 8
    Width = 333
    Height = 21
    Hint = 'tfyurt7'
    MaxLength = 30
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
  end
  object descriptionMemo: TMemo
    Left = 8
    Top = 68
    Width = 441
    Height = 124
    MaxLength = 120
    TabOrder = 1
  end
  object ButtonCancel: TButton
    Left = 272
    Top = 198
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = ButtonCancelClick
  end
  object ButtonOK: TButton
    Left = 116
    Top = 198
    Width = 75
    Height = 25
    Caption = 'Create'
    TabOrder = 3
    OnClick = ButtonOKClick
  end
  object BalloonHint1: TBalloonHint
    Delay = 50
    HideAfter = 5000
    Left = 128
    Top = 40
  end
end
