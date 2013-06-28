object CreateProjectDatabase: TCreateProjectDatabase
  Left = 205
  Top = 124
  AutoScroll = False
  BorderIcons = []
  Caption = 'Create project database'
  ClientHeight = 493
  ClientWidth = 602
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -13
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  Scaled = False
  OnActivate = FormActivate
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 16
  object ReplyMemo: TMemo
    Left = 48
    Top = 32
    Width = 505
    Height = 385
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = []
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 452
    Width = 602
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btnClose: TButton
      Left = 24
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'MS Sans Serif'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object RexecSocket: TClientSocket
    Active = False
    ClientType = ctNonBlocking
    Port = 0
    OnConnect = RexecSocketConnect
    OnDisconnect = RexecSocketDisconnect
    OnRead = RexecSocketRead
    Left = 16
    Top = 160
  end
end
