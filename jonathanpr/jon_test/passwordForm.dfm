object Password: TPassword
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Password Required'
  ClientHeight = 99
  ClientWidth = 337
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 5
    Width = 265
    Height = 19
    Caption = 'Please enter database password'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object passwordEdit: TEdit
    Left = 32
    Top = 30
    Width = 265
    Height = 21
    TabOrder = 0
    TextHint = 'Your password'
  end
  object Button1: TButton
    Left = 32
    Top = 57
    Width = 89
    Height = 25
    Caption = 'Submit'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 222
    Top = 57
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Button2Click
  end
end
