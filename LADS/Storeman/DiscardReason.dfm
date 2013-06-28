object frmDiscardReason: TfrmDiscardReason
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Job'
  ClientHeight = 151
  ClientWidth = 315
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    315
    151)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlReason: TPanel
    Left = 0
    Top = 51
    Width = 316
    Height = 57
    Alignment = taLeftJustify
    Anchors = [akLeft, akTop, akRight]
    Caption = '    Reason'
    TabOrder = 0
    DesignSize = (
      316
      57)
    object cmbReason: TComboBox
      Left = 89
      Top = 19
      Width = 209
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = cmbReasonChange
    end
  end
  object pnlNavigate: TPanel
    Left = 0
    Top = 108
    Width = 316
    Height = 44
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 1
    ExplicitTop = 133
    DesignSize = (
      316
      44)
    object btnCancel: TButton
      Left = 158
      Top = 10
      Width = 75
      Height = 25
      Anchors = [akLeft, akRight, akBottom]
      Caption = 'Cancel'
      TabOrder = 0
      OnClick = btnCancelClick
      ExplicitTop = 72
    end
    object btnOK: TButton
      Left = 77
      Top = 10
      Width = 75
      Height = 25
      Anchors = [akLeft, akRight, akBottom]
      Caption = 'OK'
      TabOrder = 1
      OnClick = btnOKClick
      ExplicitTop = 16
    end
  end
  object pnlDesc: TPanel
    Left = 0
    Top = -5
    Width = 316
    Height = 57
    Alignment = taLeftJustify
    Anchors = [akLeft, akTop, akRight]
    Caption = '    Description'
    TabOrder = 2
    object ediDesc: TEdit
      Left = 89
      Top = 19
      Width = 209
      Height = 21
      TabOrder = 0
      OnChange = ediDescChange
    end
  end
end
