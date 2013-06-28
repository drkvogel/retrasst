object AddOperator: TAddOperator
  Left = 313
  Top = 113
  Width = 443
  Height = 365
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Operator'
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
    Left = 16
    Top = 20
    Width = 47
    Height = 13
    Caption = 'Operator: '
  end
  object Label2: TLabel
    Left = 16
    Top = 57
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label4: TLabel
    Left = 16
    Top = 125
    Width = 67
    Height = 13
    Caption = 'Valid dates --  '
  end
  object Label12: TLabel
    Left = 50
    Top = 143
    Width = 26
    Height = 13
    Caption = 'From:'
  end
  object Label5: TLabel
    Left = 250
    Top = 143
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object Label11: TLabel
    Left = 16
    Top = 232
    Width = 49
    Height = 13
    Caption = 'Password:'
  end
  object Label6: TLabel
    Left = 250
    Top = 177
    Width = 38
    Height = 13
    Caption = 'Abilities:'
  end
  object Panel1: TPanel
    Left = 0
    Top = 297
    Width = 435
    Height = 41
    Align = alBottom
    TabOrder = 3
    object Save: TButton
      Left = 268
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 92
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object OperatorName: TEdit
    Left = 84
    Top = 16
    Width = 157
    Height = 21
    TabOrder = 0
  end
  object Description: TMemo
    Left = 84
    Top = 55
    Width = 329
    Height = 57
    Lines.Strings = (
      '')
    TabOrder = 1
  end
  object Password: TEdit
    Left = 84
    Top = 228
    Width = 136
    Height = 21
    TabOrder = 2
  end
  object cbVal1: TCheckBox
    Left = 276
    Top = 196
    Width = 97
    Height = 17
    Caption = 'Validation level 1'
    TabOrder = 4
  end
  object cbVal2: TCheckBox
    Left = 276
    Top = 219
    Width = 113
    Height = 17
    Caption = 'Validation level 2'
    TabOrder = 5
  end
  object cbPlotqc: TCheckBox
    Left = 276
    Top = 242
    Width = 153
    Height = 17
    Caption = 'PlotQC (disable data points)'
    TabOrder = 6
  end
  object cbWizard: TCheckBox
    Left = 276
    Top = 265
    Width = 97
    Height = 17
    Caption = 'Wizard'
    TabOrder = 7
  end
  object cbxStatus: TCheckBox
    Left = 16
    Top = 176
    Width = 81
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 8
  end
  object dtpFrom: TDateTimePicker
    Left = 84
    Top = 139
    Width = 137
    Height = 21
    CalAlignment = dtaLeft
    Date = 0.610780706017977
    Time = 0.610780706017977
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 9
  end
  object dtpTo: TDateTimePicker
    Left = 276
    Top = 139
    Width = 137
    Height = 21
    CalAlignment = dtaLeft
    Date = 0.610780706017977
    Time = 0.610780706017977
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 10
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 160
    Top = 184
  end
end
