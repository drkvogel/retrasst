object frmSelectJob: TfrmSelectJob
  Left = 0
  Top = 0
  BorderIcons = [biMinimize]
  Caption = 'Select Cryogenics Task'
  ClientHeight = 291
  ClientWidth = 583
  Color = 12316364
  Constraints.MinHeight = 150
  Constraints.MinWidth = 300
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  DesignSize = (
    583
    291)
  PixelsPerInch = 96
  TextHeight = 13
  object grdJobs: TStringGrid
    Left = 6
    Top = 8
    Width = 570
    Height = 234
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 3
    DefaultColWidth = 160
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    TabOrder = 0
    OnDblClick = grdJobsDblClick
  end
  object btnNewJob: TButton
    Left = 220
    Top = 255
    Width = 82
    Height = 28
    Anchors = [akLeft, akBottom]
    Caption = 'New Job'
    TabOrder = 2
    OnClick = btnNewJobClick
  end
  object btnDone: TButton
    Left = 384
    Top = 255
    Width = 82
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'Close'
    ModalResult = 2
    TabOrder = 3
  end
  object btnOpenJob: TButton
    Left = 118
    Top = 255
    Width = 82
    Height = 28
    Anchors = [akLeft, akBottom]
    Caption = 'Open Job'
    TabOrder = 1
    OnClick = btnOpenJobClick
  end
end
