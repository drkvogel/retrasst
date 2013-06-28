inherited frmSerialConfig: TfrmSerialConfig
  ClientHeight = 415
  OnActivate = FormActivate
  ExplicitWidth = 348
  ExplicitHeight = 449
  PixelsPerInch = 96
  TextHeight = 13
  inherited okButton: TBitBtn
    Top = 355
    TabOrder = 4
    ExplicitTop = 355
  end
  inherited btnCancel: TBitBtn
    Top = 355
    TabOrder = 5
    ExplicitTop = 355
  end
  inherited rgWorklist: TRadioGroup
    Top = 273
    TabOrder = 3
    ExplicitTop = 273
  end
  inherited rgDatabase: TRadioGroup
    Top = 209
    ExplicitTop = 209
  end
  object serialParams: TPanel [6]
    Left = 10
    Top = 58
    Width = 320
    Height = 130
    BevelOuter = bvNone
    TabOrder = 1
    object Label1: TLabel
      Left = 22
      Top = 20
      Width = 48
      Height = 13
      Caption = 'COM port:'
    end
    object Label2: TLabel
      Left = 21
      Top = 59
      Width = 49
      Height = 13
      Caption = 'Baud rate:'
    end
    object Label3: TLabel
      Left = 22
      Top = 96
      Width = 29
      Height = 13
      Caption = 'Parity:'
    end
    object Label4: TLabel
      Left = 192
      Top = 96
      Width = 44
      Height = 13
      Caption = 'Stop bits:'
    end
    object Label5: TLabel
      Left = 192
      Top = 59
      Width = 45
      Height = 13
      Caption = 'Data bits:'
    end
    object edPort: TEdit
      Left = 86
      Top = 17
      Width = 121
      Height = 21
      TabOrder = 0
      Text = 'COM1'
      OnExit = edPortExit
    end
    object edBaud: TEdit
      Left = 86
      Top = 55
      Width = 68
      Height = 21
      TabOrder = 1
      Text = '9600'
    end
    object edParity: TEdit
      Left = 86
      Top = 92
      Width = 68
      Height = 21
      TabOrder = 3
      Text = 'E'
      OnExit = edParityExit
    end
    object edStop: TEdit
      Left = 251
      Top = 92
      Width = 40
      Height = 21
      TabOrder = 4
      Text = '1'
      OnExit = edStopExit
    end
    object edData: TEdit
      Left = 251
      Top = 55
      Width = 40
      Height = 21
      TabOrder = 2
      Text = '7'
      OnExit = edDataExit
    end
  end
  inherited qCentral: TQuery
    Left = 264
    Top = 72
  end
end
