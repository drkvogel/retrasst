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
    Height = 219
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    ExplicitTop = 105
    ExplicitWidth = 956
    ExplicitHeight = 250
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 944
      Height = 202
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      TabOrder = 0
      OnDrawCell = sgVialsDrawCell
      ExplicitLeft = 1
      ExplicitTop = 11
      ExplicitWidth = 952
      ExplicitHeight = 223
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
        ExplicitWidth = 271
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
    ExplicitTop = 355
    ExplicitWidth = 956
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
      Left = 721
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
      ExplicitLeft = 804
    end
    object btnSkip: TButton
      Left = 796
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Skip'
      TabOrder = 1
      ExplicitLeft = 879
    end
    object editBarcode: TEdit
      Left = 192
      Top = 16
      Width = 278
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
      Left = 618
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
      ExplicitLeft = 905
      ExplicitTop = 14
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
      ScrollBars = ssVertical
      TabOrder = 0
      OnClick = sgChunksClick
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
      ExplicitWidth = 952
      ExplicitHeight = 88
    end
  end
  object memoDebug: TMemo
    Left = 0
    Top = 402
    Width = 948
    Height = 50
    Align = alBottom
    TabOrder = 3
    Visible = False
    ExplicitLeft = 8
    ExplicitTop = 234
    ExplicitWidth = 956
  end
  object GroupBox3: TGroupBox
    Left = 0
    Top = 452
    Width = 948
    Height = 86
    Align = alBottom
    Caption = 'Details'
    TabOrder = 4
    ExplicitLeft = 3
    ExplicitTop = 148
    ExplicitWidth = 952
    object Label3: TLabel
      Left = 70
      Top = 52
      Width = 105
      Height = 25
      Caption = 'Sample ID:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label2: TLabel
      Left = 12
      Top = 21
      Width = 163
      Height = 25
      Caption = 'Storage Location:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
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
