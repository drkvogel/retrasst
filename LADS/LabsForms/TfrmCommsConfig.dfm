object frmCommsConfig: TfrmCommsConfig
  Left = 251
  Top = 117
  BorderIcons = []
  Caption = 'Configuration'
  ClientHeight = 362
  ClientWidth = 340
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  OnShow = FormShow
  DesignSize = (
    340
    362)
  PixelsPerInch = 96
  TextHeight = 13
  object Label6: TLabel
    Left = 41
    Top = 26
    Width = 43
    Height = 13
    Caption = 'Analyser:'
  end
  object okButton: TBitBtn
    Left = 47
    Top = 300
    Width = 98
    Height = 38
    Anchors = [akBottom]
    TabOrder = 3
    OnClick = okButtonClick
    Kind = bkOK
  end
  object btnCancel: TBitBtn
    Left = 195
    Top = 300
    Width = 98
    Height = 38
    Anchors = [akBottom]
    TabOrder = 4
    Kind = bkCancel
  end
  object cbAnalyser: TComboBox
    Left = 102
    Top = 22
    Width = 197
    Height = 21
    Anchors = [akLeft, akTop, akRight]
    DropDownCount = 14
    ItemHeight = 13
    Sorted = True
    TabOrder = 0
    OnDropDown = cbAnalyserDropDown
  end
  object rgWorklist: TRadioGroup
    Left = 15
    Top = 218
    Width = 310
    Height = 52
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Worklist location'
    Columns = 2
    Items.Strings = (
      'Central worklist only'
      'Use local and central')
    TabOrder = 1
  end
  object rgDatabase: TRadioGroup
    Left = 15
    Top = 154
    Width = 310
    Height = 52
    Anchors = [akLeft, akRight, akBottom]
    Caption = 'Database selection'
    Columns = 3
    Items.Strings = (
      'Test'
      'Live'
      'Mirror')
    TabOrder = 2
  end
  object qCentral: TQuery
    DatabaseName = 'central'
    UniDirectional = True
    Left = 48
    Top = 56
  end
end
