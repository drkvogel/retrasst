object BoxType: TBoxType
  Left = 197
  Top = 111
  Width = 805
  Height = 600
  BorderIcons = []
  Caption = 'BoxType'
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
  object Panel2: TPanel
    Left = 184
    Top = 8
    Width = 393
    Height = 97
    TabOrder = 1
    object Label1: TLabel
      Left = 8
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object Add: TButton
      Left = 152
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 72
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
    object Delete: TButton
      Left = 240
      Top = 64
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteClick
    end
  end
  object BoxTypeTable: TColourStringGrid
    Left = 176
    Top = 112
    Width = 409
    Height = 409
    FixedCols = 0
    ScrollBars = ssVertical
    TabOrder = 2
    OnMouseDown = BoxTypeTableMouseDown
  end
  object Panel1: TPanel
    Left = 0
    Top = 532
    Width = 797
    Height = 41
    Align = alBottom
    TabOrder = 0
    object btnClose: TButton
      Left = 48
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
