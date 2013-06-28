object frmBoxList: TfrmBoxList
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Boxes for Transfer'
  ClientHeight = 404
  ClientWidth = 672
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnHide = FormHide
  OnShow = FormShow
  DesignSize = (
    672
    404)
  PixelsPerInch = 96
  TextHeight = 13
  object Label2: TLabel
    Left = 550
    Top = 285
    Width = 91
    Height = 13
    Anchors = [akRight, akBottom]
    Caption = 'Box name/number:'
  end
  object Label1: TLabel
    Left = 8
    Top = 11
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object Label3: TLabel
    Left = 550
    Top = 15
    Width = 106
    Height = 39
    Caption = 
      'Click headings to sort, box names to select what to print/export' +
      '.'
    WordWrap = True
  end
  object cbProject: TComboBox
    Left = 52
    Top = 9
    Width = 156
    Height = 21
    Sorted = True
    TabOrder = 0
    Text = '(select project)'
    OnChange = cbProjectChange
  end
  object btnPrint: TBitBtn
    Left = 550
    Top = 135
    Width = 106
    Height = 36
    Anchors = [akTop, akRight]
    Caption = 'Print'
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00300000000000
      00033FFFFFFFFFFFFFFF0888888888888880777777777777777F088888888888
      8880777777777777777F0000000000000000FFFFFFFFFFFFFFFF0F8F8F8F8F8F
      8F80777777777777777F08F8F8F8F8F8F9F0777777777777777F0F8F8F8F8F8F
      8F807777777777777F7F0000000000000000777777777777777F3330FFFFFFFF
      03333337F3FFFF3F7F333330F0000F0F03333337F77773737F333330FFFFFFFF
      03333337F3FF3FFF7F333330F00F000003333337F773777773333330FFFF0FF0
      33333337F3FF7F3733333330F08F0F0333333337F7737F7333333330FFFF0033
      33333337FFFF7733333333300000033333333337777773333333}
    NumGlyphs = 2
    TabOrder = 2
    OnClick = btnPrintClick
  end
  object btnExport: TBitBtn
    Left = 550
    Top = 85
    Width = 106
    Height = 36
    Anchors = [akTop, akRight]
    Caption = 'Export'
    Glyph.Data = {
      76010000424D7601000000000000760000002800000020000000100000000100
      04000000000000010000130B0000130B00001000000000000000000000000000
      800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
      FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333303
      333333333333337FF3333333333333903333333333333377FF33333333333399
      03333FFFFFFFFF777FF3000000999999903377777777777777FF0FFFF0999999
      99037F3337777777777F0FFFF099999999907F3FF777777777770F00F0999999
      99037F773777777777730FFFF099999990337F3FF777777777330F00FFFFF099
      03337F773333377773330FFFFFFFF09033337F3FF3FFF77733330F00F0000003
      33337F773777777333330FFFF0FF033333337F3FF7F3733333330F08F0F03333
      33337F7737F7333333330FFFF003333333337FFFF77333333333000000333333
      3333777777333333333333333333333333333333333333333333}
    NumGlyphs = 2
    TabOrder = 1
    OnClick = btnExportClick
  end
  object ebBoxNum: TEdit
    Left = 550
    Top = 305
    Width = 106
    Height = 21
    Anchors = [akRight, akBottom]
    TabOrder = 3
  end
  object btnAddBox: TBitBtn
    Left = 550
    Top = 338
    Width = 106
    Height = 36
    Anchors = [akRight, akBottom]
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
    TabOrder = 4
    OnClick = btnAddBoxClick
  end
  object sgBoxNames: TStringGrid
    Left = 224
    Top = 9
    Width = 312
    Height = 385
    Hint = 'Click header to sort, row to include or exclude'
    Margins.Left = 0
    Margins.Top = 0
    Margins.Right = 0
    Margins.Bottom = 0
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 2
    DefaultColWidth = 72
    DefaultDrawing = False
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 5
    OnDrawCell = sgBoxNamesDrawCell
    OnFixedCellClick = sgBoxNamesFixedCellClick
  end
  object sgBoxTypes: TStringGrid
    Left = 8
    Top = 41
    Width = 200
    Height = 353
    Hint = 'Click to sort or select'
    Anchors = [akLeft, akTop, akBottom]
    ColCount = 4
    DefaultColWidth = 19
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goFixedRowClick]
    ScrollBars = ssVertical
    TabOrder = 6
    OnDrawCell = sgBoxTypesDrawCell
    OnFixedCellClick = sgBoxTypesFixedCellClick
    OnSelectCell = sgBoxTypesSelectCell
  end
  object saveDialogue: TSaveDialog
    DefaultExt = 'txt'
    Filter = 'Text files|*.txt'
    Left = 360
    Top = 192
  end
  object printDialogue: TPrintDialog
    Left = 456
    Top = 192
  end
  object timer: TTimer
    Enabled = False
    Interval = 600
    OnTimer = timerTimer
    Left = 288
    Top = 192
  end
end
