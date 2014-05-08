object frmDiscardPrologue: TfrmDiscardPrologue
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Discard Samples'
  ClientHeight = 244
  ClientWidth = 276
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  DesignSize = (
    276
    244)
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
    Top = 202
    Width = 80
    Height = 30
    Anchors = [akLeft, akBottom]
    Caption = 'OK'
    TabOrder = 0
    OnClick = btnOKClick
    ExplicitTop = 192
  end
  object btnCancel: TButton
    Left = 159
    Top = 202
    Width = 80
    Height = 30
    Anchors = [akRight, akBottom]
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = btnCancelClick
    ExplicitLeft = 149
    ExplicitTop = 192
  end
  object rgCryovialStatus: TRadioGroup
    Left = 75
    Top = 61
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
    ExplicitLeft = 70
    ExplicitTop = 56
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
