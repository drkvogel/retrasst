object frmRetrAsstCollectSamples: TfrmRetrAsstCollectSamples
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
  KeyPreview = True
  Menu = mainMenuMain
  OldCreateOrder = False
  WindowState = wsMaximized
  OnClose = FormClose
  OnCreate = FormCreate
  OnKeyUp = FormKeyUp
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
    Height = 241
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object splitterDebug: TSplitter
      Left = 2
      Top = 96
      Width = 944
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 78
    end
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 944
      Height = 81
      Align = alClient
      DefaultDrawing = False
      DoubleBuffered = True
      Enabled = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
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
      Top = 104
      Width = 944
      Height = 135
      Align = alBottom
      BevelEdges = []
      Caption = 'panelDebug'
      TabOrder = 2
      object Splitter2: TSplitter
        Left = 1
        Top = 1
        Height = 133
        ExplicitLeft = 864
        ExplicitTop = 48
        ExplicitHeight = 100
      end
      object memoDebug: TMemo
        Left = 4
        Top = 1
        Width = 864
        Height = 133
        Align = alClient
        DoubleBuffered = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Consolas'
        Font.Style = []
        ParentDoubleBuffered = False
        ParentFont = False
        ScrollBars = ssVertical
        TabOrder = 0
        ExplicitLeft = -2
        ExplicitTop = -2
      end
      object btnSimAccept: TButton
        Left = 868
        Top = 1
        Width = 75
        Height = 133
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
    object btnAccept: TButton
      Left = 730
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
    end
    object btnSkip: TButton
      Left = 880
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Defer'
      TabOrder = 1
      OnClick = btnSkipClick
    end
    object editBarcode: TEdit
      Left = 433
      Top = 15
      Width = 297
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
    object btnNotFound: TButton
      Left = 805
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Not Found'
      TabOrder = 3
      OnClick = btnNotFoundClick
    end
    object Panel1: TPanel
      Left = 2
      Top = 15
      Width = 263
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 4
      DesignSize = (
        263
        60)
      object btnExit: TButton
        Left = 0
        Top = 0
        Width = 75
        Height = 60
        Align = alLeft
        Caption = 'Exit'
        TabOrder = 0
        OnClick = btnExitClick
      end
      object cbLog: TCheckBox
        Left = 81
        Top = 1
        Width = 64
        Height = 17
        Anchors = [akRight, akBottom]
        Caption = 'Show Log'
        TabOrder = 1
        OnClick = cbLogClick
      end
    end
    object Panel2: TPanel
      Left = 265
      Top = 15
      Width = 168
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 5
      object Label1: TLabel
        Left = 40
        Top = 0
        Width = 50
        Height = 25
        Caption = 'Enter'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 32
        Top = 31
        Width = 89
        Height = 25
        Caption = ' barcode:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
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
      Width = 880
      Height = 156
      Align = alClient
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
      ScrollBars = ssVertical
      TabOrder = 0
      OnClick = sgChunksClick
      OnDrawCell = sgChunksDrawCell
      OnFixedCellClick = sgChunksFixedCellClick
    end
    object panelLegends: TPanel
      Left = 882
      Top = 15
      Width = 64
      Height = 156
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      object groupChunkKey: TGroupBox
        Left = 0
        Top = 0
        Width = 64
        Height = 57
        Align = alTop
        Caption = 'Chunk'
        TabOrder = 0
        object labelNew: TLabel
          Left = 2
          Top = 15
          Width = 21
          Height = 13
          Align = alTop
          Caption = 'New'
          Color = clSkyBlue
          ParentColor = False
          Transparent = False
        end
        object labelInProgress: TLabel
          Left = 2
          Top = 28
          Width = 55
          Height = 13
          Align = alTop
          Caption = 'In Progress'
          Color = clMoneyGreen
          ParentColor = False
          Transparent = False
        end
        object labelDone: TLabel
          Left = 2
          Top = 41
          Width = 51
          Height = 13
          Align = alTop
          Caption = 'Completed'
          Transparent = False
        end
      end
      object groupVialKey: TGroupBox
        Left = 0
        Top = 72
        Width = 64
        Height = 84
        Align = alBottom
        Caption = 'Tube'
        TabOrder = 1
        object labelVialKeyCollected: TLabel
          Left = 2
          Top = 41
          Width = 47
          Height = 13
          Align = alTop
          Caption = 'Retrieved'
          Color = clAqua
          ParentColor = False
          Transparent = False
        end
        object labelVialKeyExpectedBackup: TLabel
          Left = 2
          Top = 28
          Width = 34
          Height = 13
          Align = alTop
          Caption = 'Backup'
          Color = clYellow
          ParentColor = False
          Transparent = False
        end
        object labelVialKeyExpectedPrimary: TLabel
          Left = 2
          Top = 15
          Width = 46
          Height = 13
          Align = alTop
          Caption = 'Preferred'
          Color = clHighlight
          ParentColor = False
          Transparent = False
        end
        object labelVialKeyIgnored: TLabel
          Left = 2
          Top = 67
          Width = 38
          Height = 13
          Align = alTop
          Caption = 'Ignored'
          Color = clLime
          ParentColor = False
          Transparent = False
        end
        object labelVialKeyNotFound: TLabel
          Left = 2
          Top = 54
          Width = 50
          Height = 13
          Align = alTop
          Caption = 'Not Found'
          Color = clFuchsia
          ParentColor = False
          Transparent = False
        end
      end
    end
  end
  object panelDetails: TPanel
    Left = 0
    Top = 424
    Width = 948
    Height = 114
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    object groupSource: TGroupBox
      Left = 0
      Top = 0
      Width = 431
      Height = 114
      Align = alLeft
      Caption = 'Source'
      ParentBackground = False
      TabOrder = 0
      object labelSite: TLabel
        Left = 361
        Top = 78
        Width = 35
        Height = 11
        Caption = 'labelSite'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -9
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelVessel: TLabel
        Left = 233
        Top = 68
        Width = 102
        Height = 25
        Caption = 'labelVessel'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelStructure: TLabel
        Left = 41
        Top = 68
        Width = 131
        Height = 25
        Caption = 'labelStructure'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelSlot: TLabel
        Left = 16
        Top = 70
        Width = 70
        Height = 23
        Caption = 'labelSlot'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -19
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelSrcPos: TLabel
        Left = 3
        Top = 13
        Width = 238
        Height = 58
        Caption = 'labelSrcPos'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -48
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelSrcBox: TLabel
        Left = 83
        Top = 29
        Width = 108
        Height = 25
        Caption = 'labelSrcBox'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelStorage: TLabel
        Left = 2
        Top = 99
        Width = 60
        Height = 13
        Align = alBottom
        Caption = 'labelStorage'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
    object groupSampleID: TGroupBox
      Left = 431
      Top = 0
      Width = 297
      Height = 114
      Align = alLeft
      Caption = 'Sample ID'
      TabOrder = 1
      object labelSampleID: TLabel
        Left = 17
        Top = 13
        Width = 275
        Height = 52
        Caption = 'labelSampleID'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -43
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object panelAliquots: TPanel
        Left = 2
        Top = 72
        Width = 293
        Height = 40
        Margins.Top = 1
        Margins.Bottom = 1
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        object groupPrimary: TGroupBox
          Left = 0
          Top = 0
          Width = 161
          Height = 40
          Align = alLeft
          Caption = 'Primary Aliquot'
          Padding.Left = 5
          Padding.Top = 1
          Padding.Right = 5
          Padding.Bottom = 1
          TabOrder = 0
          ExplicitHeight = 47
          object labelPrimary: TLabel
            Left = 7
            Top = 16
            Width = 80
            Height = 16
            Margins.Top = 1
            Margins.Bottom = 1
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
          Left = 147
          Top = 0
          Width = 146
          Height = 40
          Align = alRight
          Caption = 'Secondary Aliquot'
          Padding.Left = 5
          Padding.Top = 1
          Padding.Right = 5
          Padding.Bottom = 1
          TabOrder = 1
          ExplicitHeight = 47
          object labelSecondary: TLabel
            Left = 7
            Top = 16
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
    end
    object groupDest: TGroupBox
      Left = 728
      Top = 0
      Width = 220
      Height = 114
      Align = alClient
      Caption = 'Destination'
      ParentBackground = False
      TabOrder = 2
      object labelDestString: TLabel
        Left = 2
        Top = 99
        Width = 72
        Height = 13
        Align = alBottom
        Caption = 'labelDestString'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelDestPos: TLabel
        Left = 6
        Top = 16
        Width = 267
        Height = 58
        Caption = 'labelDestPos'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -48
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelDestBox: TLabel
        Left = 80
        Top = 29
        Width = 119
        Height = 25
        Caption = 'labelDestBox'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
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
