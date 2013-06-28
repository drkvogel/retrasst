object AddMachine: TAddMachine
  Left = 262
  Top = 189
  Width = 430
  Height = 372
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Machine'
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
    Top = 22
    Width = 76
    Height = 13
    Caption = 'Machine name: '
  end
  object Label2: TLabel
    Left = 16
    Top = 56
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object Label4: TLabel
    Left = 16
    Top = 196
    Width = 67
    Height = 13
    Caption = 'Valid dates --  '
  end
  object Label5: TLabel
    Left = 62
    Top = 252
    Width = 16
    Height = 13
    Caption = 'To:'
  end
  object Label11: TLabel
    Left = 16
    Top = 122
    Width = 44
    Height = 13
    Caption = 'Location:'
  end
  object Label12: TLabel
    Left = 62
    Top = 220
    Width = 26
    Height = 13
    Caption = 'From:'
  end
  object Label3: TLabel
    Left = 16
    Top = 159
    Width = 64
    Height = 13
    Caption = 'Cluster name:'
  end
  object MachineName: TEdit
    Left = 100
    Top = 18
    Width = 180
    Height = 21
    TabOrder = 1
  end
  object Location: TEdit
    Left = 100
    Top = 118
    Width = 301
    Height = 21
    TabOrder = 3
  end
  object Panel1: TPanel
    Left = 0
    Top = 298
    Width = 422
    Height = 47
    Align = alBottom
    TabOrder = 7
    object Save: TButton
      Left = 264
      Top = 10
      Width = 78
      Height = 28
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 80
      Top = 10
      Width = 78
      Height = 28
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object Description: TMemo
    Left = 100
    Top = 54
    Width = 301
    Height = 49
    Lines.Strings = (
      '')
    TabOrder = 2
  end
  object cbxCluster: TComboBox
    Left = 100
    Top = 155
    Width = 180
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object dtpValidFrom: TDateTimePicker
    Left = 100
    Top = 216
    Width = 130
    Height = 21
    CalAlignment = dtaLeft
    Date = 38247.5777502894
    Time = 38247.5777502894
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 4
  end
  object dtpValidTo: TDateTimePicker
    Left = 100
    Top = 249
    Width = 130
    Height = 21
    CalAlignment = dtaLeft
    Date = 38247.5777502894
    Time = 38247.5777502894
    DateFormat = dfShort
    DateMode = dmComboBox
    Kind = dtkDate
    ParseInput = False
    TabOrder = 5
  end
  object rgStatus: TRadioGroup
    Left = 320
    Top = 200
    Width = 81
    Height = 73
    Caption = 'Status'
    ItemIndex = 0
    Items.Strings = (
      'current'
      'obsolete')
    TabOrder = 6
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 256
    Top = 224
  end
end
