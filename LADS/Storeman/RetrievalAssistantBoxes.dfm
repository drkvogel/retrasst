object frmBoxes: TfrmBoxes
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Box Retrieval Plan'
  ClientHeight = 588
  ClientWidth = 853
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 284
    Width = 853
    Height = 10
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitTop = 265
    ExplicitWidth = 841
  end
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 853
    Height = 284
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 0
    object Panel2: TPanel
      Left = 769
      Top = 15
      Width = 82
      Height = 267
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitLeft = 775
      ExplicitTop = 21
      ExplicitHeight = 248
      object btnAddChunk: TButton
        Left = 0
        Top = 25
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Add chunk'
        TabOrder = 0
        OnClick = btnAddChunkClick
        ExplicitTop = 0
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
        ExplicitTop = 25
      end
      object btnSaveChunk: TButton
        Left = 0
        Top = 75
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Save chunk'
        Enabled = False
        TabOrder = 2
        ExplicitTop = 50
      end
      object btnIncr: TButton
        Left = 0
        Top = 100
        Width = 82
        Height = 25
        Align = alTop
        Caption = '+'
        TabOrder = 3
        OnClick = btnIncrClick
        ExplicitTop = 75
      end
      object btnDecr: TButton
        Left = 0
        Top = 125
        Width = 82
        Height = 25
        Align = alTop
        Caption = '-'
        TabOrder = 4
        OnClick = btnDecrClick
        ExplicitTop = 100
      end
      object btnReject: TButton
        Left = 0
        Top = 192
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Reject List'
        TabOrder = 5
        OnClick = btnRejectClick
        ExplicitLeft = 6
        ExplicitTop = 142
      end
      object Button1: TButton
        Left = 0
        Top = 217
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Save'
        TabOrder = 6
        OnClick = btnSaveClick
        ExplicitTop = 198
      end
      object Button2: TButton
        Left = 0
        Top = 242
        Width = 82
        Height = 25
        Align = alBottom
        Caption = 'Exit'
        TabOrder = 7
        OnClick = btnCancelClick
        ExplicitTop = 223
      end
      object cbLog: TCheckBox
        Left = 0
        Top = 175
        Width = 82
        Height = 17
        Align = alBottom
        Caption = 'Show Log'
        TabOrder = 8
        OnClick = cbLogClick
        ExplicitLeft = 18
        ExplicitTop = 6
        ExplicitWidth = 64
      end
      object btnAutoChunk: TButton
        Left = 0
        Top = 0
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Auto-Chunk'
        TabOrder = 9
        OnClick = btnAutoChunkClick
      end
    end
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 767
      Height = 267
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
      TabOrder = 1
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
      OnSetEditText = sgChunksSetEditText
      ExplicitLeft = -4
      ExplicitTop = 21
      ExplicitHeight = 248
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 544
    Width = 853
    Height = 44
    Align = alBottom
    BevelEdges = []
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      853
      44)
    object btnSave: TButton
      Left = 692
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Save'
      TabOrder = 0
      OnClick = btnSaveClick
    end
    object btnCancel: TButton
      Left = 773
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Exit'
      TabOrder = 1
      OnClick = btnCancelClick
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 294
    Width = 853
    Height = 250
    Align = alClient
    Caption = 'Boxes in chunk'
    TabOrder = 2
    ExplicitTop = 275
    ExplicitHeight = 269
    object sgBoxes: TStringGrid
      Left = 2
      Top = 15
      Width = 849
      Height = 144
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goFixedRowClick]
      TabOrder = 0
      OnClick = sgBoxesClick
      OnDrawCell = sgBoxesDrawCell
      OnFixedCellClick = sgBoxesFixedCellClick
      ExplicitHeight = 163
    end
    object memoDebug: TMemo
      Left = 2
      Top = 159
      Width = 849
      Height = 89
      Align = alBottom
      TabOrder = 1
      Visible = False
      ExplicitTop = 178
    end
    object panelLoading: TPanel
      Left = 232
      Top = 80
      Width = 273
      Height = 127
      Caption = 'Loading boxes, please wait...'
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
  object timerLoadBoxes: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadBoxesTimer
    Left = 464
    Top = 40
  end
end
