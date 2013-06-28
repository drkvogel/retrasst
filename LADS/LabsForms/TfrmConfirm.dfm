object frmConfirm: TfrmConfirm
  Left = 363
  Top = 131
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Update Confirmation'
  ClientHeight = 255
  ClientWidth = 625
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 339
    Top = 156
    Width = 49
    Height = 13
    Caption = 'Password:'
  end
  object Label2: TLabel
    Left = 64
    Top = 157
    Width = 54
    Height = 13
    Caption = 'User name:'
  end
  object Label4: TLabel
    Left = 18
    Top = 116
    Width = 463
    Height = 16
    Caption = 
      'Please select your user name and enter your password to continue' +
      ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label5: TLabel
    Left = 18
    Top = 57
    Width = 585
    Height = 32
    Caption = 
      'You are about to sign off a process.  By entering your user name' +
      ' and password you acknowledege that you have reviewed the data a' +
      'nd consent to your electronic signature being applied to that da' +
      'ta.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object lblSummary: TLabel
    Left = 18
    Top = 23
    Width = 73
    Height = 16
    Caption = '[summary]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object edtPassword: TEdit
    Left = 406
    Top = 153
    Width = 154
    Height = 21
    PasswordChar = '*'
    TabOrder = 0
    OnChange = userDetailChange
  end
  object cbUserNames: TComboBox
    Left = 134
    Top = 153
    Width = 156
    Height = 21
    DropDownCount = 12
    TabOrder = 1
    OnChange = userDetailChange
  end
  object BitBtn1: TBitBtn
    Left = 186
    Top = 200
    Width = 102
    Height = 37
    Caption = '&OK (save)'
    Default = True
    Enabled = False
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      3333333333333333333333330000333333333333333333333333F33333333333
      00003333344333333333333333388F3333333333000033334224333333333333
      338338F3333333330000333422224333333333333833338F3333333300003342
      222224333333333383333338F3333333000034222A22224333333338F338F333
      8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
      33333338F83338F338F33333000033A33333A222433333338333338F338F3333
      0000333333333A222433333333333338F338F33300003333333333A222433333
      333333338F338F33000033333333333A222433333333333338F338F300003333
      33333333A222433333333333338F338F00003333333333333A22433333333333
      3338F38F000033333333333333A223333333333333338F830000333333333333
      333A333333333333333338330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
    TabOrder = 2
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 337
    Top = 200
    Width = 102
    Height = 37
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 3
  end
end
