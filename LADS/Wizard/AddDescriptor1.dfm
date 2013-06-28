object AddDescriptor: TAddDescriptor
  Left = 459
  Top = 188
  Width = 583
  Height = 600
  HorzScrollBar.Visible = False
  VertScrollBar.Visible = False
  BorderIcons = []
  Caption = 'Add Descriptor'
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
    Left = 18
    Top = 18
    Width = 31
    Height = 13
    Caption = 'Name:'
  end
  object Label2: TLabel
    Left = 274
    Top = 18
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object lblParseOrder: TLabel
    Left = 18
    Top = 95
    Width = 57
    Height = 13
    Caption = 'Parse order:'
  end
  object Label4: TLabel
    Left = 18
    Top = 134
    Width = 111
    Height = 13
    Caption = 'Number of characters --'
  end
  object Label10: TLabel
    Left = 56
    Top = 159
    Width = 20
    Height = 13
    Caption = 'Min:'
  end
  object Label11: TLabel
    Left = 196
    Top = 159
    Width = 23
    Height = 13
    Caption = 'Max:'
  end
  object Label9: TLabel
    Left = 298
    Top = 57
    Width = 101
    Height = 13
    Caption = 'Specimen field name:'
  end
  object Label6: TLabel
    Left = 324
    Top = 95
    Width = 75
    Height = 13
    Caption = 'Map field name:'
  end
  object Label5: TLabel
    Left = 18
    Top = 57
    Width = 61
    Height = 13
    Caption = 'Data source:'
  end
  object Label7: TLabel
    Left = 157
    Top = 95
    Width = 73
    Height = 13
    Caption = 'Start character:'
  end
  object Label8: TLabel
    Left = 342
    Top = 134
    Width = 57
    Height = 13
    Caption = 'Object type:'
  end
  object edtName: TEdit
    Left = 90
    Top = 14
    Width = 131
    Height = 21
    TabOrder = 0
  end
  object edtDescription: TEdit
    Left = 347
    Top = 14
    Width = 213
    Height = 21
    TabOrder = 1
  end
  object edtParseOrder: TEdit
    Left = 90
    Top = 92
    Width = 32
    Height = 21
    TabOrder = 3
  end
  object edtMinLength: TEdit
    Left = 90
    Top = 155
    Width = 42
    Height = 21
    TabOrder = 5
  end
  object edtMaxLength: TEdit
    Left = 230
    Top = 155
    Width = 42
    Height = 21
    TabOrder = 6
  end
  object edtMapFieldName: TEdit
    Left = 416
    Top = 92
    Width = 144
    Height = 21
    TabOrder = 8
  end
  object Panel1: TPanel
    Left = 13
    Top = 196
    Width = 550
    Height = 41
    TabOrder = 11
    object Save: TButton
      Left = 377
      Top = 8
      Width = 76
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 89
      Top = 8
      Width = 76
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object grdValue: TColourStringGrid
    Left = 94
    Top = 350
    Width = 386
    Height = 177
    FixedCols = 0
    TabOrder = 12
    OnMouseDown = grdValueMouseDown
    FirstBitmapLeft = 92
  end
  object Panel2: TPanel
    Left = 107
    Top = 247
    Width = 361
    Height = 97
    TabOrder = 13
    object Label3: TLabel
      Left = 25
      Top = 40
      Width = 310
      Height = 13
      Caption = 
        'or click the row you wish to edit, then click the appropriate bu' +
        'tton:'
    end
    object Add: TButton
      Left = 143
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 62
      Top = 64
      Width = 76
      Height = 26
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
    object Delete: TButton
      Left = 231
      Top = 64
      Width = 76
      Height = 26
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteClick
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 532
    Width = 575
    Height = 41
    Align = alBottom
    TabOrder = 14
    object btnClose: TButton
      Left = 50
      Top = 8
      Width = 76
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object cbSource: TComboBox
    Left = 90
    Top = 53
    Width = 132
    Height = 21
    ItemHeight = 13
    TabOrder = 2
  end
  object edtSpecimenField: TEdit
    Left = 416
    Top = 53
    Width = 144
    Height = 21
    TabOrder = 7
  end
  object edtDelimiter: TEdit
    Left = 240
    Top = 92
    Width = 32
    Height = 21
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -11
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
    TabOrder = 4
  end
  object cbObjectType: TComboBox
    Left = 416
    Top = 130
    Width = 145
    Height = 21
    ItemHeight = 13
    TabOrder = 9
  end
  object cbRequired: TCheckBox
    Left = 360
    Top = 165
    Width = 69
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Current:'
    TabOrder = 10
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 496
    Top = 280
  end
end
