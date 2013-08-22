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
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 884
    Height = 265
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 0
    object Panel2: TPanel
      Left = 800
      Top = 15
      Width = 82
      Height = 248
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object btnAddChunk: TButton
        Left = 0
        Top = 25
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Add chunk'
        TabOrder = 0
        OnClick = btnAddChunkClick
      end
      object btnDelChunk: TButton
        Left = 0
        Top = 50
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Del chunk'
        Enabled = False
        TabOrder = 1
        OnClick = btnDelChunkClick
      end
      object btnIncr: TButton
        Left = 0
        Top = 75
        Width = 82
        Height = 25
        Align = alTop
        Caption = '+'
        TabOrder = 2
        OnClick = btnIncrClick
      end
      object btnDecr: TButton
        Left = 0
        Top = 100
        Width = 82
        Height = 25
        Align = alTop
        Caption = '-'
        TabOrder = 3
        OnClick = btnDecrClick
      end
      object btnAutoChunk: TButton
        Left = 0
        Top = 0
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Auto-Chunk'
        TabOrder = 4
        OnClick = btnAutoChunkClick
      end
      object btnCancel: TButton
        Left = 0
        Top = 223
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Exit'
        TabOrder = 5
        OnClick = btnCancelClick
      end
      object btnSave: TButton
        Left = 0
        Top = 198
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Save'
        TabOrder = 6
        OnClick = btnSaveClick
      end
      object btnReject: TButton
        Left = 0
        Top = 173
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Reject List'
        TabOrder = 7
        OnClick = btnRejectClick
      end
      object cbLog: TCheckBox
        Left = 0
        Top = 125
        Width = 82
        Height = 17
        Align = alTop
        Caption = 'Show Log'
        TabOrder = 8
        OnClick = cbLogClick
      end
    end
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 798
      Height = 248
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
      TabOrder = 1
      OnDrawCell = sgChunksDrawCell
    end
  end
  object groupVials: TGroupBox
    Left = 0
    Top = 316
    Width = 884
    Height = 310
    Align = alClient
    Caption = 'Vials in chunk'
    TabOrder = 1
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 880
      Height = 204
      Align = alClient
      FixedCols = 0
      Options = [goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goFixedRowClick]
      TabOrder = 0
      OnClick = sgVialsClick
      OnFixedCellClick = sgVialsFixedCellClick
    end
    object memoDebug: TMemo
      Left = 2
      Top = 219
      Width = 880
      Height = 89
      Align = alBottom
      ScrollBars = ssVertical
      TabOrder = 1
      Visible = False
    end
    object panelLoading: TPanel
      Left = 280
      Top = 112
      Width = 273
      Height = 127
      Caption = 'Loading samples, please wait...'
      TabOrder = 2
      Visible = False
      object progressBottom: TProgressBar
        Left = 1
        Top = 109
        Width = 271
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
      Width = 699
      Height = 41
      Align = alClient
      Caption = 'Sort'
      TabOrder = 0
      object btnDelSort: TButton
        Left = 615
        Top = 15
        Width = 25
        Height = 24
        Align = alRight
        Caption = '-'
        TabOrder = 0
        OnClick = btnDelSortClick
      end
      object btnAddSort: TButton
        Left = 591
        Top = 15
        Width = 24
        Height = 24
        Align = alRight
        Caption = '+'
        TabOrder = 1
        OnClick = btnAddSortClick
      end
      object btnApplySort: TButton
        Left = 640
        Top = 15
        Width = 57
        Height = 24
        Align = alRight
        Caption = 'Apply'
        TabOrder = 2
      end
    end
    object GroupBox2: TGroupBox
      Left = 699
      Top = 0
      Width = 185
      Height = 41
      Align = alRight
      Caption = 'Show rows'
      TabOrder = 1
      object radbutDefault: TRadioButton
        Left = 2
        Top = 15
        Width = 33
        Height = 24
        Align = alLeft
        Caption = '25'
        Checked = True
        TabOrder = 0
        TabStop = True
        OnClick = radbutDefaultClick
      end
      object radbutAll: TRadioButton
        Left = 35
        Top = 15
        Width = 30
        Height = 24
        Align = alLeft
        Caption = 'All'
        TabOrder = 1
        OnClick = radbutAllClick
      end
      object radbutCustom: TRadioButton
        Left = 65
        Top = 15
        Width = 57
        Height = 24
        Align = alLeft
        Caption = 'Custom'
        TabOrder = 2
        OnClick = radbutCustomClick
      end
      object editCustomRows: TEdit
        Left = 122
        Top = 15
        Width = 61
        Height = 24
        Align = alClient
        Enabled = False
        TabOrder = 3
        Text = '100'
        OnChange = editCustomRowsChange
        ExplicitHeight = 21
      end
    end
  end
  object timerCustomRows: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerCustomRowsTimer
    Left = 752
    Top = 48
  end
  object timerLoadVials: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadVialsTimer
    Left = 664
    Top = 48
  end
end
