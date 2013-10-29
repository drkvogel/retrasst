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
    Top = 258
    Width = 884
    Height = 15
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitTop = 193
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
  object panelDebug: TPanel
    Left = 0
    Top = 528
    Width = 884
    Height = 98
    Align = alBottom
    TabOrder = 0
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
  object panelChunks: TPanel
    Left = 0
    Top = 0
    Width = 884
    Height = 258
    Align = alTop
    Caption = 'panelChunks'
    TabOrder = 1
    object groupList: TGroupBox
      Left = 1
      Top = 1
      Width = 882
      Height = 213
      Align = alClient
      Caption = 'Chunks'
      TabOrder = 0
      ExplicitLeft = 0
      ExplicitTop = 0
      ExplicitWidth = 884
      ExplicitHeight = 153
      object Panel2: TPanel
        Left = 790
        Top = 15
        Width = 90
        Height = 196
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        ExplicitHeight = 136
        object btnCancel: TButton
          Left = 0
          Top = 50
          Width = 90
          Height = 25
          Align = alTop
          Caption = 'Exit'
          TabOrder = 0
          OnClick = btnCancelClick
          ExplicitTop = 111
        end
        object btnSave: TButton
          Left = 0
          Top = 25
          Width = 90
          Height = 25
          Align = alTop
          Caption = 'Save'
          TabOrder = 1
          OnClick = btnSaveClick
          ExplicitLeft = 6
          ExplicitTop = 0
        end
        object btnReject: TButton
          Left = 0
          Top = 0
          Width = 90
          Height = 25
          Align = alTop
          Caption = 'Reject List'
          TabOrder = 2
          OnClick = btnRejectClick
          ExplicitTop = -15
        end
        object cbLog: TCheckBox
          Left = 0
          Top = 75
          Width = 90
          Height = 17
          Align = alTop
          Caption = 'Show Log'
          TabOrder = 3
          OnClick = cbLogClick
          ExplicitLeft = 24
          ExplicitTop = 120
        end
      end
      object sgChunks: TStringGrid
        Left = 2
        Top = 15
        Width = 788
        Height = 196
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
        ExplicitHeight = 136
      end
    end
    object groupAutoChunk: TGroupBox
      Left = 1
      Top = 214
      Width = 882
      Height = 43
      Align = alBottom
      Caption = 'Auto-Chunk'
      TabOrder = 1
      object Label1: TLabel
        AlignWithMargins = True
        Left = 165
        Top = 16
        Width = 102
        Height = 20
        Alignment = taCenter
        AutoSize = False
        Caption = 'Max section size'
        Transparent = True
      end
      object Label2: TLabel
        Left = 327
        Top = 17
        Width = 86
        Height = 26
        Alignment = taRightJustify
        AutoSize = False
        Caption = 'Next section size'
      end
      object Label3: TLabel
        Left = 2
        Top = 16
        Width = 111
        Height = 23
        Alignment = taCenter
        AutoSize = False
        Caption = 'Destination box size'
      end
      object editMaxSize: TEdit
        Left = 261
        Top = 14
        Width = 45
        Height = 21
        Alignment = taCenter
        AutoSize = False
        TabOrder = 0
        Text = '500'
        OnChange = editDestBoxSizeChange
      end
      object comboSectionSize: TComboBox
        Left = 424
        Top = 15
        Width = 106
        Height = 21
        Style = csDropDownList
        TabOrder = 1
      end
      object btnAddChunk: TButton
        Left = 535
        Top = 14
        Width = 90
        Height = 26
        Caption = 'Add 1 Chunk'
        TabOrder = 2
        OnClick = btnAddChunkClick
      end
      object editDestBoxSize: TEdit
        AlignWithMargins = True
        Left = 110
        Top = 14
        Width = 39
        Height = 21
        Alignment = taCenter
        AutoSize = False
        TabOrder = 3
        Text = '72'
        OnChange = editDestBoxSizeChange
      end
      object btnAddAllChunks: TButton
        Left = 628
        Top = 14
        Width = 87
        Height = 26
        Caption = 'Add All Chunks'
        TabOrder = 4
        OnClick = btnAddAllChunksClick
      end
      object updownBoxSize: TUpDown
        AlignWithMargins = True
        Left = 148
        Top = 14
        Width = 17
        Height = 21
        Margins.Right = 13
        Associate = editDestBoxSize
        Position = 72
        TabOrder = 5
      end
      object updownMaxSection: TUpDown
        Left = 305
        Top = 14
        Width = 16
        Height = 22
        Associate = editMaxSize
        Max = 5000
        Position = 500
        TabOrder = 6
      end
      object btnDelChunk: TButton
        Left = 798
        Top = 15
        Width = 82
        Height = 26
        Align = alRight
        Caption = 'Delete last'
        TabOrder = 7
        OnClick = btnDelChunkClick
        ExplicitLeft = 797
        ExplicitTop = 14
      end
    end
  end
  object panelVials: TPanel
    Left = 0
    Top = 273
    Width = 884
    Height = 252
    Align = alClient
    TabOrder = 2
    ExplicitTop = 336
    ExplicitHeight = 189
    object groupVials: TGroupBox
      Left = 1
      Top = 42
      Width = 882
      Height = 209
      Align = alClient
      Caption = 'Vials in chunk'
      TabOrder = 0
      ExplicitLeft = 0
      ExplicitTop = 314
      ExplicitWidth = 884
      ExplicitHeight = 170
      object sgVials: TStringGrid
        Left = 2
        Top = 15
        Width = 878
        Height = 192
        Align = alClient
        Anchors = [akLeft, akRight, akBottom]
        DefaultDrawing = False
        FixedCols = 0
        Options = [goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goFixedRowClick]
        TabOrder = 0
        OnClick = sgVialsClick
        OnDblClick = sgVialsDblClick
        OnDrawCell = sgVialsDrawCell
        OnFixedCellClick = sgVialsFixedCellClick
        ExplicitHeight = 82
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
    object groupSort: TGroupBox
      Left = 1
      Top = 1
      Width = 882
      Height = 41
      Align = alTop
      Caption = 'Sort'
      TabOrder = 1
      ExplicitLeft = 0
      ExplicitTop = 214
      ExplicitWidth = 884
      object btnDelSort: TButton
        Left = 798
        Top = 15
        Width = 25
        Height = 24
        Align = alRight
        Caption = '-'
        TabOrder = 0
        OnClick = btnDelSortClick
      end
      object btnAddSort: TButton
        Left = 774
        Top = 15
        Width = 24
        Height = 24
        Align = alRight
        Caption = '+'
        TabOrder = 1
        OnClick = btnAddSortClick
      end
      object btnApplySort: TButton
        Left = 823
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
  object timerLoadVials: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadVialsTimer
    Left = 664
    Top = 48
  end
  object timerCalculate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = timerCalculateTimer
    Left = 560
    Top = 48
  end
end
