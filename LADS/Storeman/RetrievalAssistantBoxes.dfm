object frmBoxes: TfrmBoxes
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Box Retrieval Plan'
  ClientHeight = 588
  ClientWidth = 713
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 265
    Width = 713
    Height = 10
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitWidth = 841
  end
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 713
    Height = 265
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 0
    object Panel2: TPanel
      Left = 629
      Top = 15
      Width = 82
      Height = 248
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object btnAddChunk: TButton
        Left = 0
        Top = 0
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Add chunk'
        TabOrder = 0
        OnClick = btnAddChunkClick
      end
      object btnDelChunk: TButton
        Left = 0
        Top = 25
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Del chunk'
        Enabled = False
        TabOrder = 1
        OnClick = btnDelChunkClick
      end
      object btnSaveChunk: TButton
        Left = 0
        Top = 50
        Width = 82
        Height = 25
        Align = alTop
        Caption = 'Save chunk'
        Enabled = False
        TabOrder = 2
      end
      object btnIncr: TButton
        Left = 0
        Top = 75
        Width = 82
        Height = 25
        Align = alTop
        Caption = '+'
        TabOrder = 3
        OnClick = btnIncrClick
      end
      object btnDecr: TButton
        Left = 0
        Top = 100
        Width = 82
        Height = 25
        Align = alTop
        Caption = '-'
        TabOrder = 4
        OnClick = btnDecrClick
      end
    end
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 627
      Height = 248
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
      TabOrder = 1
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
      OnSetEditText = sgChunksSetEditText
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 544
    Width = 713
    Height = 44
    Align = alBottom
    BevelEdges = []
    BevelOuter = bvNone
    TabOrder = 1
    DesignSize = (
      713
      44)
    object btnSave: TButton
      Left = 552
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Save'
      TabOrder = 0
      OnClick = btnSaveClick
    end
    object btnCancel: TButton
      Left = 633
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Exit'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object cbLog: TCheckBox
      Left = 482
      Top = 6
      Width = 64
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = 'Show Log'
      TabOrder = 2
      OnClick = cbLogClick
    end
    object radgrpRows: TRadioGroup
      Left = 2
      Top = 0
      Width = 183
      Height = 43
      Caption = 'Show rows'
      TabOrder = 3
    end
    object radbutAll: TRadioButton
      Left = 48
      Top = 16
      Width = 30
      Height = 17
      Caption = 'All'
      TabOrder = 4
      OnClick = radbutAllClick
    end
    object radbutDefault: TRadioButton
      Left = 10
      Top = 16
      Width = 33
      Height = 17
      Caption = '25'
      Checked = True
      TabOrder = 5
      TabStop = True
      OnClick = radbutDefaultClick
    end
    object radbutCustom: TRadioButton
      Left = 84
      Top = 16
      Width = 57
      Height = 17
      Caption = 'Custom'
      TabOrder = 6
      OnClick = radbutCustomClick
    end
    object editCustomRows: TEdit
      Left = 141
      Top = 15
      Width = 38
      Height = 21
      Enabled = False
      TabOrder = 7
      Text = '100'
      OnChange = editCustomRowsChange
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 275
    Width = 713
    Height = 269
    Align = alClient
    Caption = 'Boxes in chunk'
    TabOrder = 2
    object sgBoxes: TStringGrid
      Left = 2
      Top = 15
      Width = 709
      Height = 163
      Align = alClient
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect, goFixedRowClick]
      TabOrder = 0
      OnClick = sgBoxesClick
      OnFixedCellClick = sgBoxesFixedCellClick
    end
    object memoDebug: TMemo
      Left = 2
      Top = 178
      Width = 709
      Height = 89
      Align = alBottom
      TabOrder = 1
      Visible = False
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
  object timerCustomRows: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerCustomRowsTimer
    Left = 352
    Top = 296
  end
  object timerLoadBoxes: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadBoxesTimer
    Left = 464
    Top = 40
  end
end
