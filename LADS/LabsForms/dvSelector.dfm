object selectorFrame: TselectorFrame
  Left = 0
  Top = 0
  Width = 430
  Height = 120
  TabOrder = 0
  DesignSize = (
    430
    120)
  object Label1: TLabel
    Left = 4
    Top = 5
    Width = 53
    Height = 13
    Caption = 'Descriptor:'
  end
  object Label2: TLabel
    Left = 4
    Top = 35
    Width = 35
    Height = 13
    Caption = 'Values:'
  end
  object Label4: TLabel
    Left = 218
    Top = 5
    Width = 28
    Height = 13
    Caption = 'Filter:'
  end
  object cbDescrip: TComboBox
    Left = 64
    Top = 2
    Width = 120
    Height = 21
    Sorted = True
    TabOrder = 0
    OnChange = cbDescripChange
    OnDropDown = cbDescripDropDown
  end
  object lbDValues: TListBox
    Left = 64
    Top = 32
    Width = 120
    Height = 84
    Anchors = [akLeft, akTop, akBottom]
    ItemHeight = 13
    TabOrder = 1
    OnClick = lbDValuesClick
  end
  object lbDVSelected: TListBox
    Left = 254
    Top = 2
    Width = 172
    Height = 89
    Anchors = [akLeft, akTop, akRight, akBottom]
    ItemHeight = 13
    TabOrder = 2
    OnClick = lbDVSelectedClick
  end
  object btnClear: TButton
    Left = 300
    Top = 97
    Width = 84
    Height = 23
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Clear'
    TabOrder = 3
    OnClick = btnClearClick
  end
end
