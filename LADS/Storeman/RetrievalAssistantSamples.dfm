object frmSamples: TfrmSamples
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Sample Retrieval Plan'
  ClientHeight = 626
  ClientWidth = 884
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 265
    Width = 884
    Height = 10
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitLeft = -295
    ExplicitWidth = 713
  end
  object splitterDebug: TSplitter
    Left = 0
    Top = 525
    Width = 884
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    Visible = False
    ExplicitLeft = 2
    ExplicitTop = 552
  end
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 884
    Height = 265
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 0
    object Panel2: TPanel
      Left = 792
      Top = 15
      Width = 90
      Height = 248
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object btnAutoChunk: TButton
        Left = 0
        Top = 0
        Width = 90
        Height = 25
        Align = alTop
        Caption = 'Auto-Chunk'
        TabOrder = 0
        OnClick = btnAutoChunkClick
      end
      object btnCancel: TButton
        Left = 0
        Top = 223
        Width = 90
        Height = 25
        Align = alBottom
        Caption = 'Exit'
        TabOrder = 1
        OnClick = btnCancelClick
      end
      object btnSave: TButton
        Left = 0
        Top = 198
        Width = 90
        Height = 25
        Align = alBottom
        Caption = 'Save'
        TabOrder = 2
        OnClick = btnSaveClick
      end
      object btnReject: TButton
        Left = 0
        Top = 173
        Width = 90
        Height = 25
        Align = alBottom
        Caption = 'Reject List'
        TabOrder = 3
        OnClick = btnRejectClick
      end
      object cbLog: TCheckBox
        Left = 0
        Top = 25
        Width = 90
        Height = 17
        Align = alTop
        Caption = 'Show Log'
        TabOrder = 4
        OnClick = cbLogClick
      end
    end
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 790
      Height = 248
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goColSizing, goEditing, goRowSelect, goThumbTracking, goFixedRowClick]
      TabOrder = 1
      OnClick = sgChunksClick
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
      OnGetEditText = sgChunksGetEditText
      OnSetEditText = sgChunksSetEditText
    end
  end
  object groupVials: TGroupBox
    Left = 0
    Top = 316
    Width = 884
    Height = 209
    Align = alClient
    Caption = 'Vials in chunk'
    TabOrder = 1
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 880
      Height = 192
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goFixedRowClick]
      TabOrder = 0
      OnClick = sgVialsClick
      OnDblClick = sgVialsDblClick
      OnDrawCell = sgVialsDrawCell
      OnFixedCellClick = sgVialsFixedCellClick
    end
    object panelLoading: TPanel
      Left = 176
      Top = 64
      Width = 521
      Height = 127
      Caption = 'Loading samples, please wait...'
      TabOrder = 1
      Visible = False
      object progressBottom: TProgressBar
        Left = 1
        Top = 109
        Width = 519
        Height = 17
        Align = alBottom
        TabOrder = 0
      end
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 275
    Width = 884
    Height = 41
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 2
    object groupSort: TGroupBox
      Left = 0
      Top = 0
      Width = 884
      Height = 41
      Align = alClient
      Caption = 'Sort'
      TabOrder = 0
      object btnDelSort: TButton
        Left = 800
        Top = 15
        Width = 25
        Height = 24
        Align = alRight
        Caption = '-'
        TabOrder = 0
        OnClick = btnDelSortClick
      end
      object btnAddSort: TButton
        Left = 776
        Top = 15
        Width = 24
        Height = 24
        Align = alRight
        Caption = '+'
        TabOrder = 1
        OnClick = btnAddSortClick
      end
      object btnApplySort: TButton
        Left = 825
        Top = 15
        Width = 57
        Height = 24
        Align = alRight
        Caption = 'Sort'
        TabOrder = 2
        OnClick = btnApplySortClick
      end
    end
  end
  object panelDebug: TPanel
    Left = 0
    Top = 528
    Width = 884
    Height = 98
    Align = alBottom
    TabOrder = 3
    Visible = False
    object Splitter2: TSplitter
      Left = 345
      Top = 1
      Height = 96
      ExplicitLeft = 549
    end
    object memoDebug: TMemo
      Left = 1
      Top = 1
      Width = 344
      Height = 96
      Align = alLeft
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object sgDebug: TStringGrid
      Left = 348
      Top = 1
      Width = 535
      Height = 96
      Align = alClient
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
      TabOrder = 1
    end
  end
  object timerLoadVials: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadVialsTimer
    Left = 664
    Top = 48
  end
end
