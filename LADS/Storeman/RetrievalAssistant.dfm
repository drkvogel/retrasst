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
  DesignSize = (
    710
    504)
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 702
    Height = 354
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Jobs'
    TabOrder = 0
    object sgJobs: TStringGrid
      Left = 2
      Top = 15
      Width = 698
      Height = 337
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      ColCount = 8
      DefaultDrawing = False
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goColSizing, goRowSelect]
      TabOrder = 0
      OnDrawCell = sgJobsDrawCell
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
  object groupStatusCheck: TGroupBox
    Left = 40
    Top = 392
    Width = 185
    Height = 89
    Anchors = [akLeft, akBottom]
    Caption = 'Status'
    TabOrder = 1
    object cbNewJob: TCheckBox
      Left = 2
      Top = 15
      Width = 181
      Height = 17
      Align = alTop
      Caption = 'New Job'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = cbNewJobClick
      ExplicitLeft = 64
      ExplicitTop = 24
      ExplicitWidth = 97
    end
    object cbInProgress: TCheckBox
      Left = 2
      Top = 32
      Width = 181
      Height = 17
      Align = alTop
      Caption = 'In Progress'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = cbInProgressClick
      ExplicitLeft = 40
      ExplicitTop = 56
      ExplicitWidth = 97
    end
    object cbDone: TCheckBox
      Left = 2
      Top = 49
      Width = 181
      Height = 17
      Align = alTop
      Caption = 'Done'
      TabOrder = 2
      OnClick = cbDoneClick
      ExplicitLeft = 88
      ExplicitTop = 80
      ExplicitWidth = 97
    end
    object cbDeleted: TCheckBox
      Left = 2
      Top = 66
      Width = 181
      Height = 17
      Align = alTop
      Caption = 'Deleted'
      TabOrder = 3
      OnClick = cbDeletedClick
      ExplicitLeft = 32
      ExplicitTop = 88
      ExplicitWidth = 97
    end
  end
  object groupRetrievalTypeCheck: TGroupBox
    Left = 360
    Top = 392
    Width = 185
    Height = 66
    Anchors = [akLeft, akBottom]
    Caption = 'Retrieval Type'
    TabOrder = 2
    object cbBox: TCheckBox
      Left = 2
      Top = 15
      Width = 181
      Height = 18
      Align = alTop
      Caption = 'Box Retrieval'
      Checked = True
      State = cbChecked
      TabOrder = 0
      OnClick = cbBoxClick
    end
    object cbSample: TCheckBox
      Left = 2
      Top = 33
      Width = 181
      Height = 17
      Align = alTop
      Caption = 'Sample Retrieval'
      Checked = True
      State = cbChecked
      TabOrder = 1
      OnClick = cbSampleClick
      ExplicitLeft = 32
      ExplicitTop = 56
      ExplicitWidth = 97
    end
  end
end
