object frmEnterResults: TfrmEnterResults
  Left = 650
  Top = 321
  Caption = 'Enter results from Empower(tm)'
  ClientHeight = 190
  ClientWidth = 287
  Color = 16761056
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
    Left = 15
    Top = 15
    Width = 152
    Height = 13
    Caption = 'Choose Empower(tm) output file:'
  end
  object Label2: TLabel
    Left = 15
    Top = 85
    Width = 116
    Height = 13
    Caption = 'Choose source analyser:'
  end
  object edtFilename: TEdit
    Left = 15
    Top = 35
    Width = 246
    Height = 21
    Enabled = False
    TabOrder = 0
  end
  object btnBrowse: TButton
    Left = 185
    Top = 65
    Width = 75
    Height = 25
    Caption = 'Browse...'
    TabOrder = 1
    OnClick = btnBrowseClick
  end
  object btnOK: TButton
    Left = 60
    Top = 160
    Width = 75
    Height = 25
    Caption = 'OK'
    ModalResult = 1
    TabOrder = 2
    OnClick = btnOKClick
  end
  object Button2: TButton
    Left = 150
    Top = 160
    Width = 75
    Height = 25
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 3
  end
  object cbAnalysers: TComboBox
    Left = 8
    Top = 104
    Width = 145
    Height = 21
    ItemHeight = 0
    TabOrder = 4
  end
  object OpenDialog1: TOpenDialog
    Left = 580
    Top = 5
  end
  object OpenDialog2: TOpenDialog
    Left = 580
    Top = 5
  end
  object OpenDialog3: TOpenDialog
    Left = 580
    Top = 5
  end
  object OpenDialog4: TOpenDialog
    Left = 165
    Top = 105
  end
end
