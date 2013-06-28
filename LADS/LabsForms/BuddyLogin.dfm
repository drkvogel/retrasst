inherited frmBuddyLogin: TfrmBuddyLogin
  ClientHeight = 325
  ExplicitWidth = 333
  ExplicitHeight = 359
  PixelsPerInch = 96
  TextHeight = 13
  inherited Label2: TLabel
    Left = 48
    Top = 122
    ExplicitLeft = 48
    ExplicitTop = 122
  end
  inherited Label3: TLabel
    Left = 48
    Top = 157
    ExplicitLeft = 48
    ExplicitTop = 157
  end
  object Label4: TLabel [3]
    Left = 48
    Top = 192
    Width = 59
    Height = 13
    Caption = 'Analyser(s):'
  end
  object Label1: TLabel [4]
    Left = 48
    Top = 227
    Width = 86
    Height = 13
    Caption = 'Sample Category:'
  end
  inherited exitButton: TBitBtn
    Top = 270
    TabOrder = 6
    ExplicitTop = 270
  end
  inherited okButton: TBitBtn
    Top = 270
    Enabled = True
    TabOrder = 5
    ExplicitTop = 270
  end
  inherited userList: TComboBox
    Left = 147
    Top = 119
    Width = 132
    ExplicitLeft = 147
    ExplicitTop = 119
    ExplicitWidth = 132
  end
  inherited ebPassword: TEdit
    Left = 147
    Top = 154
    Width = 132
    ExplicitLeft = 147
    ExplicitTop = 154
    ExplicitWidth = 132
  end
  object machineName: TComboBox [10]
    Left = 147
    Top = 189
    Width = 132
    Height = 21
    DropDownCount = 20
    ItemHeight = 13
    Sorted = True
    TabOrder = 3
  end
  object cbCategory: TComboBox [11]
    Left = 147
    Top = 224
    Width = 132
    Height = 21
    ItemHeight = 13
    Sorted = True
    TabOrder = 4
    OnDropDown = cbCategoryDropDown
  end
end
