object frmRetrievalTreeView: TfrmRetrievalTreeView
  Left = 0
  Top = 0
  Caption = 'Retrieval Tree View for Demo'
  ClientHeight = 426
  ClientWidth = 791
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  WindowState = wsMaximized
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object tree: TTreeView
    Left = 0
    Top = 0
    Width = 678
    Height = 426
    Align = alClient
    Indent = 19
    TabOrder = 0
    ExplicitLeft = 344
    ExplicitTop = 184
    ExplicitWidth = 121
    ExplicitHeight = 97
  end
  object panelSide: TPanel
    Left = 678
    Top = 0
    Width = 113
    Height = 426
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitLeft = 662
    ExplicitTop = 13
    ExplicitHeight = 385
    object groupStatusCheck: TGroupBox
      Left = 0
      Top = 0
      Width = 113
      Height = 105
      Align = alTop
      Caption = 'Status'
      TabOrder = 0
      object cbNewJob: TCheckBox
        Left = 2
        Top = 15
        Width = 109
        Height = 17
        Align = alTop
        Caption = 'New Job'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object cbInProgress: TCheckBox
        Left = 2
        Top = 32
        Width = 109
        Height = 17
        Align = alTop
        Caption = 'In Progress'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object cbDone: TCheckBox
        Left = 2
        Top = 49
        Width = 109
        Height = 17
        Align = alTop
        Caption = 'Done'
        TabOrder = 2
      end
      object cbDeleted: TCheckBox
        Left = 2
        Top = 83
        Width = 109
        Height = 17
        Align = alTop
        Caption = 'Deleted'
        TabOrder = 3
      end
      object cbRejected: TCheckBox
        Left = 2
        Top = 66
        Width = 109
        Height = 17
        Align = alTop
        Caption = 'Rejected'
        TabOrder = 4
      end
    end
    object groupRetrievalTypeCheck: TGroupBox
      Left = 0
      Top = 105
      Width = 113
      Height = 114
      Align = alTop
      Caption = 'Retrieval Type'
      Padding.Left = 5
      Padding.Top = 5
      Padding.Right = 5
      Padding.Bottom = 5
      TabOrder = 1
      object cbBoxRetrieval: TCheckBox
        Left = 7
        Top = 20
        Width = 99
        Height = 18
        Align = alTop
        Caption = 'Box Retrieval'
        Checked = True
        State = cbChecked
        TabOrder = 0
      end
      object cbSampleRetrieval: TCheckBox
        Left = 7
        Top = 38
        Width = 99
        Height = 17
        Align = alTop
        Caption = 'Sample Retrieval'
        Checked = True
        State = cbChecked
        TabOrder = 1
      end
      object cbBoxMove: TCheckBox
        Left = 7
        Top = 55
        Width = 99
        Height = 17
        Align = alTop
        Caption = 'Box Move'
        TabOrder = 2
      end
      object cbBoxDiscard: TCheckBox
        Left = 7
        Top = 72
        Width = 99
        Height = 17
        Align = alTop
        Caption = 'Box Discard'
        TabOrder = 3
      end
      object cbSampleDiscard: TCheckBox
        Left = 7
        Top = 89
        Width = 99
        Height = 17
        Align = alTop
        Caption = 'Sample Discard'
        TabOrder = 4
      end
    end
    object cbLog: TCheckBox
      Left = 0
      Top = 384
      Width = 113
      Height = 17
      Align = alBottom
      Caption = 'Show Log'
      TabOrder = 2
      ExplicitTop = 320
    end
    object btnClose: TBitBtn
      Left = 0
      Top = 401
      Width = 113
      Height = 25
      Align = alBottom
      Kind = bkClose
      NumGlyphs = 2
      TabOrder = 3
      OnClick = btnCloseClick
      ExplicitLeft = 6
      ExplicitTop = 272
      ExplicitWidth = 75
    end
  end
  object MainMenu1: TMainMenu
    Left = 336
    Top = 64
  end
end
