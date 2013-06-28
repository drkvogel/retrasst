object testForm: TtestForm
  Left = 177
  Top = 147
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Dummy Analyser'
  ClientHeight = 390
  ClientWidth = 342
  Color = 16761056
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnHide = FormHide
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object display: TMemo
    Left = 0
    Top = 0
    Width = 342
    Height = 390
    Align = alClient
    ScrollBars = ssVertical
    TabOrder = 0
    ExplicitHeight = 397
  end
  object timer: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerTimer
    Left = 88
    Top = 88
  end
  object qCluster: TQuery
    DatabaseName = 'localData'
    Left = 128
    Top = 128
  end
  object qCentral: TQuery
    DatabaseName = 'centralData'
    Left = 88
    Top = 128
  end
  object qProject: TQuery
    DatabaseName = 'projectData'
    Left = 168
    Top = 128
  end
end
