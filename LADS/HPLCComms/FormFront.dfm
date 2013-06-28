object frmFront: TfrmFront
  Left = 229
  Top = 125
  Caption = 'HPLC Communicator'
  ClientHeight = 764
  ClientWidth = 1218
  Color = clBtnFace
  Constraints.MinHeight = 100
  Constraints.MinWidth = 711
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 1218
    Height = 41
    Align = alTop
    Color = 16761056
    TabOrder = 1
    DesignSize = (
      1218
      41)
    object btnCreateSampleSelection: TButton
      Left = 90
      Top = 10
      Width = 146
      Height = 25
      Caption = '&Create sample selection...'
      TabOrder = 0
      OnClick = btnCreateSampleSelectionClick
    end
    object BitBtnExit: TBitBtn
      Left = 1046
      Top = 10
      Width = 165
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'E&xit HPLC Communicator'
      TabOrder = 1
      OnClick = BitBtnExitClick
      Glyph.Data = {
        76010000424D7601000000000000760000002800000020000000100000000100
        04000000000000010000120B0000120B00001000000000000000000000000000
        800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
        FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00330000000000
        03333377777777777F333301BBBBBBBB033333773F3333337F3333011BBBBBBB
        0333337F73F333337F33330111BBBBBB0333337F373F33337F333301110BBBBB
        0333337F337F33337F333301110BBBBB0333337F337F33337F333301110BBBBB
        0333337F337F33337F333301110BBBBB0333337F337F33337F333301110BBBBB
        0333337F337F33337F333301110BBBBB0333337F337FF3337F33330111B0BBBB
        0333337F337733337F333301110BBBBB0333337F337F33337F333301110BBBBB
        0333337F3F7F33337F333301E10BBBBB0333337F7F7F33337F333301EE0BBBBB
        0333337F777FFFFF7F3333000000000003333377777777777333}
      NumGlyphs = 2
    end
    object btnEnterResults: TButton
      Left = 240
      Top = 10
      Width = 106
      Height = 25
      Caption = '&Enter results...'
      TabOrder = 2
      OnClick = btnEnterResultsClick
    end
    object btnCopyResults: TButton
      Left = 385
      Top = 10
      Width = 141
      Height = 25
      Caption = 'Copy results to clipboard'
      TabOrder = 3
      OnClick = btnCopyResultsClick
    end
  end
  object btnGetWorklist: TButton
    Left = 10
    Top = 10
    Width = 75
    Height = 25
    Caption = '&Get worklist'
    TabOrder = 0
    OnClick = btnGetWorklistClick
  end
  object ListBox1: TListBox
    Left = 0
    Top = 41
    Width = 1218
    Height = 704
    Align = alClient
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'Courier New'
    Font.Style = []
    ItemHeight = 14
    MultiSelect = True
    ParentFont = False
    TabOrder = 2
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 745
    Width = 1218
    Height = 19
    Panels = <>
    SimplePanel = True
  end
end
