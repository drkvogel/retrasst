object frmRetrAsstCollectSamples: TfrmRetrAsstCollectSamples
  Left = 0
  Top = 0
  Caption = 'Process Retrieval Job'
  ClientHeight = 621
  ClientWidth = 1280
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
    Width = 1280
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
    Width = 1280
    Height = 247
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object splitterDebug: TSplitter
      Left = 2
      Top = 102
      Width = 1276
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 78
      ExplicitWidth = 944
    end
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 1276
      Height = 87
      Align = alClient
      DefaultDrawing = False
      DoubleBuffered = True
      Enabled = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
      ParentDoubleBuffered = False
      TabOrder = 0
      OnClick = sgVialsClick
      OnContextPopup = sgVialsContextPopup
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
      Top = 110
      Width = 1276
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
        Width = 1196
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
        Left = 1200
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
    Top = 544
    Width = 1280
    Height = 77
    Align = alBottom
    Caption = 'Actions'
    TabOrder = 1
    ExplicitLeft = 2
    ExplicitTop = 546
    object editBarcode: TEdit
      Left = 431
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
      Width = 327
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      TabOrder = 1
      object btnAddNote: TBitBtn
        Left = 231
        Top = 0
        Width = 96
        Height = 60
        Hint = 'Add a note'
        Align = alRight
        Caption = 'Note'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Verdana'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555500000000
          0555555F7777777775F55500FFFFFFFFF0555577F5FFFFFFF7F550F0FEEEEEEE
          F05557F7F777777757F550F0FFFFFFFFF05557F7F5FFFFFFF7F550F0FEEEEEEE
          F05557F7F777777757F550F0FF777FFFF05557F7F5FFFFFFF7F550F0FEEEEEEE
          F05557F7F777777757F550F0FF7F777FF05557F7F5FFFFFFF7F550F0FEEEEEEE
          F05557F7F777777757F550F0FF77F7FFF05557F7F5FFFFFFF7F550F0FEEEEEEE
          F05557F7F777777757F550F0FFFFFFFFF05557F7FF5F5F5F57F550F00F0F0F0F
          005557F77F7F7F7F77555055070707070555575F7F7F7F7F7F55550507070707
          0555557575757575755555505050505055555557575757575555}
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 0
        OnClick = btnAddNoteClick
        ExplicitLeft = 175
      end
      object btnSignOff: TBitBtn
        Left = 0
        Top = 0
        Width = 105
        Height = 60
        Align = alLeft
        Caption = 'Sign &Off'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Verdana'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00555775777777
          57705557757777775FF7555555555555000755555555555F777F555555555550
          87075555555555F7577F5555555555088805555555555F755F75555555555033
          805555555555F755F75555555555033B05555555555F755F75555555555033B0
          5555555555F755F75555555555033B05555555555F755F75555555555033B055
          55555555F755F75555555555033B05555555555F755F75555555555033B05555
          555555F75FF75555555555030B05555555555F7F7F75555555555000B0555555
          5555F777F7555555555501900555555555557777755555555555099055555555
          5555777755555555555550055555555555555775555555555555}
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 1
        OnClick = btnSignOffClick
      end
      object btnBadVial: TBitBtn
        Left = 133
        Top = 0
        Width = 98
        Height = 60
        Align = alRight
        Caption = 'Bad'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = [fsBold]
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000120B0000120B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00300000000000
          000037777777777777770FFFFFFFFFFFFFF07F3333FFF33333370FFFF777FFFF
          FFF07F333777333333370FFFFFFFFFFFFFF07F3333FFFFFF33370FFFF7777BBF
          FFF07F333777777F3FF70FFFFFFFB9BF1CC07F3FFF337F7377770F777FFFB99B
          C1C07F7773337F377F370FFFFFFFB9BBC1C07FFFFFFF7F337FF700000077B999
          B000777777777F33777733337377B9999B33333F733373F337FF3377377B99BB
          9BB33377F337F377377F3737377B9B79B9B737F73337F7F7F37F33733777BB7B
          BBB73373333377F37F3737333777BB777B9B3733333377F337F7333333777B77
          77BB3333333337333377333333333777337B3333333333333337}
        NumGlyphs = 2
        ParentFont = False
        TabOrder = 2
        OnClick = btnBadVialClick
        ExplicitLeft = 127
      end
    end
    object Panel2: TPanel
      Left = 329
      Top = 15
      Width = 102
      Height = 60
      Align = alLeft
      BevelOuter = bvNone
      ParentBackground = False
      TabOrder = 2
      object Label1: TLabel
        Left = 6
        Top = 16
        Width = 98
        Height = 25
        Caption = 'Barcode >'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
    object btnAccept: TBitBtn
      Left = 728
      Top = 15
      Width = 111
      Height = 60
      Hint = 'The correct vial has been found and its barcode entered'
      Align = alLeft
      Caption = '&Enter'
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
      Left = 839
      Top = 15
      Width = 120
      Height = 60
      Hint = 'The vial cannot be found anywhere'
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
    end
    object btnDefer: TBitBtn
      Left = 959
      Top = 15
      Width = 104
      Height = 60
      Hint = 'Skip for now and continue to next vial'
      Align = alLeft
      Caption = '&Skip'
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
      ExplicitLeft = 1057
    end
    object btnSave: TBitBtn
      Left = 1182
      Top = 15
      Width = 96
      Height = 60
      Align = alRight
      Caption = '&Save'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000130B0000130B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333330070
        7700333333337777777733333333008088003333333377F73377333333330088
        88003333333377FFFF7733333333000000003FFFFFFF77777777000000000000
        000077777777777777770FFFFFFF0FFFFFF07F3333337F3333370FFFFFFF0FFF
        FFF07F3FF3FF7FFFFFF70F00F0080CCC9CC07F773773777777770FFFFFFFF039
        99337F3FFFF3F7F777F30F0000F0F09999937F7777373777777F0FFFFFFFF999
        99997F3FF3FFF77777770F00F000003999337F773777773777F30FFFF0FF0339
        99337F3FF7F3733777F30F08F0F0337999337F7737F73F7777330FFFF0039999
        93337FFFF7737777733300000033333333337777773333333333}
      NumGlyphs = 2
      ParentFont = False
      TabOrder = 6
      OnClick = btnSaveClick
      ExplicitLeft = 1202
      ExplicitTop = 14
    end
  end
  object Chunks: TGroupBox
    Left = 0
    Top = 0
    Width = 1280
    Height = 173
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 2
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 1212
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
      Left = 1214
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
  object panelDetails: TPanel
    Left = 0
    Top = 430
    Width = 1280
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
      DesignSize = (
        431
        114)
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
      object cbLog: TCheckBox
        Left = 361
        Top = 17
        Width = 35
        Height = 17
        Anchors = [akRight, akBottom]
        Caption = 'dbg'
        TabOrder = 0
        OnClick = cbLogClick
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
      Width = 552
      Height = 114
      Align = alClient
      Caption = 'Destination'
      ParentBackground = False
      TabOrder = 2
      object labelDestString: TLabel
        Left = 2
        Top = 99
        Width = 548
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
