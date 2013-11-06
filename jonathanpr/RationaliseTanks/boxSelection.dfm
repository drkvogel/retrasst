object tBoxSelection: TtBoxSelection
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Multiple Box types found'
  ClientHeight = 296
  ClientWidth = 495
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 24
    Top = 16
    Width = 440
    Height = 66
    Caption = 
      'More then one box type has been detected, would you like to comb' +
      'ine box types together?'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -18
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    ParentShowHint = False
    ShowHint = False
    WordWrap = True
  end
  object CancelButton: TButton
    Left = 220
    Top = 264
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 0
    Visible = False
    OnClick = CancelButtonClick
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 111
    Width = 249
    Height = 147
    TabOrder = 1
    object Label2: TLabel
      Left = 48
      Top = 82
      Width = 154
      Height = 13
      Caption = 'Combine all box types together.'
    end
    object YesButton: TButton
      Left = 80
      Top = 11
      Width = 75
      Height = 25
      Caption = 'Yes'
      TabOrder = 0
      OnClick = YesButtonClick
    end
  end
  object GroupBox2: TGroupBox
    Left = 248
    Top = 111
    Width = 249
    Height = 147
    TabOrder = 2
    object Label3: TLabel
      Left = 32
      Top = 82
      Width = 184
      Height = 13
      Caption = 'Lets just rationalise boxes of one type'
    end
    object NoButton: TButton
      Left = 94
      Top = 11
      Width = 75
      Height = 25
      Caption = 'No'
      Enabled = False
      TabOrder = 0
      OnClick = NoButtonClick
    end
    object boxTypeComboBox: TComboBox
      Left = 0
      Top = 101
      Width = 246
      Height = 21
      TabOrder = 1
      Text = 'Please select a box type'
      OnChange = boxTypeComboBoxChange
    end
  end
end
