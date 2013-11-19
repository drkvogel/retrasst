object frmProcess: TfrmProcess
  Left = 0
  Top = 0
  Caption = 'Process Retrieval Job'
  ClientHeight = 615
  ClientWidth = 948
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = mainMenuMain
  OldCreateOrder = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter1: TSplitter
    Left = 0
    Top = 173
    Width = 948
    Height = 10
    Cursor = crVSplit
    Align = alTop
    Beveled = True
    ExplicitTop = 105
    ExplicitWidth = 956
  end
  object List: TGroupBox
    Left = 0
    Top = 183
    Width = 948
    Height = 225
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 944
      Height = 121
      Align = alClient
      DefaultDrawing = False
      Enabled = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      TabOrder = 0
      OnDrawCell = sgVialsDrawCell
    end
    object panelLoading: TPanel
      Left = 248
      Top = 64
      Width = 521
      Height = 127
      Caption = 'Loading retrieval list, please wait...'
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
    object panelDebug: TPanel
      Left = 2
      Top = 136
      Width = 944
      Height = 87
      Align = alBottom
      BevelEdges = []
      Caption = 'panelDebug'
      TabOrder = 2
      object memoDebug: TMemo
        Left = 1
        Top = 1
        Width = 664
        Height = 85
        Align = alLeft
        TabOrder = 0
      end
      object btnSimAccept: TButton
        Left = 736
        Top = 8
        Width = 75
        Height = 25
        Caption = 'Sim Accept'
        TabOrder = 1
        OnClick = btnSimAcceptClick
      end
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 538
    Width = 948
    Height = 77
    Align = alBottom
    Caption = 'Sign Off'
    TabOrder = 1
    DesignSize = (
      948
      77)
    object Label1: TLabel
      Left = 14
      Top = 32
      Width = 139
      Height = 25
      Caption = 'Enter barcode:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnAccept: TButton
      Left = 646
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
    end
    object btnSkip: TButton
      Left = 721
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Skip'
      TabOrder = 1
      OnClick = btnSkipClick
    end
    object editBarcode: TEdit
      Left = 159
      Top = 16
      Width = 311
      Height = 53
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -37
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
    object cbLog: TCheckBox
      Left = 514
      Top = 46
      Width = 64
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = 'Show Log'
      TabOrder = 3
      OnClick = cbLogClick
    end
    object btnExit: TButton
      Left = 871
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Exit'
      TabOrder = 4
      OnClick = btnExitClick
    end
    object btnNotFound: TButton
      Left = 796
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Not Found'
      TabOrder = 5
      OnClick = btnNotFoundClick
    end
  end
  object Chunks: TGroupBox
    Left = 0
    Top = 0
    Width = 948
    Height = 173
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 2
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 944
      Height = 156
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ScrollBars = ssVertical
      TabOrder = 0
      OnClick = sgChunksClick
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
    end
  end
  object GroupBox3: TGroupBox
    Left = 0
    Top = 408
    Width = 948
    Height = 130
    Align = alBottom
    Caption = 'Details'
    TabOrder = 3
    object Label3: TLabel
      Left = 49
      Top = 20
      Width = 29
      Height = 25
      Caption = 'ID:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 340
      Top = 19
      Width = 84
      Height = 25
      Caption = 'Location:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object labelStorage: TLabel
      Left = 339
      Top = 43
      Width = 198
      Height = 45
      Caption = 'labelStorage'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -37
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object labelSampleID: TLabel
      Left = 49
      Top = 43
      Width = 253
      Height = 48
      Caption = 'labelSampleID'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -40
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
  end
  object mainMenuMain: TMainMenu
    Left = 512
    Top = 65520
    object menuItemExit: TMenuItem
      Caption = 'E&xit'
      OnClick = menuItemExitClick
    end
  end
  object timerLoadPlan: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerLoadPlanTimer
    Left = 368
    Top = 16
  end
end
