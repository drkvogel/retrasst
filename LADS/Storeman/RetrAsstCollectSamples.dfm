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
    Height = 225
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object splitterDebug: TSplitter
      Left = 2
      Top = 80
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
      Height = 65
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
      Top = 88
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
      Left = 622
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
    end
    object btnSkip: TButton
      Left = 772
      Top = 15
      Width = 75
      Height = 60
      Align = alLeft
      Caption = 'Defer'
      TabOrder = 1
      OnClick = btnSkipClick
    end
    object editBarcode: TEdit
      Left = 372
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
    object btnNotFound: TButton
      Left = 697
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
      Width = 185
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 4
      DesignSize = (
        185
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
        Left = 121
        Top = 17
        Width = 64
        Height = 17
        Anchors = [akRight, akBottom]
        Caption = 'Show Log'
        TabOrder = 1
        OnClick = cbLogClick
      end
    end
    object Panel2: TPanel
      Left = 187
      Top = 15
      Width = 185
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
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'New'
          Color = clSkyBlue
          ParentColor = False
          Transparent = False
          ExplicitWidth = 21
        end
        object labelInProgress: TLabel
          Left = 2
          Top = 28
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'In Progress'
          Color = clMoneyGreen
          ParentColor = False
          Transparent = False
          ExplicitWidth = 55
        end
        object labelDone: TLabel
          Left = 2
          Top = 41
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Completed'
          Transparent = False
          ExplicitWidth = 51
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
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Retrieved'
          Color = clAqua
          ParentColor = False
          Transparent = False
          ExplicitWidth = 47
        end
        object labelVialKeyExpectedBackup: TLabel
          Left = 2
          Top = 28
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Backup'
          Color = clYellow
          ParentColor = False
          Transparent = False
          ExplicitWidth = 34
        end
        object labelVialKeyExpectedPrimary: TLabel
          Left = 2
          Top = 15
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Preferred'
          Color = clHighlight
          ParentColor = False
          Transparent = False
          ExplicitWidth = 46
        end
        object labelVialKeyIgnored: TLabel
          Left = 2
          Top = 67
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Ignored'
          Color = clLime
          ParentColor = False
          Transparent = False
          ExplicitWidth = 38
        end
        object labelVialKeyNotFound: TLabel
          Left = 2
          Top = 54
          Width = 60
          Height = 13
          Align = alTop
          Caption = 'Not Found'
          Color = clFuchsia
          ParentColor = False
          Transparent = False
          ExplicitWidth = 50
        end
      end
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
    object panelBarcode: TPanel
      Left = 165
      Top = 26
      Width = 236
      Height = 113
      BevelOuter = bvNone
      TabOrder = 0
    end
    object groupSampleID: TGroupBox
      Left = 372
      Top = 15
      Width = 292
      Height = 113
      Align = alClient
      Caption = 'Sample ID'
      TabOrder = 1
      object labelSampleID: TLabel
        Left = 17
        Top = 13
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
      object panelAliquots: TPanel
        Left = 2
        Top = 64
        Width = 288
        Height = 47
        Align = alBottom
        BevelOuter = bvNone
        TabOrder = 0
        object groupPrimary: TGroupBox
          Left = 0
          Top = 0
          Width = 161
          Height = 47
          Margins.Bottom = 9
          Align = alLeft
          Caption = 'Primary Aliquot'
          Padding.Left = 5
          Padding.Top = 5
          Padding.Right = 5
          Padding.Bottom = 5
          TabOrder = 0
          object labelPrimary: TLabel
            Left = 7
            Top = 20
            Width = 147
            Height = 20
            Align = alClient
            Caption = 'labelPrimary'
            Font.Charset = DEFAULT_CHARSET
            Font.Color = clWindowText
            Font.Height = -13
            Font.Name = 'Tahoma'
            Font.Style = [fsBold]
            ParentFont = False
            Layout = tlCenter
            ExplicitWidth = 80
            ExplicitHeight = 16
          end
        end
        object groupSecondary: TGroupBox
          Left = 142
          Top = 0
          Width = 146
          Height = 47
          Align = alRight
          Caption = 'Secondary Aliquot'
          Padding.Left = 5
          Padding.Top = 5
          Padding.Right = 5
          Padding.Bottom = 5
          TabOrder = 1
          object labelSecondary: TLabel
            Left = 7
            Top = 20
            Width = 132
            Height = 20
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
            ExplicitWidth = 99
            ExplicitHeight = 16
          end
        end
      end
    end
    object groupSource: TGroupBox
      Left = 2
      Top = 15
      Width = 370
      Height = 113
      Align = alLeft
      Caption = 'Source'
      ParentBackground = False
      TabOrder = 2
      ExplicitLeft = 5
      ExplicitTop = 11
      object labelStorage: TLabel
        Left = 2
        Top = 92
        Width = 366
        Height = 19
        Align = alBottom
        Caption = 'labelStorage'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ExplicitWidth = 87
      end
      object labelSite: TLabel
        Left = 24
        Top = 72
        Width = 40
        Height = 13
        Caption = 'labelSite'
      end
      object labelVessel: TLabel
        Left = 16
        Top = 48
        Width = 52
        Height = 13
        Caption = 'labelVessel'
      end
      object labelStructure: TLabel
        Left = 16
        Top = 24
        Width = 67
        Height = 13
        Caption = 'labelStructure'
      end
      object labelSlot: TLabel
        Left = 64
        Top = 16
        Width = 40
        Height = 13
        Caption = 'labelSlot'
      end
      object labelPosInBox: TLabel
        Left = 128
        Top = 16
        Width = 67
        Height = 13
        Caption = 'labelPosInBox'
      end
    end
    object groupDest: TGroupBox
      Left = 664
      Top = 15
      Width = 282
      Height = 113
      Align = alRight
      Caption = 'Destination'
      ParentBackground = False
      TabOrder = 3
      ExplicitLeft = 663
      ExplicitTop = 11
      object labelDestString: TLabel
        Left = 2
        Top = 92
        Width = 278
        Height = 19
        Align = alBottom
        Caption = 'labelDestString'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ExplicitWidth = 106
      end
      object labelDestPos: TLabel
        Left = 32
        Top = 24
        Width = 61
        Height = 13
        Caption = 'labelDestPos'
      end
      object labelDestBox: TLabel
        Left = 32
        Top = 56
        Width = 62
        Height = 13
        Caption = 'labelDestBox'
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
