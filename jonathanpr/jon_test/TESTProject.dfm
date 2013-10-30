object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Jon database'
  ClientHeight = 318
  ClientWidth = 616
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  DesignSize = (
    616
    318)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 8
    Top = 3
    Width = 32
    Height = 13
    Caption = 'Event:'
  end
  object ActionMainMenuBar1: TActionMainMenuBar
    Left = 0
    Top = 0
    Width = 616
    Height = 25
    UseSystemFont = False
    ActionManager = ActionManager1
    Caption = 'ActionMainMenuBar1'
    Color = clMenuBar
    ColorMap.DisabledFontColor = 7171437
    ColorMap.HighlightColor = clWhite
    ColorMap.BtnSelectedFont = clBlack
    ColorMap.UnusedColor = clWhite
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlack
    Font.Height = -12
    Font.Name = 'Segoe UI'
    Font.Style = []
    Spacing = 0
    ExplicitWidth = 601
  end
  object StringGrid: TStringGrid
    Left = -2
    Top = 64
    Width = 618
    Height = 256
    Anchors = [akLeft, akTop, akRight]
    Color = cl3DLight
    FixedCols = 0
    TabOrder = 0
    OnSelectCell = StringGridSelectCell
    ExplicitWidth = 603
    ColWidths = (
      64
      60
      64
      64
      64)
  end
  object EventCombo: TComboBox
    Left = 0
    Top = 35
    Width = 209
    Height = 21
    TabOrder = 1
    Text = 'EventCombo'
    OnChange = ComboChanged
  end
  object MemberCombobox: TComboBox
    Left = 456
    Top = 29
    Width = 145
    Height = 21
    TabOrder = 2
    Text = 'MemberCombobox'
  end
  object Button1: TButton
    Left = 336
    Top = 27
    Width = 114
    Height = 25
    Caption = 'Add Member ->'
    TabOrder = 3
    OnClick = Button1Click
  end
  object ActionManager1: TActionManager
    ActionBars = <
      item
        Items = <
          item
            Action = Action1
            Caption = '&New Member'
          end
          item
            Action = Action2
            Caption = 'N&ew Event'
          end>
        ActionBar = ActionMainMenuBar1
      end>
    Left = 296
    Top = 168
    StyleName = 'Platform Default'
    object Action1: TAction
      Caption = 'New Member'
      OnExecute = Action1Execute
    end
    object Action2: TAction
      Caption = 'New Event'
      OnExecute = Action2Execute
    end
  end
end
