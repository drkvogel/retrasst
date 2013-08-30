inherited frmLogin: TfrmLogin
  ClientHeight = 316
  ClientWidth = 313
  ExplicitWidth = 321
  ExplicitHeight = 350
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label2: TLabel
    Left = 50
    Top = 128
    ExplicitLeft = 50
    ExplicitTop = 128
  end
  inherited Label3: TLabel
    Left = 50
    Top = 169
    ExplicitLeft = 50
    ExplicitTop = 169
  end
  object Label4: TLabel [3]
    Left = 50
    Top = 211
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  inherited exitButton: TBitBtn
    Left = 169
    Top = 256
    TabOrder = 5
    ExplicitLeft = 169
    ExplicitTop = 256
  end
  inherited okButton: TBitBtn
    Left = 35
    Top = 256
    TabOrder = 4
    ExplicitLeft = 35
    ExplicitTop = 256
  end
  inherited userList: TComboBox
    Top = 125
    ExplicitTop = 125
  end
  inherited rgDatabase: TRadioGroup
    Left = 27
    ExplicitLeft = 27
  end
  inherited ebPassword: TEdit
    Top = 166
    ExplicitTop = 166
  end
  object projName: TComboBox [9]
    Left = 126
    Top = 208
    Width = 138
    Height = 21
    DropDownCount = 20
    Enabled = False
    Sorted = True
    TabOrder = 3
    OnChange = textChange
  end
end
