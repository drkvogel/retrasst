object frmRetrievalListDialog: TfrmRetrievalListDialog
  Left = 0
  Top = 0
  Caption = 'Cryovial search results'
  ClientHeight = 395
  ClientWidth = 830
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnClose = FormClose
  OnResize = FormResize
  PixelsPerInch = 96
  TextHeight = 13
  object mainStringGrid: TStringGrid
    Left = 0
    Top = 0
    Width = 830
    Height = 395
    Align = alClient
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
    TabOrder = 0
    OnDrawCell = mainStringGridDrawCell
    OnMouseDown = mainStringGridMouseDown
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 408
    Top = 200
  end
  object PopupMenu1: TPopupMenu
    TrackButton = tbLeftButton
    OnPopup = PopupMenu1Popup
    Left = 520
    Top = 192
    object NotesSubMenu: TMenuItem
      Caption = 'Display Notes From'
      object NotesFromResultsMenu: TMenuItem
        Caption = 'Results'
        OnClick = NotesFromResultsMenuClick
      end
      object NotesFromCryovialMenu: TMenuItem
        Caption = 'Cryovial'
        OnClick = NotesFromCryovialMenuClick
      end
      object NotesFromCryoStoreMenu: TMenuItem
        Caption = 'Cryovial Store'
        OnClick = NotesFromCryoStoreMenuClick
      end
      object NotesFromSpecimenMenu: TMenuItem
        Caption = 'Specimen'
        OnClick = NotesFromSpecimenMenuClick
      end
      object NotesFromBoxNameMenu: TMenuItem
        Caption = 'Box'
        OnClick = NotesFromBoxNameMenuClick
      end
    end
    object Anal: TMenuItem
      Caption = 'Search for Analyses'
      object MenuFindAnyAnalyses: TMenuItem
        Caption = 'Find any Analyses (barcode,aliquot)'
        OnClick = MenuFindAnyAnalysesClick
      end
      object FindanyAnalysescryovialbarcode: TMenuItem
        Caption = 'Find any Analyses (sample id)'
        OnClick = FindanyAnalysescryovialbarcodeClick
      end
      object MenuFindAnyAnalysesFromSameSource: TMenuItem
        Caption = 'Find any Analyses from same Source'
        OnClick = MenuFindAnyAnalysesFromSameSourceClick
      end
    end
    object Searchforcryovialhistory1: TMenuItem
      Caption = 'Display extended information on based on'
      object MenuCryovialHistory: TMenuItem
        Caption = 'Cryovial'
        OnClick = MenuCryovialHistoryClick
      end
    end
    object N4: TMenuItem
      Caption = '-'
    end
    object Rebuildlistusing: TMenuItem
      Caption = 'Rebuild list using'
      object MenuSearchWithSampleID: TMenuItem
        Caption = 'Find others with Sample id (Source name)'
        OnClick = MenuSearchWithSampleIDClick
      end
      object MenuSearchWithBarcodeAndAliquot: TMenuItem
        Caption = 'Find others with Cryovial Barcode and Aliquot type'
        OnClick = MenuSearchWithBarcodeAndAliquotClick
      end
      object SearchWithCryovialSampleid: TMenuItem
        Caption = 'Find others with cryovial.Sample_id = specimen.sample_id'
        OnClick = SearchWithCryovialSampleidClick
      end
      object MenuSearchWithBarcode: TMenuItem
        Caption = 'Find others with Cryovial Barcode'
        OnClick = MenuSearchWithBarcodeClick
      end
      object MenuFindSiblings: TMenuItem
        Caption = 'Find Siblings  (cryovial_cid)'
        Visible = False
        OnClick = MenuFindSiblingsClick
      end
    end
  end
  object AnimTimer: TTimer
    Enabled = False
    Interval = 100
    OnTimer = AnimTimerTimer
    Left = 544
    Top = 304
  end
end
