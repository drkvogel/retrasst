object frmRatTanksBoxType: TfrmRatTanksBoxType
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Destination Box Type'
  ClientHeight = 458
  ClientWidth = 372
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label5: TLabel
    Left = 20
    Top = 27
    Width = 94
    Height = 13
    Caption = 'Available box types'
  end
  object OK_Button: TButton
    Left = 24
    Top = 424
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 0
    OnClick = OK_ButtonClick
  end
  object Cancel_Button: TButton
    Left = 265
    Top = 424
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = Cancel_ButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 64
    Width = 369
    Height = 354
    TabOrder = 2
    object Label2: TLabel
      Left = 42
      Top = 69
      Width = 57
      Height = 13
      Caption = 'Description:'
    end
    object Label3: TLabel
      Left = 39
      Top = 152
      Width = 60
      Height = 13
      Caption = 'First aliquot:'
    end
    object Label4: TLabel
      Left = 34
      Top = 111
      Width = 65
      Height = 13
      Caption = 'Example box:'
    end
    object Label6: TLabel
      Left = 47
      Top = 277
      Width = 52
      Height = 13
      Caption = 'Formation:'
    end
    object Label7: TLabel
      Left = 25
      Top = 194
      Width = 74
      Height = 13
      Caption = 'Second aliquot:'
    end
    object Label8: TLabel
      Left = 36
      Top = 235
      Width = 63
      Height = 13
      Caption = 'Third aliquot:'
    end
    object Label1: TLabel
      Left = 36
      Top = 26
      Width = 63
      Height = 13
      Caption = 'Simple name:'
    end
    object DescriptionEdit: TEdit
      Left = 120
      Top = 66
      Width = 225
      Height = 21
      Enabled = False
      MaxLength = 60
      TabOrder = 1
    end
    object ExampleBoxEdit: TEdit
      Left = 120
      Top = 108
      Width = 225
      Height = 21
      Enabled = False
      ReadOnly = True
      TabOrder = 2
    end
    object FirstAliquotComboBox: TComboBox
      Left = 120
      Top = 149
      Width = 225
      Height = 21
      Enabled = False
      Sorted = True
      TabOrder = 3
    end
    object FormationComboBox: TComboBox
      Left = 120
      Top = 274
      Width = 225
      Height = 21
      Enabled = False
      Sorted = True
      TabOrder = 6
    end
    object EditButton: TButton
      Left = 141
      Top = 313
      Width = 75
      Height = 28
      Caption = 'New'
      TabOrder = 7
      OnClick = EditButtonClick
    end
    object ThirdAliquotComboBox: TComboBox
      Left = 120
      Top = 232
      Width = 225
      Height = 21
      Enabled = False
      Sorted = True
      TabOrder = 5
    end
    object SecondAliquotComboBox: TComboBox
      Left = 120
      Top = 191
      Width = 225
      Height = 21
      Enabled = False
      Sorted = True
      TabOrder = 4
    end
    object SimpleNameEdit: TEdit
      Left = 120
      Top = 23
      Width = 225
      Height = 21
      Enabled = False
      MaxLength = 32
      TabOrder = 0
      OnChange = SimpleNameEditChange
    end
  end
  object BoxType_ComboBox: TComboBox
    Left = 120
    Top = 24
    Width = 225
    Height = 21
    Sorted = True
    TabOrder = 3
    OnChange = BoxType_ComboBoxChange
  end
  object BalloonHint: TBalloonHint
    Delay = 100
    HideAfter = 5000
    Left = 312
    Top = 384
  end
end
