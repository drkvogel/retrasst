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
      ExplicitHeight = 68
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
        ExplicitLeft = -2
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
      Left = 801
      Top = 17
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
      Width = 874
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
    object groupChunkKey: TGroupBox
      Left = 876
      Top = 15
      Width = 70
      Height = 156
      Align = alRight
      Caption = 'Key'
      TabOrder = 1
      object labelNew: TLabel
        Left = 2
        Top = 15
        Width = 66
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
        Width = 66
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
        Width = 66
        Height = 13
        Align = alTop
        Caption = 'Completed'
        Transparent = False
        ExplicitWidth = 51
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
          Width = 143
          Height = 28
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
          Width = 143
          Height = 31
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
    object Panel1: TPanel
      Left = 425
      Top = 15
      Width = 453
      Height = 113
      Align = alClient
      BevelOuter = bvNone
      TabOrder = 1
      object Label2: TLabel
        Left = 17
        Top = 10
        Width = 24
        Height = 16
        Caption = 'Loc:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelStorage: TLabel
        Left = 51
        Top = -1
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
        Left = 51
        Top = 35
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
        Top = 49
        Width = 30
        Height = 16
        Caption = 'Dest:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelDestype: TLabel
        Left = 51
        Top = 72
        Width = 172
        Height = 37
        Caption = 'labelDestype'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -31
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label6: TLabel
        Left = 17
        Top = 87
        Width = 33
        Height = 16
        Caption = 'Type:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
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
        Width = 17
        Height = 16
        Caption = 'ID:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -13
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object labelSampleID: TLabel
        Left = 6
        Top = 34
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
    object groupVialKey: TGroupBox
      Left = 878
      Top = 15
      Width = 68
      Height = 113
      Align = alRight
      Caption = 'Key'
      TabOrder = 3
      object labelVialKeyCollected: TLabel
        Left = 2
        Top = 41
        Width = 64
        Height = 13
        Align = alTop
        Caption = 'Collected'
        Color = clAqua
        ParentColor = False
        Transparent = False
        ExplicitWidth = 44
      end
      object labelVialKeyExpectedBackup: TLabel
        Left = 2
        Top = 28
        Width = 64
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
        Width = 64
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
        Width = 64
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
        Width = 64
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
