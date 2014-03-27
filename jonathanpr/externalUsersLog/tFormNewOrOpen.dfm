object NewOrOpenForm: TNewOrOpenForm
  Left = 0
  Top = 0
  Caption = 'NewOrOpenForm'
  ClientHeight = 620
  ClientWidth = 830
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object TPanel
    Left = 0
    Top = 16
    Width = 833
    Height = 489
    TabOrder = 0
    object projectTitle_Label: TLabel
      Left = 80
      Top = 40
      Width = 689
      Height = 65
      Caption = 'Select a current pre-existing project or create a new one'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -27
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object OpenProject_Button: TButton
      Left = 104
      Top = 320
      Width = 177
      Height = 41
      Caption = 'Open'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object NewProject_Button: TButton
      Left = 592
      Top = 320
      Width = 177
      Height = 41
      Caption = 'New'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      TabOrder = 1
    end
    object Projects_ComboBox: TComboBox
      Left = 32
      Top = 258
      Width = 329
      Height = 32
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -20
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      Text = 'Projects_ComboBox'
    end
  end
end
