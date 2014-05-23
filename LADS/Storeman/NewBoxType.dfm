object frmNewBoxType: TfrmNewBoxType
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Select Box Type'
  ClientHeight = 396
  ClientWidth = 365
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 35
    Top = 100
    Width = 63
    Height = 13
    Caption = 'Simple name:'
  end
  object Label2: TLabel
    Left = 35
    Top = 139
    Width = 57
    Height = 13
    Caption = 'Description:'
  end
  object Label3: TLabel
    Left = 35
    Top = 177
    Width = 65
    Height = 13
    Caption = 'Example box:'
  end
  object Label4: TLabel
    Left = 35
    Top = 216
    Width = 60
    Height = 13
    Caption = 'First aliquot:'
  end
  object Label5: TLabel
    Left = 35
    Top = 254
    Width = 74
    Height = 13
    Caption = 'Second aliquot:'
  end
  object Label6: TLabel
    Left = 35
    Top = 293
    Width = 63
    Height = 13
    Caption = 'Third aliquot:'
  end
  object Label7: TLabel
    Left = 35
    Top = 24
    Width = 52
    Height = 13
    Caption = 'Formation:'
  end
  object Label8: TLabel
    Left = 35
    Top = 62
    Width = 77
    Height = 13
    Caption = 'Matching types:'
  end
  object txtName: TEdit
    Left = 130
    Top = 97
    Width = 200
    Height = 21
    MaxLength = 30
    TabOrder = 2
    OnExit = txtNameExit
  end
  object txtFull: TEdit
    Left = 130
    Top = 136
    Width = 200
    Height = 21
    MaxLength = 64
    TabOrder = 3
    OnExit = txtNameExit
  end
  object txtExample: TEdit
    Left = 130
    Top = 174
    Width = 200
    Height = 21
    TabStop = False
    ReadOnly = True
    TabOrder = 4
  end
  object cbAT1: TComboBox
    Left = 130
    Top = 213
    Width = 200
    Height = 21
    Sorted = True
    TabOrder = 5
    OnChange = cbATChange
    OnDropDown = aliquotDropDown
  end
  object cbAT2: TComboBox
    Left = 130
    Top = 251
    Width = 200
    Height = 21
    Sorted = True
    TabOrder = 6
    OnChange = cbATChange
    OnDropDown = aliquotDropDown
  end
  object cbAT3: TComboBox
    Left = 130
    Top = 290
    Width = 200
    Height = 21
    Sorted = True
    TabOrder = 7
    OnChange = cbATChange
    OnDropDown = aliquotDropDown
  end
  object cbForms: TComboBox
    Left = 130
    Top = 21
    Width = 200
    Height = 21
    TabOrder = 0
    OnChange = cbFormsChange
    OnDropDown = cbFormsDropDown
  end
  object cbTypes: TComboBox
    Left = 130
    Top = 59
    Width = 200
    Height = 21
    TabOrder = 1
    OnChange = cbTypesChange
    OnDropDown = cbTypesDropDown
  end
  object btnOK: TBitBtn
    Left = 72
    Top = 338
    Width = 97
    Height = 36
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 8
  end
  object btnClear: TBitBtn
    Left = 195
    Top = 338
    Width = 92
    Height = 36
    Caption = 'Clear'
    Glyph.Data = {
      DE010000424DDE01000000000000760000002800000024000000120000000100
      0400000000006801000000000000000000001000000000000000000000000000
      80000080000000808000800000008000800080800000C0C0C000808080000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333444444
      33333333333F8888883F33330000324334222222443333388F3833333388F333
      000032244222222222433338F8833FFFFF338F3300003222222AAAAA22243338
      F333F88888F338F30000322222A33333A2224338F33F8333338F338F00003222
      223333333A224338F33833333338F38F00003222222333333A444338FFFF8F33
      3338888300003AAAAAAA33333333333888888833333333330000333333333333
      333333333333333333FFFFFF000033333333333344444433FFFF333333888888
      00003A444333333A22222438888F333338F3333800003A2243333333A2222438
      F38F333333833338000033A224333334422224338338FFFFF8833338000033A2
      22444442222224338F3388888333FF380000333A2222222222AA243338FF3333
      33FF88F800003333AA222222AA33A3333388FFFFFF8833830000333333AAAAAA
      3333333333338888883333330000333333333333333333333333333333333333
      0000}
    NumGlyphs = 2
    TabOrder = 9
    OnClick = btnClearClick
  end
end
