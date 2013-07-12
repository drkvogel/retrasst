object frmReferredBoxesSummary: TfrmReferredBoxesSummary
  Left = 0
  Top = 0
  Caption = 'Referred Boxes - Summary'
  ClientHeight = 578
  ClientWidth = 976
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
  object Splitter1: TSplitter
    Left = 0
    Top = 337
    Width = 976
    Height = 3
    Cursor = crVSplit
    Align = alTop
    ExplicitWidth = 200
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 976
    Height = 337
    Align = alTop
    Caption = 'Boxes to sign off'
    TabOrder = 0
    object sgBoxes: TStringGrid
      Left = 2
      Top = 15
      Width = 972
      Height = 320
      Align = alClient
      ColCount = 11
      DefaultDrawing = False
      FixedCols = 0
      TabOrder = 0
      OnDrawCell = sgBoxesDrawCell
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 537
    Width = 976
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    object btnConfirm: TButton
      Left = 391
      Top = 4
      Width = 75
      Height = 25
      Caption = 'Confirm'
      TabOrder = 0
      OnClick = btnConfirmClick
    end
    object btnCancel: TButton
      Left = 511
      Top = 4
      Width = 75
      Height = 25
      Caption = 'Dismiss'
      TabOrder = 1
      OnClick = btnCancelClick
    end
    object cbDummy: TCheckBox
      Left = 264
      Top = 8
      Width = 97
      Height = 17
      Caption = 'Dummy run'
      Checked = True
      State = cbChecked
      TabOrder = 2
      OnClick = cbDummyClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 340
    Width = 976
    Height = 197
    Align = alClient
    Caption = 'Log'
    TabOrder = 2
    object memoLog: TMemo
      Left = 2
      Top = 15
      Width = 972
      Height = 180
      Align = alClient
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
end
