object Form2: TForm2
  Left = 486
  Top = 372
  Caption = 'Form2'
  ClientHeight = 290
  ClientWidth = 240
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 252
    Width = 34
    Height = 13
    Caption = 'Result:'
  end
  object Label2: TLabel
    Left = 24
    Top = 37
    Width = 162
    Height = 13
    Caption = 'Definition for '#39'validateTestResult'#39':'
  end
  object Label3: TLabel
    Left = 24
    Top = 18
    Width = 98
    Height = 13
    Caption = 'test result (integer):'
  end
  object Button1: TButton
    Left = 64
    Top = 207
    Width = 75
    Height = 25
    Caption = 'validate'
    TabOrder = 0
    OnClick = Button1Click
  end
  object script: TMemo
    Left = 24
    Top = 56
    Width = 185
    Height = 122
    Lines.Strings = (
      'function validateTestResult ( r )'
      '  return r < '
      '    ( '
      '        getTestResult(2) +'
      '        getTestResult(4)'
      '    ) '
      'end'
      '')
    TabOrder = 1
  end
  object result: TEdit
    Left = 64
    Top = 249
    Width = 130
    Height = 21
    TabOrder = 2
  end
  object testResult: TEdit
    Left = 131
    Top = 12
    Width = 78
    Height = 21
    TabOrder = 3
  end
  object StaticText1: TStaticText
    Left = 24
    Top = 184
    Width = 170
    Height = 17
    Caption = '(getTestResult returns testID + 1)'
    TabOrder = 4
  end
end
