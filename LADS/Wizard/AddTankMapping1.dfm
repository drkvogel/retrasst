object AddTankMapping: TAddTankMapping
  Left = 611
  Top = 167
  Width = 495
  Height = 236
  BorderIcons = []
  Caption = 'Add Tank Mapping'
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
  object Label8: TLabel
    Left = 28
    Top = 28
    Width = 44
    Height = 13
    Caption = 'Location:'
  end
  object Label9: TLabel
    Left = 28
    Top = 74
    Width = 67
    Height = 13
    Caption = 'Serial number:'
  end
  object Label2: TLabel
    Left = 330
    Top = 120
    Width = 44
    Height = 13
    Caption = 'Capacity:'
  end
  object Label3: TLabel
    Left = 330
    Top = 28
    Width = 40
    Height = 13
    Caption = 'Position:'
  end
  object Label1: TLabel
    Left = 330
    Top = 74
    Width = 63
    Height = 13
    Caption = 'Temperature:'
  end
  object Label4: TLabel
    Left = 28
    Top = 120
    Width = 52
    Height = 13
    Caption = 'Rack type:'
  end
  object Panel1: TPanel
    Left = 0
    Top = 168
    Width = 487
    Height = 41
    Align = alBottom
    TabOrder = 6
    object Save: TButton
      Left = 314
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object btnClose: TButton
      Left = 98
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Close'
      ModalResult = 2
      TabOrder = 1
      OnClick = btnCloseClick
    end
  end
  object edtSerial: TEdit
    Left = 110
    Top = 70
    Width = 160
    Height = 21
    TabOrder = 2
  end
  object edtRackCount: TEdit
    Left = 408
    Top = 116
    Width = 50
    Height = 21
    TabOrder = 5
  end
  object edtPosition: TEdit
    Left = 408
    Top = 24
    Width = 50
    Height = 21
    TabOrder = 1
  end
  object edtTemperature: TEdit
    Left = 408
    Top = 70
    Width = 50
    Height = 21
    TabOrder = 3
    Text = '-150'
  end
  object cbRackType: TComboBox
    Left = 110
    Top = 116
    Width = 161
    Height = 21
    ItemHeight = 13
    TabOrder = 4
  end
  object cbLocation: TComboBox
    Left = 110
    Top = 24
    Width = 161
    Height = 21
    ItemHeight = 13
    TabOrder = 0
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 288
    Top = 96
  end
end
