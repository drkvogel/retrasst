object frmProcess: TfrmProcess
  Left = 0
  Top = 0
  Caption = 'Process Retrieval Job'
  ClientHeight = 482
  ClientWidth = 956
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
  object List: TGroupBox
    Left = 0
    Top = 105
    Width = 956
    Height = 250
    Align = alClient
    Caption = 'Retrieval List'
    TabOrder = 0
    object sgVials: TStringGrid
      Left = 2
      Top = 15
      Width = 952
      Height = 147
      Align = alClient
      FixedCols = 0
      TabOrder = 0
    end
    object GroupBox3: TGroupBox
      Left = 2
      Top = 162
      Width = 952
      Height = 86
      Align = alBottom
      Caption = 'Details'
      TabOrder = 1
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
    object panelLoading: TPanel
      Left = 328
      Top = 64
      Width = 273
      Height = 127
      Caption = 'Loading retrieval list, please wait...'
      TabOrder = 2
      Visible = False
      object progressBottom: TProgressBar
        Left = 1
        Top = 109
        Width = 271
        Height = 17
        Align = alBottom
        TabOrder = 0
      end
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 355
    Width = 956
    Height = 77
    Align = alBottom
    Caption = 'Sign Off'
    TabOrder = 1
    DesignSize = (
      956
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
      Left = 804
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Accept'
      TabOrder = 0
      OnClick = btnAcceptClick
    end
    object btnSkip: TButton
      Left = 879
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Skip'
      TabOrder = 1
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
      Left = 722
      Top = 46
      Width = 64
      Height = 17
      Anchors = [akRight, akBottom]
      Caption = 'Show Log'
      TabOrder = 3
      OnClick = cbLogClick
    end
  end
  object Chunks: TGroupBox
    Left = 0
    Top = 0
    Width = 956
    Height = 105
    Align = alTop
    Caption = 'Chunks'
    TabOrder = 2
    object sgChunks: TStringGrid
      Left = 2
      Top = 15
      Width = 952
      Height = 88
      Align = alClient
      FixedCols = 0
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
  object memoDebug: TMemo
    Left = 0
    Top = 432
    Width = 956
    Height = 50
    Align = alBottom
    TabOrder = 3
    Visible = False
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
