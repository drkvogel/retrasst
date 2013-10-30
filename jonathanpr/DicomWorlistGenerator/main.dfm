object DicomWorkListGenerator: TDicomWorkListGenerator
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'DicomWorkListGenerator'
  ClientHeight = 144
  ClientWidth = 167
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 8
    Top = 24
    Width = 137
    Height = 25
    Caption = 'Generate worklists'
    TabOrder = 0
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 8
    Top = 96
    Width = 137
    Height = 25
    Caption = 'Test'
    TabOrder = 1
    OnClick = Button2Click
  end
end
