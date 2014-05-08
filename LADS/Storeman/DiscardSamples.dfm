object frmDiscardSamples: TfrmDiscardSamples
  Left = 0
  Top = 0
  Caption = 'Discard Samples'
  ClientHeight = 761
  ClientWidth = 969
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnClose = FormClose
  DesignSize = (
    969
    761)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlResults: TPanel
    Left = 0
    Top = 47
    Width = 972
    Height = 681
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 0
    VerticalAlignment = taAlignTop
    DesignSize = (
      972
      681)
    object grdResults: TStringGrid
      Left = 2
      Top = 0
      Width = 970
      Height = 681
      Anchors = [akLeft, akTop, akRight, akBottom]
      ParentShowHint = False
      ShowHint = False
      TabOrder = 0
      OnContextPopup = grdResultsContextPopup
      OnDrawCell = grdResultsDrawCell
      OnKeyPress = grdResultsKeyPress
      OnMouseMove = grdResultsMouseMove
      OnMouseUp = grdResultsMouseUp
      ColWidths = (
        64
        64
        64
        64
        64)
    end
  end
  object pnlSearch: TPanel
    Left = 0
    Top = 0
    Width = 968
    Height = 41
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    DesignSize = (
      968
      41)
    object labMode: TLabel
      Left = 456
      Top = 14
      Width = 26
      Height = 13
      Caption = 'Mode'
    end
    object lblCurrentNote: TLabel
      Left = 528
      Top = 14
      Width = 3
      Height = 13
      Anchors = [akLeft, akTop, akRight]
    end
    object btnConfirm: TButton
      Left = 176
      Top = 7
      Width = 56
      Height = 26
      Caption = 'Confirm'
      TabOrder = 0
      OnClick = btnConfirmClick
    end
    object btnClear: TButton
      Left = 96
      Top = 7
      Width = 56
      Height = 26
      Caption = 'Clear'
      TabOrder = 1
      OnClick = btnClearClick
    end
    object btnSearch: TButton
      Left = 16
      Top = 7
      Width = 56
      Height = 26
      Caption = 'Search'
      TabOrder = 2
      OnClick = btnSearchClick
    end
    object btnNote: TButton
      Left = 860
      Top = 8
      Width = 84
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Set Note Text'
      TabOrder = 3
      OnClick = btnNoteClick
    end
    object bitDiscard: TBitBtn
      Left = 488
      Top = 7
      Width = 25
      Height = 25
      Hint = 'Click on grid marks for discard'
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00000000000000
        0000000011111110000000119999999110000199999999999100019999999999
        91001999FF999FF999101999FFF9FFF9991019999FFFFF999910199999FFF999
        991019999FFFFF9999101999FFF9FFF999101999FF999FF99910019999999999
        9100019999999999910000119999999110000000111111100000}
      ParentShowHint = False
      ShowHint = True
      TabOrder = 4
      OnClick = bitDiscardClick
    end
    object bitNote: TBitBtn
      Left = 488
      Top = 8
      Width = 25
      Height = 25
      Hint = 'Click on grid applies note'
      Glyph.Data = {
        F6000000424DF600000000000000760000002800000010000000100000000100
        0400000000008000000000000000000000001000000000000000000000000000
        8000008000000080800080000000800080008080000080808000C0C0C0000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00777777770000
        000000000000770000000FFFFFFF007000000FFFFFFFFF0770000FF777777770
        07770FFFFFFFFFFFF0070FFFFFFFFFFFFF070FF77777777777070FFFFFFFFFFF
        FF070FFFFFFFFFFFFF070FF77777777777070F9F9999FF9F99070F9999F9FF9F
        9F070F9F9F9F999999070FFFFFFFFFFFFF070000000000000007}
      ParentShowHint = False
      ShowHint = True
      TabOrder = 5
      OnClick = bitNoteClick
    end
    object btnAbort: TButton
      Left = 256
      Top = 7
      Width = 56
      Height = 26
      Caption = 'Abort'
      TabOrder = 6
      OnClick = btnAbortClick
    end
  end
  object pnlStatus: TPanel
    Left = 0
    Top = 734
    Width = 968
    Height = 25
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 2
    DesignSize = (
      968
      25)
    object labStatus: TLabel
      Left = 0
      Top = 7
      Width = 3
      Height = 13
      Anchors = [akLeft, akTop, akRight, akBottom]
    end
  end
  object popCol: TPopupMenu
    AutoPopup = False
    Left = 328
    Top = 192
    object miSortAsc: TMenuItem
      Caption = 'sort ascending'
      OnClick = miSortAscClick
    end
    object miSortDesc: TMenuItem
      Caption = 'sort descending'
      OnClick = miSortDescClick
    end
    object miDiscardCol: TMenuItem
      Caption = 'mark column for discard'
      OnClick = miDiscardColClick
    end
    object miNoteCol: TMenuItem
      Caption = 'apply note to column'
      OnClick = miNoteColClick
    end
  end
  object popCell: TPopupMenu
    AutoPopup = False
    Left = 368
    Top = 192
    object miDiscardCell: TMenuItem
      Caption = 'mark for discard'
      OnClick = miDiscardCellClick
    end
    object miNoteCell: TMenuItem
      Caption = 'apply note'
      OnClick = miNoteCellClick
    end
    object miViewNote: TMenuItem
      Caption = 'view note'
      OnClick = miViewNoteClick
    end
    object miViewDnote: TMenuItem
      Caption = 'view draft note'
      OnClick = miViewDnoteClick
    end
  end
  object popRow: TPopupMenu
    AutoPopup = False
    Left = 408
    Top = 192
    object miDiscardRow: TMenuItem
      Caption = 'mark row for discard'
      OnClick = miDiscardRowClick
    end
    object miNoteRow: TMenuItem
      Caption = 'apply note to row'
      OnClick = miNoteRowClick
    end
  end
  object popTab: TPopupMenu
    Left = 448
    Top = 192
    object miSortAsc2: TMenuItem
      Caption = 'sort ascending'
      OnClick = miSortAsc2Click
    end
    object miSortDesc2: TMenuItem
      Caption = 'sort descending'
      OnClick = miSortDesc2Click
    end
    object miDiscardAll: TMenuItem
      Caption = 'mark all for discard'
      OnClick = miDiscardAllClick
    end
    object miNoteAll: TMenuItem
      Caption = 'apply note to all'
      OnClick = miNoteAllClick
    end
  end
end
