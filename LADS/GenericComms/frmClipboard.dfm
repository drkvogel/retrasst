object formClipboard: TformClipboard
  Left = 0
  Top = 0
  Caption = 'Worklist Selection'
  ClientHeight = 366
  ClientWidth = 798
  Color = 16770290
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 337
    Width = 798
    Height = 29
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    object btnCopy: TButton
      Left = 0
      Top = 0
      Width = 137
      Height = 29
      Align = alLeft
      Caption = 'Copy to clipboard'
      TabOrder = 0
      OnClick = btnCopyClick
    end
    object btnClose: TButton
      Left = 680
      Top = 0
      Width = 118
      Height = 29
      Align = alRight
      Caption = 'Close'
      TabOrder = 1
      OnClick = btnCloseClick
    end
    object btnSave: TButton
      Left = 137
      Top = 0
      Width = 144
      Height = 29
      Align = alLeft
      Caption = 'Save to file'
      TabOrder = 2
    end
  end
  object memoClipboard: TMemo
    Left = 0
    Top = 49
    Width = 798
    Height = 288
    Align = alClient
    Lines.Strings = (
      '')
    TabOrder = 1
  end
  object Panel2: TPanel
    Left = 0
    Top = 0
    Width = 798
    Height = 49
    Align = alTop
    TabOrder = 2
    object GroupBox1: TGroupBox
      Left = 1
      Top = 1
      Width = 796
      Height = 47
      Align = alClient
      Caption = 'Input format'
      TabOrder = 0
      object comboFormat: TComboBox
        Left = 2
        Top = 15
        Width = 717
        Height = 21
        Align = alClient
        Style = csDropDownList
        ItemHeight = 0
        TabOrder = 0
        OnChange = comboFormatChange
      end
      object btnGenerate: TButton
        Left = 719
        Top = 15
        Width = 75
        Height = 30
        Align = alRight
        Caption = 'Generate'
        TabOrder = 1
        OnClick = btnGenerateClick
        ExplicitLeft = 664
        ExplicitTop = 24
        ExplicitHeight = 25
      end
    end
  end
end
