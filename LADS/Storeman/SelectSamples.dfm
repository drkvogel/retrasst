object frmSelectBoxes: TfrmSelectBoxes
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Select Boxes for Analysis'
  ClientHeight = 385
  ClientWidth = 357
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poMainFormCenter
  DesignSize = (
    357
    385)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 55
    Top = 100
    Width = 43
    Height = 13
    Caption = 'Barcode:'
  end
  object Label3: TLabel
    Left = 55
    Top = 64
    Width = 49
    Height = 13
    Caption = 'Box Type:'
  end
  object Label2: TLabel
    Left = 55
    Top = 29
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object BitBtn1: TBitBtn
    Left = 245
    Top = 264
    Width = 92
    Height = 36
    Anchors = [akRight, akBottom]
    Caption = 'OK'
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
    ModalResult = 1
    NumGlyphs = 2
    TabOrder = 5
  end
  object BitBtn2: TBitBtn
    Left = 245
    Top = 320
    Width = 92
    Height = 36
    Anchors = [akRight, akBottom]
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 6
  end
  object cbType: TComboBox
    Left = 125
    Top = 61
    Width = 176
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    Sorted = True
    TabOrder = 1
  end
  object txtName: TEdit
    Left = 125
    Top = 97
    Width = 176
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 2
  end
  object btnAddBox: TBitBtn
    Left = 245
    Top = 146
    Width = 92
    Height = 36
    Anchors = [akTop, akRight]
    Caption = 'Add'
    Default = True
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
      33333333FF33333333FF333993333333300033377F3333333777333993333333
      300033F77FFF3333377739999993333333333777777F3333333F399999933333
      33003777777333333377333993333333330033377F3333333377333993333333
      3333333773333333333F333333333333330033333333F33333773333333C3333
      330033333337FF3333773333333CC333333333FFFFF77FFF3FF33CCCCCCCCCC3
      993337777777777F77F33CCCCCCCCCC3993337777777777377333333333CC333
      333333333337733333FF3333333C333330003333333733333777333333333333
      3000333333333333377733333333333333333333333333333333}
    NumGlyphs = 2
    TabOrder = 3
    OnClick = btnAddBoxClick
  end
  object lstBoxes: TListBox
    Left = 20
    Top = 146
    Width = 203
    Height = 222
    TabStop = False
    Anchors = [akLeft, akTop, akRight, akBottom]
    Enabled = False
    ExtendedSelect = False
    ItemHeight = 13
    Sorted = True
    TabOrder = 4
  end
  object txtProject: TEdit
    Left = 125
    Top = 26
    Width = 176
    Height = 21
    TabStop = False
    Anchors = [akLeft, akTop, akRight]
    ReadOnly = True
    TabOrder = 0
  end
end
