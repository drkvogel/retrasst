object AddTestToProfile: TAddTestToProfile
  Left = 391
  Top = 119
  Width = 326
  Height = 550
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Test To Profile'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 16
    Top = 48
    Width = 53
    Height = 13
    Caption = 'Test name:'
  end
  object Label2: TLabel
    Left = 38
    Top = 134
    Width = 32
    Height = 13
    Caption = 'Upper:'
  end
  object Label3: TLabel
    Left = 38
    Top = 169
    Width = 32
    Height = 13
    Caption = 'Lower:'
  end
  object labelTestName: TLabel
    Left = 95
    Top = 48
    Width = 3
    Height = 13
  end
  object Label4: TLabel
    Left = 16
    Top = 16
    Width = 61
    Height = 13
    Caption = 'Profile name:'
  end
  object labelProfileName: TLabel
    Left = 95
    Top = 16
    Width = 3
    Height = 13
  end
  object Label5: TLabel
    Left = 38
    Top = 413
    Width = 32
    Height = 13
    Caption = 'Upper:'
  end
  object Label6: TLabel
    Left = 38
    Top = 448
    Width = 32
    Height = 13
    Caption = 'Lower:'
  end
  object Label7: TLabel
    Left = 16
    Top = 389
    Width = 77
    Height = 13
    Caption = 'Trigger limits:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label8: TLabel
    Left = 16
    Top = 111
    Width = 77
    Height = 13
    Caption = 'Project limits:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label9: TLabel
    Left = 16
    Top = 333
    Width = 84
    Height = 13
    Caption = 'Trigger profile:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label10: TLabel
    Left = 16
    Top = 80
    Width = 55
    Height = 13
    Caption = 'Machine(s):'
  end
  object lblHasCalc: TLabel
    Left = 200
    Top = 48
    Width = 3
    Height = 13
  end
  object Label11: TLabel
    Left = 38
    Top = 224
    Width = 32
    Height = 13
    Caption = 'Upper:'
  end
  object Label12: TLabel
    Left = 38
    Top = 258
    Width = 32
    Height = 13
    Caption = 'Lower:'
  end
  object Label13: TLabel
    Left = 16
    Top = 200
    Width = 89
    Height = 13
    Caption = 'Warning levels:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ckbIgnoreWarning: TCheckBox
    Left = 232
    Top = 222
    Width = 73
    Height = 17
    Caption = 'No warnings'
    TabOrder = 6
  end
  object editUpperLimit: TEdit
    Left = 93
    Top = 131
    Width = 121
    Height = 21
    TabOrder = 1
  end
  object editLowerLimit: TEdit
    Left = 93
    Top = 165
    Width = 121
    Height = 21
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 0
    Top = 482
    Width = 318
    Height = 41
    Align = alBottom
    TabOrder = 12
    object Cancel: TButton
      Left = 52
      Top = 8
      Width = 65
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = CancelClick
    end
    object btnSaveClose: TButton
      Left = 164
      Top = 8
      Width = 97
      Height = 25
      Caption = 'Save and Close'
      TabOrder = 1
      OnClick = btnSaveCloseClick
    end
  end
  object editTriggerUpperLimit: TEdit
    Left = 93
    Top = 409
    Width = 121
    Height = 21
    TabOrder = 10
  end
  object editTriggerLowerLimit: TEdit
    Left = 93
    Top = 444
    Width = 121
    Height = 21
    TabOrder = 11
  end
  object cbxTriggerProfile: TComboBox
    Left = 16
    Top = 352
    Width = 289
    Height = 21
    ItemHeight = 13
    TabOrder = 9
  end
  object cbxMachines: TComboBox
    Left = 93
    Top = 76
    Width = 153
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object ckbPrivate: TCheckBox
    Left = 56
    Top = 296
    Width = 220
    Height = 17
    Caption = 'Private (do not release results to project)'
    TabOrder = 8
  end
  object ckbIgnoreLowerLimit: TCheckBox
    Left = 232
    Top = 167
    Width = 73
    Height = 17
    Caption = 'Use default'
    TabOrder = 4
  end
  object edtUpperWarning: TEdit
    Left = 93
    Top = 220
    Width = 121
    Height = 21
    TabOrder = 5
  end
  object ckbIgnoreUpperLimit: TCheckBox
    Left = 232
    Top = 132
    Width = 73
    Height = 17
    Caption = 'Use default'
    TabOrder = 2
  end
  object edtLowerWarning: TEdit
    Left = 93
    Top = 254
    Width = 121
    Height = 21
    TabOrder = 7
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 264
    Top = 24
  end
end
