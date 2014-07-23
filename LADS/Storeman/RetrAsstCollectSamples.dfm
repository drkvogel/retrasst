object frmRetrAsstCollectSamples: TfrmRetrAsstCollectSamples
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsSingle
  Caption = 'Process Retrieval Job'
  ClientHeight = 631
  ClientWidth = 1290
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
    Width = 1290
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
    Width = 1290
    Height = 257
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    ExplicitWidth = 948
    ExplicitHeight = 241
    object splitterDebug: TSplitter
      Left = 2
      Top = 112
      Width = 1286
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 78
      ExplicitWidth = 944
    end
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 1286
      Height = 97
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
      ExplicitWidth = 944
      ExplicitHeight = 81
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
      Top = 120
      Width = 1286
      Height = 135
      Align = alBottom
      BevelEdges = []
      Caption = 'panelDebug'
      TabOrder = 2
      ExplicitTop = 104
      ExplicitWidth = 944
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
        Width = 1206
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
        ExplicitWidth = 864
      end
      object btnSimAccept: TButton
        Left = 1210
        Top = 1
        Width = 75
        Height = 133
        Align = alRight
        Caption = 'Sim Accept'
        TabOrder = 1
        OnClick = btnSimAcceptClick
        ExplicitLeft = 868
      end
    end
  end
  object groupSignOff: TGroupBox
    Left = 0
    Top = 554
    Width = 1290
    Height = 77
    Align = alBottom
    Caption = 'Sign Off'
    TabOrder = 1
    ExplicitTop = 543
    ExplicitWidth = 988
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
      TabOrder = 0
      OnKeyUp = editBarcodeKeyUp
      ExplicitHeight = 53
    end
    object Panel1: TPanel
      Left = 2
      Top = 15
      Width = 263
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 1
      DesignSize = (
        263
        60)
      object btnExit: TButton
        Left = 0
        Top = 0
        Width = 75
        Height = 60
        Align = alLeft
        Caption = 'E&xit'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Verdana'
        Font.Style = [fsBold]
        ParentFont = False
        TabOrder = 0
        OnClick = btnExitClick
      end
      object cbLog: TCheckBox
        Left = 81
        Top = 1
        Width = 64
        Height = 17
        Anchors = [akRight, akBottom]
        Caption = 'dbg'
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
      TabOrder = 2
      object Label1: TLabel
        Left = 8
        Top = 16
        Width = 154
        Height = 25
        Caption = 'Enter barcode >'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
    object btnAccept: TBitBtn
      Left = 730
      Top = 15
      Width = 111
      Height = 60
      Align = alLeft
      Caption = '&Accept'
      Default = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333333333333330000333333333333333333333333F33333333333
        00003333344333333333333333388F3333333333000033334224333333333333
        338338F3333333330000333422224333333333333833338F3333333300003342
        222224333333333383333338F3333333000034222A22224333333338F338F333
        8F33333300003222A3A2224333333338F3838F338F33333300003A2A333A2224
        33333338F83338F338F33333000033A33333A222433333338333338F338F3333
        0000333333333A222433333333333338F338F33300003333333333A222433333
        333333338F338F33000033333333333A222433333333333338F338F300003333
        33333333A222433333333333338F338F00003333333333333A22433333333333
        3338F38F000033333333333333A223333333333333338F830000333333333333
        333A333333333333333338330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
      ParentFont = False
      TabOrder = 3
      OnClick = btnAcceptClick
    end
    object btnNotFound: TBitBtn
      Left = 841
      Top = 15
      Width = 120
      Height = 60
      Align = alLeft
      Cancel = True
      Caption = '&Not Found'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
        3333333333333FFFFF333333000033333388888833333333333F888888FFF333
        000033338811111188333333338833FFF388FF33000033381119999111833333
        38F338888F338FF30000339119933331111833338F388333383338F300003391
        13333381111833338F8F3333833F38F3000039118333381119118338F38F3338
        33F8F38F000039183333811193918338F8F333833F838F8F0000391833381119
        33918338F8F33833F8338F8F000039183381119333918338F8F3833F83338F8F
        000039183811193333918338F8F833F83333838F000039118111933339118338
        F3833F83333833830000339111193333391833338F33F8333FF838F300003391
        11833338111833338F338FFFF883F83300003339111888811183333338FF3888
        83FF83330000333399111111993333333388FFFFFF8833330000333333999999
        3333333333338888883333330000333333333333333333333333333333333333
        0000}
      NumGlyphs = 2
      ParentFont = False
      TabOrder = 4
      OnClick = btnNotFoundClick
      ExplicitLeft = 847
    end
    object btnDefer: TBitBtn
      Left = 961
      Top = 15
      Width = 104
      Height = 60
      Align = alLeft
      Caption = '&Defer'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      Glyph.Data = {
        DE010000424DDE01000000000000760000002800000024000000120000000100
        0400000000006801000000000000000000001000000000000000000000000000
        80000080000000808000800000008000800080800000C0C0C000808080000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00344446333334
        44433333FFFF333333FFFF33000033AAA43333332A4333338833F33333883F33
        00003332A46333332A4333333383F33333383F3300003332A2433336A6633333
        33833F333383F33300003333AA463362A433333333383F333833F33300003333
        6AA4462A46333333333833FF833F33330000333332AA22246333333333338333
        33F3333300003333336AAA22646333333333383333F8FF33000033444466AA43
        6A43333338FFF8833F383F330000336AA246A2436A43333338833F833F383F33
        000033336A24AA442A433333333833F33FF83F330000333333A2AA2AA4333333
        333383333333F3330000333333322AAA4333333333333833333F333300003333
        333322A4333333333333338333F333330000333333344A433333333333333338
        3F333333000033333336A24333333333333333833F333333000033333336AA43
        33333333333333833F3333330000333333336663333333333333333888333333
        0000}
      NumGlyphs = 2
      ParentFont = False
      TabOrder = 5
      OnClick = btnDeferClick
      ExplicitLeft = 953
    end
  end
  object Chunks: TGroupBox
    Left = 0
    Top = 0
    Width = 1290
    Height = 173
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 2
    ExplicitWidth = 948
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 1222
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
      ExplicitWidth = 880
    end
    object panelLegends: TPanel
      Left = 1224
      Top = 15
      Width = 64
      Height = 156
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitLeft = 882
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
  object panelDetails: TPanel
    Left = 0
    Top = 440
    Width = 1290
    Height = 114
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 3
    ExplicitTop = 424
    ExplicitWidth = 948
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
        Width = 427
        Height = 13
        Align = alBottom
        Caption = 'labelStorage'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ExplicitWidth = 60
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
          object labelPrimary: TLabel
            Left = 7
            Top = 16
            Width = 147
            Height = 21
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
            ExplicitWidth = 80
            ExplicitHeight = 16
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
          object labelSecondary: TLabel
            Left = 7
            Top = 16
            Width = 132
            Height = 21
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
    object groupDest: TGroupBox
      Left = 728
      Top = 0
      Width = 562
      Height = 114
      Align = alClient
      Caption = 'Destination'
      ParentBackground = False
      TabOrder = 2
      ExplicitWidth = 220
      object labelDestString: TLabel
        Left = 2
        Top = 99
        Width = 558
        Height = 13
        Align = alBottom
        Caption = 'labelDestString'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -11
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ExplicitWidth = 72
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
