object frmRetrievalProcess: TfrmRetrievalProcess
  Left = 0
  Top = 0
  Caption = 'Process Retrieval Job'
  ClientHeight = 477
  ClientWidth = 628
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 628
    Height = 400
    Align = alClient
    Caption = 'GroupBox1'
    TabOrder = 0
    ExplicitHeight = 273
    object StringGrid1: TStringGrid
      Left = 2
      Top = 15
      Width = 624
      Height = 297
      Align = alClient
      FixedCols = 0
      TabOrder = 0
      ExplicitLeft = 296
      ExplicitTop = 88
      ExplicitWidth = 320
      ExplicitHeight = 120
    end
    object GroupBox3: TGroupBox
      Left = 2
      Top = 312
      Width = 624
      Height = 86
      Align = alBottom
      Caption = 'Details'
      TabOrder = 1
      object Label3: TLabel
        Left = 70
        Top = 52
        Width = 105
        Height = 25
        Caption = 'Sample ID:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label2: TLabel
        Left = 12
        Top = 21
        Width = 163
        Height = 25
        Caption = 'Storage Location:'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -21
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
    end
  end
  object GroupBox2: TGroupBox
    Left = 0
    Top = 400
    Width = 628
    Height = 77
    Align = alBottom
    Caption = 'Sign Off'
    TabOrder = 1
    ExplicitTop = 404
    object Label1: TLabel
      Left = 14
      Top = 32
      Width = 139
      Height = 25
      Caption = 'Enter barcode:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -21
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object btnAccept: TButton
      Left = 476
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Accept'
      TabOrder = 0
      ExplicitLeft = 304
      ExplicitTop = 40
      ExplicitHeight = 25
    end
    object btnSkip: TButton
      Left = 551
      Top = 15
      Width = 75
      Height = 60
      Align = alRight
      Caption = 'Skip'
      TabOrder = 1
      ExplicitLeft = 504
      ExplicitTop = 48
      ExplicitHeight = 25
    end
    object editBarcode: TEdit
      Left = 192
      Top = 16
      Width = 278
      Height = 53
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -37
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
    end
  end
  object MainMenu1: TMainMenu
    Left = 512
    Top = 65520
    object Exit1: TMenuItem
      Caption = 'E&xit'
      OnClick = Exit1Click
    end
  end
end
