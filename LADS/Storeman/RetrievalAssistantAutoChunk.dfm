object frmAutoChunk: TfrmAutoChunk
  Left = 0
  Top = 0
  Caption = 'Auto-Chunk'
  ClientHeight = 268
  ClientWidth = 421
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 421
    Height = 268
    Align = alClient
    Caption = 'Auto chunk'
    TabOrder = 0
    ExplicitLeft = -8
    ExplicitTop = 16
    object Label1: TLabel
      Left = 32
      Top = 83
      Width = 78
      Height = 13
      Caption = 'Max section size'
    end
    object Label2: TLabel
      Left = 32
      Top = 112
      Width = 81
      Height = 13
      Caption = 'Next section size'
    end
    object Label3: TLabel
      Left = 32
      Top = 48
      Width = 96
      Height = 13
      Caption = 'Destination box size'
    end
    object editMaxSize: TEdit
      Left = 146
      Top = 80
      Width = 103
      Height = 21
      TabOrder = 0
      Text = '500'
      OnChange = editMaxSizeChange
    end
    object btnOK: TButton
      Left = 146
      Top = 224
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 1
      OnClick = btnOKClick
    end
    object comboSectionSize: TComboBox
      Left = 146
      Top = 107
      Width = 121
      Height = 21
      Style = csDropDownList
      TabOrder = 2
    end
    object btnAddChunk: TButton
      Left = 146
      Top = 134
      Width = 121
      Height = 25
      Caption = 'Add 1 Chunk'
      TabOrder = 3
      OnClick = btnAddChunkClick
    end
    object editDestBoxSize: TEdit
      Left = 146
      Top = 45
      Width = 103
      Height = 21
      TabOrder = 4
      Text = '72'
    end
    object btnAddAllChunks: TButton
      Left = 146
      Top = 165
      Width = 121
      Height = 25
      Caption = 'Add All Chunks'
      TabOrder = 5
      OnClick = btnAddAllChunksClick
    end
    object updownBoxSize: TUpDown
      Left = 251
      Top = 45
      Width = 16
      Height = 21
      Associate = editDestBoxSize
      Position = 72
      TabOrder = 6
    end
    object updownMaxSection: TUpDown
      Left = 251
      Top = 80
      Width = 16
      Height = 21
      Associate = editMaxSize
      Max = 5000
      Position = 500
      TabOrder = 7
    end
  end
  object timerCalculate: TTimer
    Enabled = False
    Interval = 200
    OnTimer = timerCalculateTimer
    Left = 280
    Top = 65520
  end
end
