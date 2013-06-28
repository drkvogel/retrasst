object frmDiscardPrologue: TfrmDiscardPrologue
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Discard Samples'
  ClientHeight = 234
  ClientWidth = 266
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    266
    234)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 20
    Top = 22
    Width = 38
    Height = 13
    Caption = 'Project:'
  end
  object btnOK: TButton
    Left = 37
    Top = 192
    Width = 80
    Height = 30
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    TabOrder = 0
    OnClick = btnOKClick
  end
  object btnCancel: TButton
    Left = 149
    Top = 192
    Width = 80
    Height = 30
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = btnCancelClick
  end
  object rgCryovialStatus: TRadioGroup
    Left = 70
    Top = 56
    Width = 127
    Height = 113
    Anchors = []
    Caption = 'Stage'
    Items.Strings = (
      'Select sample(s)'
      'Confirm removal'
      'Confirm disposal')
    TabOrder = 2
    OnClick = rgCryovialStatusClick
  end
  object cmbProject: TComboBox
    Left = 70
    Top = 19
    Width = 173
    Height = 21
    Style = csDropDownList
    TabOrder = 3
    OnChange = cmbProjectChange
  end
end
