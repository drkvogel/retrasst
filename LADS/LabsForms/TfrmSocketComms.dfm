inherited frmNetComms: TfrmNetComms
  OnActivate = FormActivate
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label6: TLabel
    Left = 40
    ExplicitLeft = 40
  end
  object Label1: TLabel [1]
    Left = 40
    Top = 72
    Width = 22
    Height = 13
    Caption = 'Port:'
  end
  object Label2: TLabel [2]
    Left = 40
    Top = 112
    Width = 36
    Height = 13
    Caption = 'Retries:'
  end
  object Label3: TLabel [3]
    Left = 200
    Top = 72
    Width = 35
    Height = 13
    Caption = 'RTT/s:'
  end
  inherited okButton: TBitBtn
    TabOrder = 6
  end
  inherited btnCancel: TBitBtn
    TabOrder = 7
  end
  inherited rgWorklist: TRadioGroup
    TabOrder = 5
  end
  inherited rgDatabase: TRadioGroup
    TabOrder = 4
  end
  object tbPort: TEdit [9]
    Left = 90
    Top = 68
    Width = 50
    Height = 21
    TabOrder = 1
    Text = '6001'
  end
  object tbRetries: TEdit [10]
    Left = 90
    Top = 109
    Width = 50
    Height = 21
    TabOrder = 3
    Text = '3'
  end
  object tbRTT: TEdit [11]
    Left = 249
    Top = 68
    Width = 50
    Height = 21
    TabOrder = 2
    Text = '10'
  end
  inherited qCentral: TQuery
    Left = 192
    Top = 96
  end
  object serverSocket: TServerSocket
    Active = False
    Port = 6001
    ServerType = stNonBlocking
    OnClientConnect = serverSocketClientConnect
    OnClientDisconnect = serverSocketClientDisconnect
    OnClientError = serverSocketClientError
    Left = 152
    Top = 96
  end
end
