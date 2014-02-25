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
  OnClose = FormClose
  OnCreate = FormCreate
  OnResize = FormResize
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
  object groupRetrievalList: TGroupBox
    Left = 0
    Top = 183
    Width = 948
    Height = 225
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object splitterDebug: TSplitter
      Left = 2
      Top = 131
      Width = 944
      Height = 5
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 124
    end
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 944
      Height = 116
      Align = alClient
      DefaultDrawing = False
      DoubleBuffered = True
      Enabled = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRowSelect]
      ParentDoubleBuffered = False
      TabOrder = 0
      OnClick = sgVialsClick
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
      object Splitter2: TSplitter
        Left = 1
        Top = 1
        Height = 85
        ExplicitLeft = 864
        ExplicitTop = 48
        ExplicitHeight = 100
      end
      object memoDebug: TMemo
        Left = 4
        Top = 1
        Width = 864
        Height = 85
        Align = alClient
        DoubleBuffered = False
        ParentDoubleBuffered = False
        ScrollBars = ssVertical
        TabOrder = 0
      end
      object btnSimAccept: TButton
        Left = 868
        Top = 1
        Width = 75
        Height = 85
        Align = alRight
        Caption = 'Sim Accept'
        TabOrder = 1
        OnClick = btnSimAcceptClick
      end
    end
  end
  object groupSignOff: TGroupBox
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
      Left = 2
      Top = 15
      Width = 139
      Height = 60
      Align = alLeft
      Caption = 'Enter barcode:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitHeight = 25
    end
    object btnAccept: TButton
      Left = 391
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
      ExplicitLeft = 365
    end
    object btnSkip: TButton
      Left = 541
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Defer'
      TabOrder = 1
      OnClick = btnSkipClick
      ExplicitLeft = 554
      ExplicitTop = 14
    end
    object editBarcode: TEdit
      Left = 141
      Top = 15
      Width = 250
      Height = 60
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -37
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnKeyUp = editBarcodeKeyUp
      ExplicitHeight = 53
    end
    object cbLog: TCheckBox
      Left = 14
      Top = 57
      Width = 64
      Height = 17
      Anchors = [akLeft, akBottom]
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
      Left = 466
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
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
  object groupDetails: TGroupBox
    Left = 0
    Top = 408
    Width = 948
    Height = 130
    Align = alBottom
    Caption = 'Details'
    TabOrder = 3
    object panelAliquots: TPanel
      Left = 2
      Top = 15
      Width = 157
      Height = 113
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 0
      object groupPrimary: TGroupBox
        Left = 0
        Top = 0
        Width = 157
        Height = 55
        Margins.Bottom = 9
        Align = alTop
        Caption = 'Primary Aliquot'
        Padding.Left = 5
        Padding.Top = 5
        Padding.Right = 5
        Padding.Bottom = 5
        TabOrder = 0
        object labelPrimary: TLabel
          Left = 7
          Top = 20
          Width = 80
          Height = 16
          Align = alClient
          Caption = 'labelPrimary'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
      end
      object groupSecondary: TGroupBox
        Left = 0
        Top = 55
        Width = 157
        Height = 58
        Align = alClient
        Caption = 'Secondary Aliquot'
        Padding.Left = 5
        Padding.Top = 5
        Padding.Right = 5
        Padding.Bottom = 5
        TabOrder = 1
        object labelSecondary: TLabel
          Left = 7
          Top = 20
          Width = 99
          Height = 16
          Margins.Left = 5
          Margins.Top = 5
          Margins.Right = 5
          Margins.Bottom = 5
          Align = alClient
          Caption = 'labelSecondary'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -13
          Font.Name = 'Tahoma'
          Font.Style = [fsBold]
          ParentFont = False
          Layout = tlCenter
        end
      end
    end
    object Panel1: TPanel
      Left = 425
      Top = 15
      Width = 521
      Height = 113
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object Label2: TLabel
        Left = 17
        Top = 19
        Width = 38
        Height = 25
        Caption = 'Loc:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelStorage: TLabel
        Left = 74
        Top = 3
        Width = 167
        Height = 37
        Caption = 'labelStorage'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -31
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelDestbox: TLabel
        Left = 84
        Top = 54
        Width = 173
        Height = 37
        Caption = 'labelDestbox'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -31
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label4: TLabel
        Left = 17
        Top = 67
        Width = 48
        Height = 25
        Caption = 'Dest:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
    object Panel2: TPanel
      Left = 159
      Top = 15
      Width = 266
      Height = 113
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 2
      object Label3: TLabel
        Left = 9
        Top = 12
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
      object labelSampleID: TLabel
        Left = 9
        Top = 43
        Width = 231
        Height = 45
        Caption = 'labelSampleID'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -37
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
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
