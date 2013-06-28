object formMain: TformMain
  Left = 0
  Top = 0
  Caption = 'Generic Comms'
  ClientHeight = 506
  ClientWidth = 889
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 889
    Height = 43
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 1
    object GroupBox1: TGroupBox
      Left = 0
      Top = 0
      Width = 313
      Height = 43
      Align = alLeft
      Caption = 'Project'
      TabOrder = 0
      object comboProjects: TComboBox
        Left = 100
        Top = 15
        Width = 211
        Height = 21
        Align = alClient
        Style = csDropDownList
        ItemHeight = 13
        TabOrder = 0
        OnChange = comboProjectsChange
      end
      object btnGetWorklist: TButton
        Left = 2
        Top = 15
        Width = 98
        Height = 26
        Align = alLeft
        Caption = '&Get worklist'
        TabOrder = 1
        OnClick = btnGetWorklistClick
      end
    end
    object GroupBox2: TGroupBox
      Left = 313
      Top = 0
      Width = 185
      Height = 43
      Align = alLeft
      Caption = 'Input to analyser'
      Color = 16770290
      ParentBackground = False
      ParentColor = False
      TabOrder = 1
      object btnCreateSampleSelection: TButton
        Left = 2
        Top = 15
        Width = 181
        Height = 25
        Align = alTop
        Caption = '&Create sample selection...'
        TabOrder = 0
        OnClick = btnCreateSampleSelectionClick
      end
    end
    object GroupBox3: TGroupBox
      Left = 498
      Top = 0
      Width = 185
      Height = 43
      Align = alLeft
      Caption = 'Output from analyser'
      Color = 10999254
      ParentBackground = False
      ParentColor = False
      TabOrder = 2
      object btnEnterResults: TButton
        Left = 2
        Top = 15
        Width = 181
        Height = 25
        Align = alTop
        Caption = '&Enter results...'
        TabOrder = 0
        OnClick = btnEnterResultsClick
      end
    end
    object groupboxManage: TGroupBox
      Left = 771
      Top = 0
      Width = 118
      Height = 43
      Align = alRight
      Caption = 'Manage'
      TabOrder = 3
      object btnManage: TButton
        Left = 2
        Top = 15
        Width = 114
        Height = 25
        Align = alTop
        Caption = 'File formats...'
        TabOrder = 0
        OnClick = btnManageClick
      end
    end
  end
  object statusBar: TStatusBar
    Left = 0
    Top = 472
    Width = 889
    Height = 34
    Panels = <>
    SimplePanel = True
    SimpleText = 'Ready.'
  end
  object listboxWorklist: TListBox
    Left = 0
    Top = 69
    Width = 889
    Height = 403
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -12
    Font.Name = 'Courier New'
    Font.Style = []
    ItemHeight = 15
    MultiSelect = True
    ParentFont = False
    TabOrder = 0
  end
  object btnPopulateWorklist: TButton
    Left = 644
    Top = 481
    Width = 121
    Height = 25
    Caption = 'Populate worklist'
    TabOrder = 2
    Visible = False
    OnClick = btnPopulateWorklistClick
  end
  object sgWLColumnHeaders: TStringGrid
    Left = 0
    Top = 43
    Width = 889
    Height = 26
    Align = alTop
    ColCount = 16
    FixedCols = 0
    RowCount = 2
    ScrollBars = ssNone
    TabOrder = 4
    ColWidths = (
      174
      156
      125
      82
      79
      163
      64
      64
      64
      64
      64
      64
      64
      64
      64
      64)
  end
  object BitBtn1: TBitBtn
    Left = 771
    Top = 481
    Width = 103
    Height = 25
    Caption = 'Exit'
    TabOrder = 5
    OnClick = BitBtn1Click
  end
  object Database1: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'cp_gcomms'
    LoginPrompt = False
    SessionName = 'Default'
    Left = 192
    Top = 124
  end
  object qCentral: TQuery
    DatabaseName = 'centralData'
    Left = 120
    Top = 124
  end
  object qLocal: TQuery
    DatabaseName = 'localData'
    Left = 160
    Top = 124
  end
end
