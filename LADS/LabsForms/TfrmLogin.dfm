inherited frmLogin: TfrmLogin
  ClientHeight = 316
  ClientWidth = 313
  ExplicitWidth = 319
  ExplicitHeight = 344
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label2: TLabel
    Left = 40
    Top = 109
    ExplicitLeft = 40
    ExplicitTop = 109
  end
  object Label4: TLabel [3]
    Left = 40
    Top = 197
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  inherited Label3: TLabel
    Left = 40
    Top = 150
    ExplicitLeft = 40
    ExplicitTop = 150
  end
  inherited exitButton: TBitBtn
    Left = 169
    Top = 256
    ExplicitLeft = 169
    ExplicitTop = 256
  end
  inherited okButton: TBitBtn
    Left = 35
    Top = 256
    ExplicitLeft = 35
    ExplicitTop = 256
  end
  inherited userList: TComboBox
    Left = 117
    Top = 106
    ExplicitLeft = 117
    ExplicitTop = 106
  end
  inherited ebPassword: TEdit
    Left = 117
    Top = 147
    ExplicitLeft = 117
    ExplicitTop = 147
  end
  object projName: TComboBox [9]
    Left = 117
    Top = 194
    Width = 160
    Height = 21
    DropDownCount = 20
    Enabled = False
    Sorted = True
    TabOrder = 4
    OnChange = textChange
    OnDropDown = projNameDropDown
  end
  inherited timer: TTimer
    Left = 248
    Top = 32
  end
  inherited IdSMTP1: TIdSMTP
    Left = 192
    Top = 32
  end
end
