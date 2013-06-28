object AboutBox: TAboutBox
  Left = 192
  Top = 111
  BorderIcons = [biMinimize, biMaximize]
  BorderStyle = bsDialog
  Caption = 'About Lab Wizard'
  ClientHeight = 224
  ClientWidth = 328
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
    Left = 98
    Top = 32
    Width = 118
    Height = 13
    Caption = 'CTSU Laboratory Wizard'
  end
  object Label2: TLabel
    Left = 72
    Top = 80
    Width = 38
    Height = 13
    Caption = 'Version:'
  end
  object Label3: TLabel
    Left = 72
    Top = 128
    Width = 122
    Height = 13
    Caption = 'Created: DATE and TIME'
  end
  object Bevel1: TBevel
    Left = 63
    Top = 16
    Width = 201
    Height = 137
    Shape = bsFrame
  end
  object lblVersion: TLabel
    Left = 120
    Top = 80
    Width = 3
    Height = 13
  end
  object btnOK: TButton
    Left = 126
    Top = 176
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = btnOKClick
  end
end
