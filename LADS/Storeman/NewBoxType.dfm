object frmNewBoxType: TfrmNewBoxType
  Left = 0
  Top = 0
  Caption = 'Create New Box Type'
  ClientHeight = 362
  ClientWidth = 358
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 52
    Top = 32
    Width = 63
    Height = 13
    Caption = 'Simple name:'
  end
  object Label2: TLabel
    Left = 52
    Top = 72
    Width = 57
    Height = 13
    Caption = 'Description:'
  end
  object Label3: TLabel
    Left = 52
    Top = 112
    Width = 65
    Height = 13
    Caption = 'Example box:'
  end
  object Label4: TLabel
    Left = 52
    Top = 152
    Width = 60
    Height = 13
    Caption = 'First aliquot:'
  end
  object Label5: TLabel
    Left = 52
    Top = 192
    Width = 74
    Height = 13
    Caption = 'Second aliquot:'
  end
  object Label6: TLabel
    Left = 52
    Top = 232
    Width = 63
    Height = 13
    Caption = 'Third aliquot:'
  end
  object Label7: TLabel
    Left = 52
    Top = 272
    Width = 52
    Height = 13
    Caption = 'Formation:'
  end
  object txtName: TEdit
    Left = 156
    Top = 29
    Width = 150
    Height = 21
    MaxLength = 30
    TabOrder = 0
    OnExit = txtNameExit
  end
  object txtFull: TEdit
    Left = 156
    Top = 69
    Width = 150
    Height = 21
    MaxLength = 64
    TabOrder = 1
  end
  object txtExample: TEdit
    Left = 156
    Top = 109
    Width = 150
    Height = 21
    TabStop = False
    ReadOnly = True
    TabOrder = 2
  end
  object cbAT1: TComboBox
    Left = 156
    Top = 149
    Width = 150
    Height = 21
    TabOrder = 3
    OnDropDown = aliquotDropDown
  end
  object cbAT2: TComboBox
    Left = 156
    Top = 189
    Width = 150
    Height = 21
    TabOrder = 4
    OnDropDown = aliquotDropDown
  end
  object cbAT3: TComboBox
    Left = 156
    Top = 229
    Width = 150
    Height = 21
    TabOrder = 5
    OnDropDown = aliquotDropDown
  end
  object cbForm: TComboBox
    Left = 156
    Top = 269
    Width = 150
    Height = 21
    TabOrder = 6
    OnDropDown = cbFormDropDown
  end
  object BitBtn1: TBitBtn
    Left = 77
    Top = 312
    Width = 92
    Height = 34
    Caption = 'OK'
    Default = True
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
    TabOrder = 7
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 192
    Top = 312
    Width = 92
    Height = 34
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 8
  end
end
