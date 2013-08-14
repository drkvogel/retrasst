object frmAutoChunk: TfrmAutoChunk
  Left = 0
  Top = 0
  Caption = 'Auto-Chunk'
  ClientHeight = 181
  ClientWidth = 390
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
    Width = 390
    Height = 181
    Align = alClient
    Caption = 'Auto chunk'
    TabOrder = 0
    ExplicitLeft = 32
    ExplicitTop = -8
    ExplicitWidth = 418
    ExplicitHeight = 282
    object Label1: TLabel
      Left = 24
      Top = 35
      Width = 78
      Height = 13
      Caption = 'Max section size'
    end
    object Label2: TLabel
      Left = 24
      Top = 64
      Width = 81
      Height = 13
      Caption = 'Next section size'
    end
    object editMaxSize: TEdit
      Left = 138
      Top = 32
      Width = 121
      Height = 21
      TabOrder = 0
      Text = '500'
      OnChange = editMaxSizeChange
    end
    object btnOK: TButton
      Left = 138
      Top = 128
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 1
      OnClick = btnOKClick
    end
    object comboSectionSize: TComboBox
      Left = 138
      Top = 59
      Width = 121
      Height = 21
      Style = csDropDownList
      TabOrder = 2
    end
    object btnAddChunk: TButton
      Left = 138
      Top = 86
      Width = 75
      Height = 25
      Caption = 'Add Chunk'
      TabOrder = 3
      OnClick = btnAddChunkClick
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
