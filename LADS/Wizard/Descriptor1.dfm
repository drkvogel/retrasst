object Descriptor: TDescriptor
  Left = 457
  Top = 254
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Descriptor'
  ClientHeight = 548
  ClientWidth = 751
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 507
    Width = 751
    Height = 41
    Align = alBottom
    TabOrder = 0
    ExplicitTop = 514
    object btnClose: TButton
      Left = 40
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object Panel2: TPanel
    Left = 175
    Top = 6
    Width = 401
    Height = 136
    TabOrder = 1
    object Label1: TLabel
      Left = 16
      Top = 8
      Width = 271
      Height = 13
      Caption = 'Click the button for the type of descriptor you wish to add:'
    end
    object Label2: TLabel
      Left = 16
      Top = 72
      Width = 370
      Height = 13
      Caption = 
        'Click anywhere on the row you wish to change or delete, then cli' +
        'ck the button:'
    end
    object Barcode: TButton
      Left = 40
      Top = 32
      Width = 90
      Height = 28
      Caption = 'Barcode'
      TabOrder = 0
      OnClick = BarcodeClick
    end
    object Byhand: TButton
      Left = 156
      Top = 32
      Width = 90
      Height = 28
      Caption = 'Byhand'
      TabOrder = 1
      OnClick = ByhandClick
    end
    object Specimen: TButton
      Left = 272
      Top = 32
      Width = 90
      Height = 28
      Caption = 'Specimen'
      TabOrder = 2
      OnClick = SpecimenClick
    end
    object Change: TButton
      Left = 156
      Top = 96
      Width = 90
      Height = 28
      Caption = 'Change'
      TabOrder = 3
      OnClick = ChangeClick
    end
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 64
    Top = 72
  end
end
