object PasswordDialog: TPasswordDialog
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Password required'
  ClientHeight = 89
  ClientWidth = 282
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
    Left = 8
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
    Left = 8
    Top = 30
    Width = 265
    Height = 21
    TabOrder = 0
    TextHint = 'Your password'
  end
  object SubmitButton: TButton
    Left = 8
    Top = 57
    Width = 89
    Height = 25
    Caption = 'Submit'
    TabOrder = 1
    OnClick = SubmitButtonClick
  end
  object CancelButton: TButton
    Left = 198
    Top = 57
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelButtonClick
  end
end
