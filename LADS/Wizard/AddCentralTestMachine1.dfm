object AddCentralTestMachine: TAddCentralTestMachine
  Left = 344
  Top = 113
  Width = 269
  Height = 292
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
    Left = 32
    Top = 40
    Width = 73
    Height = 13
    Caption = 'Machine name:'
  end
  object lblMachineName: TLabel
    Left = 136
    Top = 40
    Width = 3
    Height = 13
  end
  object Label4: TLabel
    Left = 32
    Top = 72
    Width = 93
    Height = 13
    Caption = 'Machine test name:'
  end
  object Label5: TLabel
    Left = 20
    Top = 72
    Width = 4
    Height = 13
    Caption = '*'
  end
  object Label6: TLabel
    Left = 24
    Top = 192
    Width = 4
    Height = 13
    Caption = '*'
  end
  object Label7: TLabel
    Left = 32
    Top = 192
    Width = 156
    Height = 13
    Caption = 'This machine'#39's name for this test.'
  end
  object Label8: TLabel
    Left = 68
    Top = 8
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
    Left = 129
    Top = 8
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
    Left = 32
    Top = 112
    Width = 42
    Height = 13
    Caption = 'Protocol:'
  end
  object Label9: TLabel
    Left = 32
    Top = 144
    Width = 51
    Height = 13
    Caption = 'Test order:'
  end
  object edtMachineTestName: TEdit
    Left = 136
    Top = 64
    Width = 97
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 224
    Width = 261
    Height = 41
    Align = alBottom
    TabOrder = 3
    object Save: TButton
      Left = 152
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 24
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object edtProtocol: TEdit
    Left = 136
    Top = 104
    Width = 25
    Height = 21
    TabOrder = 1
  end
  object edtTestOrder: TEdit
    Left = 136
    Top = 144
    Width = 25
    Height = 21
    TabOrder = 2
  end
end
