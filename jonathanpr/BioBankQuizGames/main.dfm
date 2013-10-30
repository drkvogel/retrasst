object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Form1'
  ClientHeight = 169
  ClientWidth = 568
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 8
    Top = 71
    Width = 545
    Height = 89
    Caption = 'The new games, will serialize the Rosetta on completion'
    TabOrder = 0
    object Button3: TButton
      Left = 456
      Top = 32
      Width = 75
      Height = 25
      Caption = 'Symbol Digits'
      TabOrder = 0
      OnClick = Button3Click
    end
    object Button2: TButton
      Left = 16
      Top = 32
      Width = 75
      Height = 25
      Caption = 'Trails'
      TabOrder = 1
      OnClick = Button2Click
    end
  end
  object Panel2: TPanel
    Left = 8
    Top = 8
    Width = 545
    Height = 57
    Caption = 'Original Pairs game'
    TabOrder = 1
    object Button1: TButton
      Left = 16
      Top = 17
      Width = 75
      Height = 25
      Caption = 'Pairs'
      TabOrder = 0
      OnClick = Button1Click
    end
  end
end
