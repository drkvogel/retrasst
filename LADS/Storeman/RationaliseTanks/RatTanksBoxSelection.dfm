object frmRatTanksBoxSelection: TfrmRatTanksBoxSelection
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Multiple Box types found'
  ClientHeight = 259
  ClientWidth = 495
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnDestroy = FormDestroy
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 432
    Height = 36
    Caption = 
      'More then one box type has been detected in the vessel, do you w' +
      'ant to combine the varous box types when rationalising?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    WordWrap = True
  end
  object CancelButton: TButton
    Left = 214
    Top = 224
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 0
    Visible = False
    OnClick = CancelButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 71
    Width = 247
    Height = 138
    TabOrder = 1
    object Label2: TLabel
      Left = 40
      Top = 58
      Width = 154
      Height = 13
      Caption = 'Combine all box types together.'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object YesButton: TButton
      Left = 88
      Top = 27
      Width = 75
      Height = 25
      Caption = 'Yes'
      TabOrder = 0
      OnClick = YesButtonClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 248
    Top = 71
    Width = 247
    Height = 138
    TabOrder = 2
    object Label3: TLabel
      Left = 40
      Top = 58
      Width = 159
      Height = 26
      Caption = 'Let'#39's just rationalise boxes of the type below:'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -11
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object NoButton: TButton
      Left = 85
      Top = 27
      Width = 75
      Height = 25
      Caption = 'No'
      Enabled = False
      TabOrder = 0
      OnClick = NoButtonClick
    end
    object boxTypeComboBox: TComboBox
      Left = 7
      Top = 101
      Width = 234
      Height = 21
      TabOrder = 1
      Text = 'Please select a box type'
      OnChange = boxTypeComboBoxChange
    end
  end
end
