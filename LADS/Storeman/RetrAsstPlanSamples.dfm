object frmSamples: TfrmSamples
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Sample Retrieval Plan'
  ClientHeight = 626
  ClientWidth = 963
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
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 258
    Width = 963
    Height = 15
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitTop = 193
    ExplicitWidth = 884
  end
  object splitterDebug: TSplitter
    Left = 0
    Top = 525
    Width = 963
    Height = 3
    Cursor = crVSplit
    Align = alBottom
    Visible = False
    ExplicitLeft = 2
    ExplicitTop = 552
    ExplicitWidth = 884
  end
  object panelDebug: TPanel
    Left = 0
    Top = 528
    Width = 963
    Height = 98
    Align = alBottom
    TabOrder = 0
    Visible = False
    object Splitter2: TSplitter
      Left = 713
      Top = 1
      Height = 96
      ExplicitLeft = 549
    end
    object memoDebug: TMemo
      Left = 1
      Top = 1
      Width = 712
      Height = 96
      Align = alLeft
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object sgDebug: TStringGrid
      Left = 716
      Top = 1
      Width = 246
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
    Width = 963
    Height = 258
    Align = alTop
    Caption = 'panelChunks'
    TabOrder = 1
    object groupList: TGroupBox
      Left = 1
      Top = 1
      Width = 961
      Height = 256
      Align = alClient
      Caption = 'Chunks'
      TabOrder = 0
      object Panel2: TPanel
        Left = 869
        Top = 15
        Width = 90
        Height = 198
        Align = alRight
        BevelOuter = bvNone
        TabOrder = 0
        object btnCancel: TButton
          Left = 0
          Top = 50
          Width = 90
          Height = 25
          Align = alTop
          Caption = 'Exit'
          TabOrder = 0
          OnClick = btnCancelClick
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
        end
      end
      object sgChunks: TStringGrid
        Left = 2
        Top = 15
        Width = 867
        Height = 198
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
      object Panel1: TPanel
        Left = 2
        Top = 213
        Width = 957
        Height = 41
        Align = alBottom
        BevelOuter = bvNone
        Caption = 'Panel1'
        TabOrder = 2
        object groupAutoChunk: TGroupBox
          Left = 201
          Top = 0
          Width = 756
          Height = 41
          Align = alClient
          Caption = 'Auto-Chunk'
          TabOrder = 0
          object Label1: TLabel
            AlignWithMargins = True
            Left = 156
            Top = 16
            Width = 91
            Height = 20
            Alignment = taCenter
            AutoSize = False
            Caption = 'Max chunk size'
            GlowSize = 8
            Transparent = True
          end
          object Label2: TLabel
            Left = 289
            Top = 16
            Width = 86
            Height = 26
            Alignment = taRightJustify
            AutoSize = False
            Caption = 'Next chunk size'
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
            Left = 242
            Top = 14
            Width = 33
            Height = 21
            Alignment = taCenter
            AutoSize = False
            TabOrder = 0
            Text = '500'
            OnChange = editDestBoxSizeChange
          end
          object comboSectionSize: TComboBox
            Left = 380
            Top = 14
            Width = 106
            Height = 21
            Style = csDropDownList
            TabOrder = 1
          end
          object btnAddChunk: TButton
            Left = 488
            Top = 11
            Width = 90
            Height = 26
            Caption = 'Add 1 Chunk'
            TabOrder = 2
            OnClick = btnAddChunkClick
          end
          object editDestBoxSize: TEdit
            AlignWithMargins = True
            Left = 108
            Top = 14
            Width = 30
            Height = 21
            Alignment = taCenter
            AutoSize = False
            TabOrder = 3
            Text = '72'
            OnChange = editDestBoxSizeChange
          end
          object btnAddAllChunks: TButton
            Left = 580
            Top = 11
            Width = 87
            Height = 26
            Caption = 'Add All Chunks'
            TabOrder = 4
            OnClick = btnAddAllChunksClick
          end
          object updownBoxSize: TUpDown
            AlignWithMargins = True
            Left = 138
            Top = 14
            Width = 16
            Height = 21
            Margins.Right = 13
            Associate = editDestBoxSize
            Position = 72
            TabOrder = 5
          end
          object updownMaxSection: TUpDown
            Left = 275
            Top = 14
            Width = 16
            Height = 21
            Associate = editMaxSize
            Max = 5000
            Position = 500
            TabOrder = 6
          end
          object btnDelChunk: TButton
            Left = 672
            Top = 15
            Width = 82
            Height = 24
            Align = alRight
            Caption = 'Delete last'
            TabOrder = 7
            OnClick = btnDelChunkClick
          end
        end
        object GroupBox1: TGroupBox
          Left = 0
          Top = 0
          Width = 97
          Height = 41
          Margins.Bottom = 9
          Align = alLeft
          Caption = 'Primary Aliquot'
          Padding.Left = 5
          Padding.Top = 5
          Padding.Right = 5
          Padding.Bottom = 5
          TabOrder = 1
          object labelPrimary: TLabel
            Left = 7
            Top = 20
            Width = 58
            Height = 13
            Align = alClient
            Caption = 'labelPrimary'
            Layout = tlCenter
          end
        end
        object GroupBox2: TGroupBox
          Left = 97
          Top = 0
          Width = 104
          Height = 41
          Align = alLeft
          Caption = 'Secondary Aliquot'
          Padding.Left = 5
          Padding.Top = 5
          Padding.Right = 5
          Padding.Bottom = 5
          TabOrder = 2
          object labelSecondary: TLabel
            Left = 7
            Top = 20
            Width = 73
            Height = 13
            Margins.Left = 5
            Margins.Top = 5
            Margins.Right = 5
            Margins.Bottom = 5
            Align = alClient
            Caption = 'labelSecondary'
            Layout = tlCenter
          end
        end
      end
    end
  end
  object panelVials: TPanel
    Left = 0
    Top = 273
    Width = 963
    Height = 252
    Align = alClient
    TabOrder = 2
    object groupVials: TGroupBox
      Left = 1
      Top = 42
      Width = 961
      Height = 209
      Align = alClient
      Caption = 'Vials in chunk'
      TabOrder = 0
      object sgVials: TStringGrid
        Left = 2
        Top = 15
        Width = 957
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
      Width = 961
      Height = 41
      Align = alTop
      Caption = 'Sort'
      TabOrder = 1
      object btnDelSort: TButton
        Left = 877
        Top = 15
        Width = 25
        Height = 24
        Align = alRight
        Caption = '-'
        TabOrder = 0
        OnClick = btnDelSortClick
      end
      object btnAddSort: TButton
        Left = 853
        Top = 15
        Width = 24
        Height = 24
        Align = alRight
        Caption = '+'
        TabOrder = 1
        OnClick = btnAddSortClick
      end
      object btnApplySort: TButton
        Left = 902
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