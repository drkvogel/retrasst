object frmRetrievalManager: TfrmRetrievalManager
  Left = 0
  Top = 0
  Caption = 'Retrieval Manager'
  ClientHeight = 469
  ClientWidth = 732
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 145
    Width = 732
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitLeft = 114
    ExplicitTop = 87
    ExplicitWidth = 728
  end
  object groupList: TGroupBox
    Left = 0
    Top = 0
    Width = 732
    Height = 145
    Align = alTop
    Caption = 'Manage Chunks'
    TabOrder = 0
    ExplicitTop = 3
    object Panel2: TPanel
      Left = 648
      Top = 15
      Width = 82
      Height = 128
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      object btnAddChunk: TButton
        Left = 6
        Top = 0
        Width = 75
        Height = 25
        Caption = '+'
        TabOrder = 0
      end
      object btnDelChunk: TButton
        Left = 6
        Top = 31
        Width = 75
        Height = 25
        Caption = '-'
        TabOrder = 1
      end
      object btnSaveChunk: TButton
        Left = 6
        Top = 62
        Width = 75
        Height = 25
        Caption = 'Save Chunk'
        Enabled = False
        TabOrder = 2
      end
    end
    object sgChunkList: TStringGrid
      Left = 2
      Top = 15
      Width = 646
      Height = 128
      Align = alClient
      FixedCols = 0
      TabOrder = 1
      ExplicitLeft = 120
      ExplicitTop = 56
      ExplicitWidth = 320
      ExplicitHeight = 120
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 428
    Width = 732
    Height = 41
    Align = alBottom
    BevelEdges = []
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 224
    ExplicitTop = 368
    ExplicitWidth = 185
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
    end
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 148
    Width = 732
    Height = 280
    Align = alClient
    Caption = 'Chunk'
    TabOrder = 2
    ExplicitTop = 149
    object sgChunk: TStringGrid
      Left = 2
      Top = 15
      Width = 728
      Height = 263
      Align = alClient
      FixedCols = 0
      TabOrder = 0
      ExplicitTop = 19
    end
  end
end
