object AddQCTest: TAddQCTest
  Left = 192
  Top = 111
  Width = 482
  Height = 271
  BorderIcons = []
  Caption = 'Add QC Test'
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
  object Label2: TLabel
    Left = 104
    Top = 104
    Width = 71
    Height = 13
    Caption = 'Packet values:'
  end
  object Label4: TLabel
    Left = 120
    Top = 160
    Width = 89
    Height = 13
    Caption = 'Standard deviation'
  end
  object Label5: TLabel
    Left = 160
    Top = 72
    Width = 24
    Height = 13
    Caption = 'Test:'
  end
  object Label7: TLabel
    Left = 120
    Top = 128
    Width = 30
    Height = 13
    Caption = 'Mean:'
  end
  object lblTest: TLabel
    Left = 216
    Top = 72
    Width = 3
    Height = 13
  end
  object pnlMaterial: TPanel
    Left = 0
    Top = 0
    Width = 145
    Height = 41
    BevelOuter = bvNone
    TabOrder = 0
    object Label1: TLabel
      Left = 16
      Top = 16
      Width = 40
      Height = 13
      Caption = 'Material:'
    end
    object lblMaterial: TLabel
      Left = 64
      Top = 16
      Width = 3
      Height = 13
    end
  end
  object pnlLevel: TPanel
    Left = 152
    Top = 0
    Width = 153
    Height = 41
    BevelOuter = bvNone
    TabOrder = 1
    object lblLevel: TLabel
      Left = 54
      Top = 16
      Width = 3
      Height = 13
    end
    object Label6: TLabel
      Left = 12
      Top = 16
      Width = 29
      Height = 13
      Caption = 'Level:'
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 203
    Width = 474
    Height = 41
    Align = alBottom
    TabOrder = 2
    object btnCancel: TButton
      Left = 72
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
    end
    object btnSave: TButton
      Left = 312
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 1
      OnClick = btnSaveClick
    end
  end
  object pnlLot: TPanel
    Left = 312
    Top = 0
    Width = 153
    Height = 41
    BevelOuter = bvNone
    TabOrder = 3
    object lblLot: TLabel
      Left = 54
      Top = 16
      Width = 3
      Height = 13
    end
    object Label3: TLabel
      Left = 12
      Top = 16
      Width = 18
      Height = 13
      Caption = 'Lot:'
    end
  end
  object edtMean: TEdit
    Left = 216
    Top = 120
    Width = 121
    Height = 21
    TabOrder = 4
  end
  object edtStdDev: TEdit
    Left = 216
    Top = 152
    Width = 121
    Height = 21
    TabOrder = 5
  end
  object QueryCentral: TQuery
    DatabaseName = 'centralData'
    Left = 320
    Top = 72
  end
end
