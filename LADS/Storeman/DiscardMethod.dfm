object frmDiscardMethod: TfrmDiscardMethod
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Job'
  ClientHeight = 100
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
    100)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlMethod: TPanel
    Left = 0
    Top = 0
    Width = 316
    Height = 57
    Alignment = taLeftJustify
    Anchors = [akLeft, akTop, akRight]
    Caption = '    Method'
    TabOrder = 0
    DesignSize = (
      316
      57)
    object cmbMethod: TComboBox
      Left = 89
      Top = 19
      Width = 209
      Height = 21
      Style = csDropDownList
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      OnChange = cmbMethodChange
    end
  end
  object pnlNavigate: TPanel
    Left = 0
    Top = 57
    Width = 316
    Height = 44
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 1
    ExplicitTop = 63
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
    end
  end
end
