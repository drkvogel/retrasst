object frmSamples: TfrmSamples
  Left = 0
  Top = 0
  Caption = 'frmSamples'
  ClientHeight = 756
  ClientWidth = 746
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 265
    Width = 746
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
    Width = 746
    Height = 265
    Align = alTop
    Caption = 'Manage Chunks'
    TabOrder = 0
    ExplicitLeft = -295
    ExplicitWidth = 713
    object Panel2: TPanel
      Left = 662
      Top = 15
      Width = 82
      Height = 248
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 0
      ExplicitLeft = 629
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
      Width = 660
      Height = 248
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goHorzLine, goRangeSelect, goColSizing, goEditing, goAlwaysShowEditor, goThumbTracking]
      TabOrder = 1
      OnDrawCell = sgChunksDrawCell
      ExplicitWidth = 627
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 712
    Width = 746
    Height = 44
    Align = alBottom
    BevelEdges = []
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = -295
    ExplicitTop = 238
    ExplicitWidth = 713
    DesignSize = (
      746
      44)
    object btnSave: TButton
      Left = 585
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Save'
      TabOrder = 0
      OnClick = btnSaveClick
      ExplicitLeft = 552
    end
    object btnCancel: TButton
      Left = 666
      Top = 6
      Width = 75
      Height = 25
      Anchors = [akRight, akBottom]
      Caption = 'Exit'
      TabOrder = 1
      OnClick = btnCancelClick
      ExplicitLeft = 633
    end
    object cbLog: TCheckBox
      Left = 515
      Top = 6
      Width = 64
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = 'Show Log'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = cbLogClick
      ExplicitLeft = 482
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
    Width = 746
    Height = 437
    Align = alClient
    Caption = 'Chunk details'
    TabOrder = 2
    ExplicitLeft = -295
    ExplicitTop = 13
    ExplicitWidth = 713
    ExplicitHeight = 269
    object sgChunk: TStringGrid
      Left = 2
      Top = 15
      Width = 742
      Height = 331
      Align = alClient
      FixedCols = 0
      TabOrder = 0
      ExplicitWidth = 709
      ExplicitHeight = 163
    end
    object memoDebug: TMemo
      Left = 2
      Top = 346
      Width = 742
      Height = 89
      Align = alBottom
      Lines.Strings = (
        'memoDebug')
      TabOrder = 1
      ExplicitTop = 178
      ExplicitWidth = 709
    end
  end
  object timerCustomRows: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerCustomRowsTimer
    Left = 352
    Top = 296
  end
end
