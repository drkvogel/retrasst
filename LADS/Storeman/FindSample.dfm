object frmFind: TfrmFind
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  BorderStyle = bsDialog
  Caption = 'Find Box/Cryovial'
  ClientHeight = 211
  ClientWidth = 423
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnShow = FormShow
  DesignSize = (
    423
    211)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 170
    Top = 24
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object Label2: TLabel
    Left = 170
    Top = 101
    Width = 43
    Height = 13
    Caption = 'Barcode:'
  end
  object Label3: TLabel
    Left = 170
    Top = 62
    Width = 28
    Height = 13
    Caption = 'Type:'
  end
  object rgTypes: TRadioGroup
    Left = 32
    Top = 24
    Width = 81
    Height = 81
    Caption = 'Search for'
    ItemIndex = 0
    Items.Strings = (
      'Box'
      'Cryovial')
    TabOrder = 0
    OnClick = rgTypesClick
  end
  object cbType: TComboBox
    Left = 230
    Top = 59
    Width = 160
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Sorted = True
    TabOrder = 1
    OnChange = detailChange
    OnDropDown = cbTypeDropDown
  end
  object txtName: TEdit
    Left = 230
    Top = 98
    Width = 160
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
    OnChange = detailChange
  end
  object btnOK: TBitBtn
    Left = 108
    Top = 159
    Width = 92
    Height = 34
    Anchors = []
    Caption = 'OK'
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
    TabOrder = 3
    OnClick = btnOKClick
  end
  object btnCancel: TBitBtn
    Left = 237
    Top = 159
    Width = 92
    Height = 34
    Anchors = []
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 4
  end
  object cbProject: TComboBox
    Left = 230
    Top = 21
    Width = 160
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Sorted = True
    TabOrder = 5
    OnChange = cbProjectChange
  end
end
