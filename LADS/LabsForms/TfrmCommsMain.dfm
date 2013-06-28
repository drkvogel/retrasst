object mainForm: TmainForm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  Caption = 'Loading...'
  ClientHeight = 390
  ClientWidth = 300
  Color = 12316364
  Constraints.MinHeight = 200
  Constraints.MinWidth = 180
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = True
  OnClose = FormClose
  OnCreate = FormCreate
  DesignSize = (
    300
    390)
  PixelsPerInch = 96
  TextHeight = 13
  object buddyDetails: TLabel
    Left = 102
    Top = 14
    Width = 87
    Height = 13
    Caption = 'Analyser/cluster(s)'
  end
  object version: TLabel
    Left = 8
    Top = 14
    Width = 58
    Height = 13
    Caption = 'Comms v2.0'
  end
  object lblCategory: TLabel
    Left = 8
    Top = 40
    Width = 82
    Height = 13
    Caption = 'Sample category:'
  end
  object history: TMemo
    Left = 8
    Top = 72
    Width = 284
    Height = 290
    Anchors = [akLeft, akTop, akRight, akBottom]
    ReadOnly = True
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object statusBar: TStatusBar
    Left = 0
    Top = 371
    Width = 300
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = 'Reading configuration from database'
  end
  object cbCategory: TComboBox
    Left = 102
    Top = 37
    Width = 189
    Height = 21
    AutoDropDown = True
    AutoCloseUp = True
    Anchors = [akLeft, akTop, akRight]
    ItemHeight = 13
    Sorted = True
    TabOrder = 0
    Text = '(All)'
    OnDropDown = cbCategoryDropDown
  end
  object tdbCentral: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'central'
    LoginPrompt = False
    SessionName = 'Default'
    TransIsolation = tiDirtyRead
    Left = 24
    Top = 112
  end
  object qCentral: TQuery
    DatabaseName = 'central'
    UniDirectional = True
    Left = 56
    Top = 112
  end
  object tdbCluster: TDatabase
    AliasName = 'alias_local_buddy'
    DatabaseName = 'cluster'
    LoginPrompt = False
    SessionName = 'Default'
    TransIsolation = tiDirtyRead
    Left = 136
    Top = 112
  end
  object qCluster: TQuery
    DatabaseName = 'cluster'
    UniDirectional = True
    Left = 168
    Top = 112
  end
  object tdbProject: TDatabase
    AliasName = 'dsn_lab_ingres'
    DatabaseName = 'project'
    LoginPrompt = False
    SessionName = 'Default'
    TransIsolation = tiDirtyRead
    Left = 24
    Top = 168
  end
  object qProject: TQuery
    DatabaseName = 'project'
    UniDirectional = True
    Left = 56
    Top = 168
  end
  object MainMenu1: TMainMenu
    Left = 168
    Top = 168
    object File1: TMenuItem
      Caption = '&File'
      object Open1: TMenuItem
        Caption = '&Open...'
        OnClick = Open1Click
      end
      object Configuration1: TMenuItem
        Caption = '&Configure...'
        OnClick = Configuration1Click
      end
      object N2: TMenuItem
        Caption = '-'
      end
      object Reset1: TMenuItem
        Caption = '&Reset...'
        OnClick = Reset1Click
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
    object Help1: TMenuItem
      Caption = '&Help'
      object longAudit: TMenuItem
        Caption = '&Long Audit'
        OnClick = longAuditClick
      end
      object N1: TMenuItem
        Caption = '-'
      end
      object About1: TMenuItem
        Caption = '&About...'
        OnClick = About1Click
      end
    end
  end
  object timer: TTimer
    Enabled = False
    OnTimer = timerTimer
    Left = 136
    Top = 168
  end
  object openDialog: TOpenDialog
    DefaultExt = 'xml'
    Filter = 'Text files|*.txt|XML files|*.txt;*.xml|All files|*.*'
    Left = 208
    Top = 168
  end
end
