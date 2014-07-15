object frmSelectJob: TfrmSelectJob
  Left = 0
  Top = 0
  BorderIcons = [biMinimize]
  Caption = 'Select Cryogenics Task'
  ClientHeight = 432
  ClientWidth = 715
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
    715
    432)
  PixelsPerInch = 96
  TextHeight = 13
  object LblWhy: TLabel
    Left = 330
    Top = 18
    Width = 44
    Height = 13
    Caption = 'Exercise:'
  end
  object Label1: TLabel
    Left = 10
    Top = 18
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object grdJobs: TStringGrid
    Left = 6
    Top = 55
    Width = 702
    Height = 323
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 3
    DefaultColWidth = 160
    FixedCols = 0
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    TabOrder = 3
    OnDblClick = grdJobsDblClick
  end
  object btnNewJob: TButton
    Left = 220
    Top = 392
    Width = 82
    Height = 28
    Anchors = [akLeft, akBottom]
    Caption = 'New Job'
    TabOrder = 5
    OnClick = btnNewJobClick
  end
  object btnDone: TButton
    Left = 516
    Top = 392
    Width = 82
    Height = 28
    Anchors = [akRight, akBottom]
    Caption = 'Close'
    ModalResult = 2
    TabOrder = 6
  end
  object btnOpenJob: TButton
    Left = 118
    Top = 392
    Width = 82
    Height = 28
    Anchors = [akLeft, akBottom]
    Caption = 'Open Job'
    TabOrder = 4
    OnClick = btnOpenJobClick
  end
  object cbExercise: TComboBox
    Left = 388
    Top = 14
    Width = 210
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 12
    TabOrder = 1
    Text = '(none)'
    OnChange = cbExerciseChange
    OnDropDown = cbExerciseDropDown
  end
  object btnNewEx: TButton
    Left = 614
    Top = 8
    Width = 90
    Height = 32
    Anchors = [akTop, akRight]
    Caption = 'New Exercise'
    TabOrder = 2
    OnClick = btnNewExClick
  end
  object cbProjects: TComboBox
    Left = 63
    Top = 14
    Width = 210
    Height = 21
    DropDownCount = 12
    Sorted = True
    TabOrder = 0
    Text = '(any)'
    OnChange = cbProjectsChange
    OnDropDown = cbProjectsDropDown
  end
end
