object frmRetrievalAssistant: TfrmRetrievalAssistant
  Left = 0
  Top = 0
  Caption = 'Retrieval Assistant'
  ClientHeight = 504
  ClientWidth = 710
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 710
    Height = 408
    Align = alClient
    Caption = 'Jobs'
    TabOrder = 0
    ExplicitWidth = 702
    ExplicitHeight = 354
    object sgJobs: TStringGrid
      Left = 2
      Top = 15
      Width = 706
      Height = 391
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      ColCount = 8
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goColSizing, goRowSelect]
      TabOrder = 0
      OnDblClick = sgJobsDblClick
      OnDrawCell = sgJobsDrawCell
      ExplicitWidth = 698
      ExplicitHeight = 274
      ColWidths = (
        196
        64
        64
        64
        64
        64
        64
        64)
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 408
    Width = 710
    Height = 96
    Align = alBottom
    TabOrder = 1
    object groupStatusCheck: TGroupBox
      Left = 1
      Top = 1
      Width = 89
      Height = 94
      Align = alLeft
      Caption = 'Status'
      TabOrder = 0
      ExplicitLeft = 16
      ExplicitTop = 6
      ExplicitHeight = 99
      object cbNewJob: TCheckBox
        Left = 2
        Top = 15
        Width = 85
        Height = 17
        Align = alTop
        Caption = 'New Job'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = cbNewJobClick
        ExplicitWidth = 181
      end
      object cbInProgress: TCheckBox
        Left = 2
        Top = 32
        Width = 85
        Height = 17
        Align = alTop
        Caption = 'In Progress'
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = cbInProgressClick
        ExplicitWidth = 181
      end
      object cbDone: TCheckBox
        Left = 2
        Top = 49
        Width = 85
        Height = 17
        Align = alTop
        Caption = 'Done'
        TabOrder = 2
        OnClick = cbDoneClick
        ExplicitLeft = 3
        ExplicitTop = 43
        ExplicitWidth = 181
      end
      object cbDeleted: TCheckBox
        Left = 2
        Top = 66
        Width = 85
        Height = 17
        Align = alTop
        Caption = 'Deleted'
        TabOrder = 3
        OnClick = cbDeletedClick
        ExplicitWidth = 181
      end
    end
    object groupRetrievalTypeCheck: TGroupBox
      Left = 90
      Top = 1
      Width = 114
      Height = 94
      Align = alLeft
      Caption = 'Retrieval Type'
      Padding.Left = 5
      Padding.Top = 5
      Padding.Right = 5
      Padding.Bottom = 5
      TabOrder = 1
      ExplicitLeft = 207
      ExplicitTop = 6
      ExplicitHeight = 66
      object cbBox: TCheckBox
        Left = 7
        Top = 20
        Width = 100
        Height = 18
        Align = alTop
        Caption = 'Box Retrieval'
        Checked = True
        State = cbChecked
        TabOrder = 0
        OnClick = cbBoxClick
        ExplicitLeft = 1
        ExplicitTop = 15
        ExplicitWidth = 181
      end
      object cbSample: TCheckBox
        Left = 7
        Top = 38
        Width = 100
        Height = 17
        Align = alTop
        Caption = 'Sample Retrieval'
        Checked = True
        State = cbChecked
        TabOrder = 1
        OnClick = cbSampleClick
        ExplicitLeft = 2
        ExplicitTop = 33
        ExplicitWidth = 95
      end
    end
    object btnExit: TButton
      Left = 624
      Top = 40
      Width = 75
      Height = 41
      Caption = 'Exit'
      TabOrder = 2
    end
  end
end
