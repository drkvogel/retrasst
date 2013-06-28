object AddCentralTest: TAddCentralTest
  Left = 313
  Top = 114
  Width = 478
  Height = 463
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Central Test'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 20
    Width = 66
    Height = 13
    Caption = 'Name of test: '
  end
  object Label2: TLabel
    Left = 16
    Top = 56
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label5: TLabel
    Left = 277
    Top = 172
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object Label12: TLabel
    Left = 16
    Top = 172
    Width = 49
    Height = 13
    Caption = 'Valid from:'
  end
  object Label3: TLabel
    Left = 16
    Top = 212
    Width = 143
    Height = 13
    Caption = 'Default limits (analytic range) --'
  end
  object Label13: TLabel
    Left = 46
    Top = 244
    Width = 32
    Height = 13
    Caption = 'Lower:'
  end
  object Label14: TLabel
    Left = 261
    Top = 244
    Width = 32
    Height = 13
    Caption = 'Upper:'
  end
  object Label15: TLabel
    Left = 232
    Top = 136
    Width = 61
    Height = 13
    Caption = 'Sample type:'
  end
  object TestName: TEdit
    Left = 90
    Top = 16
    Width = 201
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 57
    Top = 277
    Width = 356
    Height = 41
    TabOrder = 8
    object Save: TButton
      Left = 233
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 49
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object DefaultLowerLimit: TEdit
    Left = 90
    Top = 240
    Width = 87
    Height = 21
    TabOrder = 6
  end
  object DefaultUpperLimit: TEdit
    Left = 306
    Top = 240
    Width = 87
    Height = 21
    TabOrder = 7
  end
  object Description: TMemo
    Left = 90
    Top = 56
    Width = 336
    Height = 57
    Lines.Strings = (
      '')
    TabOrder = 1
  end
  object SampleType: TEdit
    Left = 306
    Top = 132
    Width = 120
    Height = 21
    TabOrder = 3
  end
  object dtpFrom: TDateTimePicker
    Left = 90
    Top = 169
    Width = 120
    Height = 21
    CalAlignment = dtaLeft
    Date = 38279.6779598843
    Time = 38279.6779598843
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 4
  end
  object dtpTo: TDateTimePicker
    Left = 306
    Top = 169
    Width = 120
    Height = 21
    CalAlignment = dtaLeft
    Date = 38279.6779879167
    Time = 38279.6779879167
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 5
  end
  object cbActive: TCheckBox
    Left = 16
    Top = 133
    Width = 87
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 2
  end
  object Panel3: TPanel
    Left = 0
    Top = 395
    Width = 470
    Height = 41
    Align = alBottom
    TabOrder = 9
    object btnClose: TButton
      Left = 32
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object btnAddMachines: TButton
    Left = 60
    Top = 340
    Width = 150
    Height = 32
    Caption = 'Add/Delete Machines'
    TabOrder = 10
    OnClick = btnAddMachinesClick
  end
  object btnCalculation: TButton
    Left = 260
    Top = 340
    Width = 150
    Height = 32
    Caption = 'Add/Delete Calculation'
    TabOrder = 11
    OnClick = btnCalculationClick
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 24
    Top = 88
  end
end
