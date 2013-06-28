object AddBoxSize: TAddBoxSize
  Left = 318
  Top = 203
  Width = 386
  Height = 240
  Caption = 'Add Box Size'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 12
    Top = 20
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label2: TLabel
    Left = 12
    Top = 58
    Width = 44
    Height = 13
    Caption = 'Capacity:'
  end
  object Label6: TLabel
    Left = 208
    Top = 58
    Width = 71
    Height = 13
    Caption = 'Empty position:'
  end
  object Label5: TLabel
    Left = 48
    Top = 142
    Width = 12
    Height = 13
    Caption = 'to:'
  end
  object Label12: TLabel
    Left = 19
    Top = 104
    Width = 49
    Height = 13
    Caption = 'Valid from:'
  end
  object edtDescription: TEdit
    Left = 80
    Top = 16
    Width = 281
    Height = 21
    TabOrder = 0
  end
  object edtCapacity: TEdit
    Left = 80
    Top = 54
    Width = 69
    Height = 21
    TabOrder = 1
  end
  object Panel1: TPanel
    Left = 0
    Top = 172
    Width = 378
    Height = 41
    Align = alBottom
    TabOrder = 6
    object Save: TButton
      Left = 231
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 63
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object edtEmptyPos: TEdit
    Left = 293
    Top = 54
    Width = 68
    Height = 21
    TabOrder = 2
  end
  object dtpFrom: TDateTimePicker
    Left = 80
    Top = 100
    Width = 109
    Height = 21
    CalAlignment = dtaLeft
    Date = 38279.6779598843
    Time = 38279.6779598843
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 3
  end
  object dtpTo: TDateTimePicker
    Left = 80
    Top = 138
    Width = 109
    Height = 21
    CalAlignment = dtaLeft
    Date = 38279.6779879167
    Time = 38279.6779879167
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 4
  end
  object cbActive: TCheckBox
    Left = 241
    Top = 102
    Width = 120
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current definition:'
    TabOrder = 5
  end
  object cbVacutainer: TCheckBox
    Left = 241
    Top = 140
    Width = 120
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Store vacutainers:'
    TabOrder = 7
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 160
    Top = 56
  end
end
