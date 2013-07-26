object frmRetrievalManager: TfrmRetrievalManager
  Left = 0
  Top = 0
  Caption = 'Retrieval Manager'
  ClientHeight = 615
  ClientWidth = 547
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 265
    Width = 547
    Height = 10
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitWidth = 841
  end
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 547
    Height = 265
    Align = alTop
    Caption = 'Manage Chunks'
    TabOrder = 0
    ExplicitWidth = 841
    object Panel2: TPanel
      Left = 463
      Top = 15
      Width = 82
      Height = 248
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitLeft = 648
      ExplicitHeight = 128
      object btnAddChunk: TButton
        Left = 6
        Top = 0
        Width = 75
        Height = 25
        Caption = 'Add chunk'
        TabOrder = 0
        OnClick = btnAddChunkClick
      end
      object btnDelChunk: TButton
        Left = 6
        Top = 31
        Width = 75
        Height = 25
        Caption = 'Del chunk'
        Enabled = False
        TabOrder = 1
        OnClick = btnDelChunkClick
      end
      object btnSaveChunk: TButton
        Left = 6
        Top = 62
        Width = 75
        Height = 25
        Caption = 'Save chunk'
        Enabled = False
        TabOrder = 2
      end
      object btnIncr: TButton
        Left = 6
        Top = 93
        Width = 75
        Height = 25
        Caption = '+'
        TabOrder = 3
      end
      object btnDecr: TButton
        Left = 6
        Top = 124
        Width = 75
        Height = 25
        Caption = '-'
        TabOrder = 4
      end
    end
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 461
      Height = 248
      Align = alClient
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
      TabOrder = 1
      OnDrawCell = sgChunksDrawCell
      OnSetEditText = sgChunksSetEditText
      ExplicitWidth = 646
      ExplicitHeight = 128
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 574
    Width = 547
    Height = 41
    Align = alBottom
    BevelEdges = []
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 428
    ExplicitWidth = 732
    object Label1: TLabel
      Left = 64
      Top = 16
      Width = 31
      Height = 13
      Caption = 'Label1'
    end
    object btnSave: TButton
      Left = 400
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
    end
    object btnCancel: TButton
      Left = 520
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object cbLog: TCheckBox
      Left = 200
      Top = 16
      Width = 97
      Height = 17
      Caption = 'Show Log'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = cbLogClick
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 275
    Width = 547
    Height = 299
    Align = alClient
    Caption = 'Chunk details'
    TabOrder = 2
    ExplicitTop = 277
    ExplicitWidth = 841
    object sgChunk: TStringGrid
      Left = 2
      Top = 15
      Width = 543
      Height = 193
      Align = alClient
      FixedCols = 0
      TabOrder = 0
      ExplicitWidth = 728
      ExplicitHeight = 263
    end
    object memoDebug: TMemo
      Left = 2
      Top = 208
      Width = 543
      Height = 89
      Align = alBottom
      Lines.Strings = (
        'memoDebug')
      TabOrder = 1
      ExplicitLeft = 88
      ExplicitTop = 192
      ExplicitWidth = 185
    end
  end
end
