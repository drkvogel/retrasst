object frmDiscardNote: TfrmDiscardNote
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Note'
  ClientHeight = 151
  ClientWidth = 426
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
  object memNote: TMemo
    Left = 8
    Top = 39
    Width = 410
    Height = 73
    MaxLength = 255
    TabOrder = 0
    OnChange = memNoteChange
  end
  object btnOK: TButton
    Left = 135
    Top = 118
    Width = 75
    Height = 25
    Caption = 'OK'
    TabOrder = 1
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 216
    Top = 118
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = btnCancelClick
  end
  object cmbCanned: TComboBox
    Left = 8
    Top = 8
    Width = 353
    Height = 21
    Style = csDropDownList
    TabOrder = 3
    OnChange = cmbCannedChange
  end
  object btnPaste: TButton
    Left = 367
    Top = 6
    Width = 51
    Height = 25
    Caption = 'Paste'
    TabOrder = 4
    OnClick = btnPasteClick
  end
end
