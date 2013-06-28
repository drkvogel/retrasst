object EditCentralDb: TEditCentralDb
  Left = 196
  Top = 112
  Width = 431
  Height = 376
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Edit Central Database'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 12
    Top = 8
    Width = 400
    Height = 292
    TabOrder = 0
    object ProjectInfo: TButton
      Left = 30
      Top = 19
      Width = 150
      Height = 36
      Caption = 'Project Information'
      TabOrder = 0
      OnClick = ProjectInfoClick
    end
    object Tests: TButton
      Left = 30
      Top = 126
      Width = 150
      Height = 36
      Caption = 'Test Definitions'
      TabOrder = 2
      OnClick = TestsClick
    end
    object Machines: TButton
      Left = 30
      Top = 179
      Width = 150
      Height = 36
      Caption = 'Machine Definitions'
      TabOrder = 3
      OnClick = MachinesClick
    end
    object Operators: TButton
      Left = 30
      Top = 72
      Width = 150
      Height = 36
      Caption = 'Operators'
      TabOrder = 1
      OnClick = OperatorsClick
    end
    object btnBoxSize: TButton
      Left = 219
      Top = 19
      Width = 150
      Height = 36
      Caption = 'Box Sizes'
      TabOrder = 5
      OnClick = btnBoxSizeClick
    end
    object btnQC: TButton
      Left = 30
      Top = 232
      Width = 150
      Height = 36
      Caption = 'Quality Control'
      TabOrder = 4
      OnClick = btnQCClick
    end
    object btnRackSize: TButton
      Left = 219
      Top = 72
      Width = 150
      Height = 36
      Caption = 'Rack Sizes'
      TabOrder = 6
      OnClick = btnRackSizeClick
    end
    object lbObjects: TListBox
      Left = 214
      Top = 122
      Width = 160
      Height = 156
      ItemHeight = 13
      TabOrder = 7
      OnClick = lbObjectsClick
    end
  end
  object Panel2: TPanel
    Left = 0
    Top = 308
    Width = 423
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btnClose: TButton
      Left = 40
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
