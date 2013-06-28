object frmPassword: TfrmPassword
  Left = 404
  Top = 531
  BorderIcons = []
  Caption = 'Your password has expired'
  ClientHeight = 283
  ClientWidth = 299
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Label3: TLabel
    Left = 26
    Top = 143
    Width = 73
    Height = 13
    Caption = 'New password:'
  end
  object Label1: TLabel
    Left = 26
    Top = 180
    Width = 61
    Height = 13
    Caption = 'Confirm new:'
  end
  object version: TLabel
    Left = 26
    Top = 20
    Width = 194
    Height = 16
    Caption = 'Please change your password'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clNavy
    Font.Height = -13
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label2: TLabel
    Left = 26
    Top = 70
    Width = 54
    Height = 13
    Caption = 'User name:'
  end
  object Label4: TLabel
    Left = 26
    Top = 106
    Width = 67
    Height = 13
    Caption = 'Old password:'
  end
  object ebNewPass: TEdit
    Left = 136
    Top = 140
    Width = 136
    Height = 21
    PasswordChar = '*'
    TabOrder = 2
    OnChange = ebNewPassChange
  end
  object okButton: TBitBtn
    Left = 36
    Top = 228
    Width = 100
    Height = 36
    Caption = '&OK (save)'
    Enabled = False
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 4
    OnClick = okButtonClick
  end
  object exitButton: TBitBtn
    Left = 164
    Top = 228
    Width = 100
    Height = 36
    Caption = '&Cancel'
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 5
  end
  object ebConfirmed: TEdit
    Left = 136
    Top = 177
    Width = 136
    Height = 21
    PasswordChar = '*'
    TabOrder = 3
    OnChange = ebNewPassChange
  end
  object ebUserName: TEdit
    Left = 136
    Top = 67
    Width = 136
    Height = 21
    ReadOnly = True
    TabOrder = 0
  end
  object ebOldPass: TEdit
    Left = 136
    Top = 103
    Width = 136
    Height = 21
    PasswordChar = '*'
    TabOrder = 1
    OnChange = ebNewPassChange
  end
end
