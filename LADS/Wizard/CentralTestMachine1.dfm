object CentralTestMachine: TCentralTestMachine
  Left = 348
  Top = 111
  Width = 495
  Height = 469
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'CentralTestMachine'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object CentralTestMachineGrid: TColourStringGrid
    Left = 43
    Top = 128
    Width = 400
    Height = 265
    ScrollBars = ssVertical
    TabOrder = 0
    OnMouseDown = CentralTestMachineGridMouseDown
    FirstBitmapLeft = 92
  end
  object Panel2: TPanel
    Left = 43
    Top = 16
    Width = 400
    Height = 97
    Caption = 'Panel2'
    TabOrder = 1
    object Label1: TLabel
      Left = 13
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 162
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 78
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
    object Delete: TButton
      Left = 246
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteClick
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 401
    Width = 487
    Height = 41
    Align = alBottom
    TabOrder = 2
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
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 336
    Top = 288
  end
end
