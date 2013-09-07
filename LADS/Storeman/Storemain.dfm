object frmStoremain: TfrmStoremain
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Main Menu'
  ClientHeight = 327
  ClientWidth = 527
  Color = 12316364
  Constraints.MaxHeight = 450
  Constraints.MaxWidth = 600
  Constraints.MinHeight = 260
  Constraints.MinWidth = 420
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 176
    Height = 264
    Align = alLeft
    Caption = 'Panel1'
    ShowCaption = False
    TabOrder = 0
    DesignSize = (
      176
      264)
    object Label1: TLabel
      Left = 46
      Top = 21
      Width = 84
      Height = 32
      Alignment = taCenter
      Anchors = []
      Caption = 'Inventory Management'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
      ExplicitTop = 20
    end
    object BtnConfigure: TButton
      Left = 30
      Top = 162
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Configure Storage'
      TabOrder = 2
      WordWrap = True
      OnClick = ConfigureClick
    end
    object BtnBrowse: TButton
      Left = 30
      Top = 68
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Browse Samples'
      TabOrder = 0
      WordWrap = True
      OnClick = TreeBrowse
    end
    object btnMove: TButton
      Left = 30
      Top = 115
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Move Boxes'
      TabOrder = 1
      WordWrap = True
      OnClick = btnMoveClick
    end
    object BtnRationalyse: TButton
      Left = 30
      Top = 210
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Rationalise'
      TabOrder = 3
      WordWrap = True
    end
  end
  object Panel2: TPanel
    Left = 176
    Top = 0
    Width = 175
    Height = 264
    Align = alClient
    Caption = 'Panel2'
    ShowCaption = False
    TabOrder = 1
    DesignSize = (
      175
      264)
    object Label2: TLabel
      Left = 45
      Top = 21
      Width = 84
      Height = 32
      Alignment = taCenter
      Anchors = []
      Caption = 'Sample Management'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
      ExplicitTop = 20
    end
    object BtnXfer: TButton
      Left = 30
      Top = 68
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Boxes for Transfer'
      TabOrder = 0
      WordWrap = True
      OnClick = BtnXferClick
    end
    object BtnDiscard: TButton
      Left = 30
      Top = 115
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Discard Samples'
      TabOrder = 1
      WordWrap = True
      OnClick = BtnDiscardClick
    end
    object BtnAnalyse: TButton
      Left = 30
      Top = 162
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Analyse Samples'
      TabOrder = 2
      WordWrap = True
      OnClick = BtnAnalyseClick
    end
    object BtnReferred: TButton
      Left = 30
      Top = 210
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Referred Boxes'
      TabOrder = 3
      WordWrap = True
      OnClick = BtnReferredClick
    end
  end
  object Panel3: TPanel
    Left = 351
    Top = 0
    Width = 176
    Height = 264
    Align = alRight
    Caption = 'Panel3'
    ShowCaption = False
    TabOrder = 2
    DesignSize = (
      176
      264)
    object Label3: TLabel
      Left = 58
      Top = 21
      Width = 59
      Height = 32
      Alignment = taCenter
      Anchors = []
      Caption = 'Sample Retrieval'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
      WordWrap = True
      ExplicitTop = 20
    end
    object BtnCreate: TButton
      Left = 30
      Top = 115
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Create List'
      TabOrder = 1
      WordWrap = True
      OnClick = OnCreateList
    end
    object BtnPrevious: TButton
      Left = 30
      Top = 68
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'View Lists'
      TabOrder = 0
      WordWrap = True
    end
    object BtnRetrieve: TButton
      Left = 30
      Top = 162
      Width = 116
      Height = 35
      Anchors = []
      Caption = 'Retrieval Assistant'
      TabOrder = 2
      WordWrap = True
      OnClick = BtnRetrieveClick
    end
  end
  object Panel4: TPanel
    Left = 0
    Top = 264
    Width = 527
    Height = 63
    Align = alBottom
    Caption = 'Panel4'
    ShowCaption = False
    TabOrder = 3
    DesignSize = (
      527
      63)
    object lbBoxesReady: TLabel
      Left = 90
      Top = 13
      Width = 6
      Height = 14
      Anchors = []
      Caption = '?'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbForRemoval: TLabel
      Left = 90
      Top = 35
      Width = 6
      Height = 14
      Anchors = []
      Caption = '?'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbRemoved: TLabel
      Left = 310
      Top = 13
      Width = 6
      Height = 14
      Anchors = []
      Caption = '?'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object lbRetrievalLists: TLabel
      Left = 310
      Top = 35
      Width = 6
      Height = 14
      Anchors = []
      Caption = '?'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label4: TLabel
      Left = 16
      Top = 12
      Width = 41
      Height = 14
      Caption = 'Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -12
      Font.Name = 'Tahoma'
      Font.Style = [fsBold]
      ParentFont = False
    end
  end
  object timer: TTimer
    Enabled = False
    OnTimer = timerTimer
    Left = 456
    Top = 280
  end
end
