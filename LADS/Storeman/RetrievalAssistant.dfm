object frmRetrievalAssistant: TfrmRetrievalAssistant
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMaximize]
  Caption = 'Retrieval Assistant'
  ClientHeight = 504
  ClientWidth = 771
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  WindowState = wsMaximized
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 771
    Height = 504
    Align = alClient
    Caption = 'Jobs'
    TabOrder = 0
    object sgJobs: TStringGrid
      Left = 2
      Top = 15
      Width = 654
      Height = 388
      Margins.Bottom = 10
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      ColCount = 7
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goColSizing, goRowSelect]
      TabOrder = 0
      OnClick = sgJobsClick
      OnDblClick = sgJobsDblClick
      OnDrawCell = sgJobsDrawCell
      ExplicitHeight = 409
      ColWidths = (
        196
        64
        64
        64
        64
        64
        64)
    end
    object Panel2: TPanel
      Left = 656
      Top = 15
      Width = 113
      Height = 388
      Align = alRight
      BevelOuter = bvNone
      TabOrder = 1
      ExplicitHeight = 409
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
          OnClick = cbNewJobClick
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
          OnClick = cbInProgressClick
        end
        object cbDone: TCheckBox
          Left = 2
          Top = 49
          Width = 109
          Height = 17
          Align = alTop
          Caption = 'Done'
          TabOrder = 2
          OnClick = cbDoneClick
        end
        object cbDeleted: TCheckBox
          Left = 2
          Top = 66
          Width = 109
          Height = 17
          Align = alTop
          Caption = 'Deleted'
          TabOrder = 3
          OnClick = cbDeletedClick
        end
        object cbRejected: TCheckBox
          Left = 2
          Top = 83
          Width = 109
          Height = 17
          Align = alTop
          Caption = 'Rejected'
          TabOrder = 4
          OnClick = cbRejectedClick
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
          OnClick = cbBoxRetrievalClick
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
          OnClick = cbSampleRetrievalClick
        end
        object cbBoxMove: TCheckBox
          Left = 7
          Top = 55
          Width = 99
          Height = 17
          Align = alTop
          Caption = 'Box Move'
          TabOrder = 2
          OnClick = cbBoxMoveClick
        end
        object cbBoxDiscard: TCheckBox
          Left = 7
          Top = 72
          Width = 99
          Height = 17
          Align = alTop
          Caption = 'Box Discard'
          TabOrder = 3
          OnClick = cbBoxDiscardClick
        end
        object cbSampleDiscard: TCheckBox
          Left = 7
          Top = 89
          Width = 99
          Height = 17
          Align = alTop
          Caption = 'Sample Discard'
          TabOrder = 4
          OnClick = cbSampleDiscardClick
        end
      end
      object btnExit: TButton
        Left = 0
        Top = 347
        Width = 113
        Height = 41
        Align = alBottom
        Caption = 'Exit'
        TabOrder = 2
        OnClick = btnExitClick
        ExplicitTop = 368
      end
      object cbLog: TCheckBox
        Left = 0
        Top = 330
        Width = 113
        Height = 17
        Align = alBottom
        Caption = 'Show Log'
        TabOrder = 3
        OnClick = cbLogClick
        ExplicitTop = 351
      end
    end
    object panelDebug: TPanel
      Left = 2
      Top = 403
      Width = 767
      Height = 99
      Align = alBottom
      Caption = 'panelDebug'
      TabOrder = 2
      ExplicitLeft = 1
      ExplicitTop = 409
      object memoDebug: TMemo
        Left = 1
        Top = 1
        Width = 616
        Height = 97
        Align = alLeft
        ScrollBars = ssVertical
        TabOrder = 0
        Visible = False
      end
      object btnResetJobs: TButton
        Left = 660
        Top = 13
        Width = 75
        Height = 25
        Caption = 'Reset Jobs'
        TabOrder = 1
        OnClick = btnResetJobsClick
      end
    end
  end
  object Timer1: TTimer
    Left = 248
    Top = 88
  end
end
