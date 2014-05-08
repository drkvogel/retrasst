object frmRatTankDbSelectionDialog: TfrmRatTankDbSelectionDialog
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Select database source'
  ClientHeight = 102
  ClientWidth = 428
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object dbCombobox: TComboBox
    Left = 40
    Top = 16
    Width = 353
    Height = 21
    TabOrder = 0
    Text = 'dbCombobox'
  end
  object Button1: TButton
    Left = 40
    Top = 51
    Width = 113
    Height = 25
    Caption = 'Select'
    TabOrder = 1
    OnClick = Button1Click
  end
  object Button2: TButton
    Left = 280
    Top = 51
    Width = 113
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = Button2Click
  end
end
