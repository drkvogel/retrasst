object frmAnalyseSpecimens: TfrmAnalyseSpecimens
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Select Samples for Analysis'
  ClientHeight = 212
  ClientWidth = 325
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poMainFormCenter
  DesignSize = (
    325
    212)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 53
    Top = 23
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object cbProject: TComboBox
    Left = 113
    Top = 20
    Width = 143
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Sorted = True
    TabOrder = 0
    Text = '(select project)'
    OnChange = cbProjectChange
  end
  object rgSource: TRadioGroup
    Left = 31
    Top = 64
    Width = 106
    Height = 129
    Anchors = [akLeft, akTop, akBottom]
    Caption = 'Source'
    ItemIndex = 0
    Items.Strings = (
      'Stored boxes'
      'Retrieval list'
      'Text list (file)')
    TabOrder = 1
  end
  object BitBtn1: TBitBtn
    Left = 189
    Top = 91
    Width = 105
    Height = 36
    Anchors = [akRight, akBottom]
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
    TabOrder = 2
    OnClick = BitBtn1Click
  end
  object BitBtn2: TBitBtn
    Left = 189
    Top = 141
    Width = 105
    Height = 36
    Anchors = [akRight, akBottom]
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 3
  end
  object openDialogue: TOpenDialog
    DefaultExt = 'txt'
    Left = 149
    Top = 144
  end
end
