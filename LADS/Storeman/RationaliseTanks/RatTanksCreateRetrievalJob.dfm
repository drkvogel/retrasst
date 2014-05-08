object frmRatTankCreateRetrievalJob: TfrmRatTankCreateRetrievalJob
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Create Retrieval Job'
  ClientHeight = 301
  ClientWidth = 461
  Color = 12316364
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
    Top = 100
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
  object Label2: TLabel
    Left = 8
    Top = 48
    Width = 113
    Height = 18
    Caption = 'Retrivel exercise*'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 132
    Top = 75
    Width = 296
    Height = 26
    Caption = 
      '* attach this task to a retrival exercise, this is optional, lea' +
      've blank for none. Click button for create a new one.'
    WordWrap = True
  end
  object Label4: TLabel
    Left = 8
    Top = 199
    Width = 114
    Height = 18
    Caption = 'Reason for action'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object external_nameEdit: TEdit
    Left = 140
    Top = 8
    Width = 309
    Height = 21
    Hint = 'tfyurt7'
    MaxLength = 30
    ParentShowHint = False
    ShowHint = False
    TabOrder = 0
  end
  object descriptionMemo: TMemo
    Left = 8
    Top = 124
    Width = 441
    Height = 53
    MaxLength = 120
    TabOrder = 2
  end
  object ButtonCancel: TButton
    Left = 259
    Top = 240
    Width = 75
    Height = 28
    Caption = 'Cancel'
    TabOrder = 4
    OnClick = ButtonCancelClick
  end
  object ButtonOK: TButton
    Left = 127
    Top = 240
    Width = 75
    Height = 28
    Caption = 'Create'
    TabOrder = 3
    OnClick = ButtonOKClick
  end
  object ProgressBar: TProgressBar
    Left = 0
    Top = 283
    Width = 461
    Height = 18
    Align = alBottom
    TabOrder = 5
    ExplicitLeft = 8
    ExplicitTop = 301
    ExplicitWidth = 441
  end
  object RetrivelExerciseComboBox: TComboBox
    Left = 140
    Top = 48
    Width = 261
    Height = 21
    Sorted = True
    TabOrder = 1
  end
  object New_Retrival_exercise_Button: TButton
    Left = 407
    Top = 46
    Width = 42
    Height = 25
    Caption = '...'
    TabOrder = 6
    OnClick = New_Retrival_exercise_ButtonClick
  end
  object reasonEdit: TEdit
    Left = 140
    Top = 200
    Width = 309
    Height = 21
    Hint = 'tfyurt7'
    MaxLength = 30
    ParentShowHint = False
    ShowHint = False
    TabOrder = 7
  end
  object BalloonHint1: TBalloonHint
    Delay = 50
    HideAfter = 5000
    Left = 424
    Top = 88
  end
end
