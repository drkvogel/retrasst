object AddProfileMapNew: TAddProfileMapNew
  Left = 197
  Top = 111
  Width = 595
  Height = 358
  Caption = 'Add Profile Map'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object lblDescrip0: TLabel
    Left = 16
    Top = 28
    Width = 51
    Height = 13
    Caption = 'Descriptor:'
  end
  object Label1: TLabel
    Left = 16
    Top = 68
    Width = 80
    Height = 13
    Caption = 'Values available:'
  end
  object Label2: TLabel
    Left = 300
    Top = 28
    Width = 73
    Height = 13
    Caption = 'Values chosen:'
  end
  object Label3: TLabel
    Left = 16
    Top = 252
    Width = 75
    Height = 13
    Caption = 'Profile selected:'
  end
  object Panel1: TPanel
    Left = 0
    Top = 290
    Width = 587
    Height = 41
    Align = alBottom
    TabOrder = 0
    object btnCancel: TButton
      Left = 76
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
    end
    object btnSave: TButton
      Left = 256
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnSaveClick
    end
    object btnClear: TButton
      Left = 436
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Clear'
      TabOrder = 2
      OnClick = FormActivate
    end
  end
  object lbxValues: TListBox
    Left = 104
    Top = 64
    Width = 160
    Height = 161
    ItemHeight = 13
    TabOrder = 1
    OnClick = lbxValuesClick
  end
  object lbxValuesChosen: TListBox
    Left = 384
    Top = 24
    Width = 190
    Height = 201
    Enabled = False
    ItemHeight = 13
    TabOrder = 2
  end
  object edtDescriptor: TEdit
    Left = 104
    Top = 24
    Width = 160
    Height = 21
    Enabled = False
    TabOrder = 3
  end
  object edtProfile: TEdit
    Left = 104
    Top = 248
    Width = 471
    Height = 21
    Enabled = False
    TabOrder = 4
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 40
    Top = 128
  end
end
