object AddBoxType: TAddBoxType
  Left = 192
  Top = 107
  Width = 449
  Height = 600
  BorderIcons = []
  Caption = 'Add BoxType'
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
  object Label1: TLabel
    Left = 88
    Top = 24
    Width = 34
    Height = 13
    Caption = 'Name: '
  end
  object Label6: TLabel
    Left = 88
    Top = 104
    Width = 36
    Height = 13
    Caption = 'Status: '
  end
  object Label2: TLabel
    Left = 88
    Top = 64
    Width = 44
    Height = 13
    Caption = 'Capacity:'
  end
  object edtBoxName: TEdit
    Left = 144
    Top = 16
    Width = 201
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 140
    Width = 441
    Height = 41
    TabOrder = 1
    object Save: TButton
      Left = 264
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 88
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object edtCapacity: TEdit
    Left = 144
    Top = 56
    Width = 145
    Height = 21
    TabOrder = 2
  end
  object cbxStatus: TComboBox
    Left = 144
    Top = 96
    Width = 145
    Height = 21
    Enabled = False
    ItemHeight = 13
    TabOrder = 3
    Items.Strings = (
      'new entry'
      'boxes created'
      'no longer in use')
  end
  object Panel3: TPanel
    Left = 24
    Top = 184
    Width = 393
    Height = 121
    TabOrder = 4
    object Label3: TLabel
      Left = 77
      Top = 40
      Width = 239
      Height = 13
      Caption = 'or click any aliquots you wish to delete,  then click:'
    end
    object Label4: TLabel
      Left = 16
      Top = 96
      Width = 366
      Height = 13
      Caption = 
        'To change the order, mouse down on old position, mouse up on new' +
        ' position.'
    end
    object btnAddAliquot: TButton
      Left = 152
      Top = 8
      Width = 89
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = btnAddAliquotClick
    end
    object btnDeleteAliquot: TButton
      Left = 152
      Top = 64
      Width = 89
      Height = 25
      Caption = 'Delete'
      TabOrder = 1
      OnClick = btnDeleteAliquotClick
    end
  end
  object grdAliquot: TColourStringGrid
    Left = 140
    Top = 312
    Width = 157
    Height = 217
    ScrollBars = ssVertical
    TabOrder = 5
    OnMouseDown = grdAliquotMouseDown
    OnMouseUp = grdAliquotMouseUp
    RowHeights = (
      24
      24
      24
      24
      24)
  end
  object Panel2: TPanel
    Left = 0
    Top = 532
    Width = 441
    Height = 41
    Align = alBottom
    TabOrder = 6
    object btnClose: TButton
      Left = 56
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
