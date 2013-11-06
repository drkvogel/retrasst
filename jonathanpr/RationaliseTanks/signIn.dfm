object tSignIn: TtSignIn
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Sign in'
  ClientHeight = 69
  ClientWidth = 409
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object userNameTEdit: TEdit
    Left = 8
    Top = 8
    Width = 194
    Height = 21
    TabOrder = 0
    TextHint = 'User name'
  end
  object passwordTEdit: TEdit
    Left = 208
    Top = 8
    Width = 194
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
    TextHint = 'Password'
  end
  object CancelButton: TButton
    Left = 247
    Top = 35
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelButtonClick
  end
  object SignInButton: TButton
    Left = 87
    Top = 35
    Width = 75
    Height = 25
    Caption = 'Sign In'
    TabOrder = 3
    OnClick = SignInButtonClick
  end
end
