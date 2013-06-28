object AddTestMachine: TAddTestMachine
  Left = 341
  Top = 121
  Width = 281
  Height = 235
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Central Test Machine'
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
    Top = 50
    Width = 108
    Height = 13
    Caption = 'Analyser/cluster name:'
  end
  object lblMachineName: TLabel
    Left = 150
    Top = 50
    Width = 3
    Height = 13
  end
  object Label4: TLabel
    Left = 12
    Top = 89
    Width = 124
    Height = 13
    Caption = 'Analyser code for this test:'
  end
  object Label8: TLabel
    Left = 68
    Top = 12
    Width = 41
    Height = 20
    Caption = 'Test:'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblTest: TLabel
    Left = 130
    Top = 12
    Width = 6
    Height = 20
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 12
    Top = 128
    Width = 42
    Height = 13
    Caption = 'Protocol:'
  end
  object Label9: TLabel
    Left = 158
    Top = 128
    Width = 51
    Height = 13
    Caption = 'Test order:'
  end
  object edtMachineTestName: TEdit
    Left = 158
    Top = 85
    Width = 97
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 166
    Width = 273
    Height = 42
    Align = alBottom
    TabOrder = 3
    object Save: TButton
      Left = 167
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 31
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object edtProtocol: TEdit
    Left = 64
    Top = 124
    Width = 32
    Height = 21
    TabOrder = 1
  end
  object edtTestOrder: TEdit
    Left = 223
    Top = 124
    Width = 32
    Height = 21
    TabOrder = 2
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 224
    Top = 24
  end
end
