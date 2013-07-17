object frmRetrievalAssistant: TfrmRetrievalAssistant
  Left = 0
  Top = 0
  Caption = 'Retrieval Assistant'
  ClientHeight = 504
  ClientWidth = 710
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    710
    504)
  PixelsPerInch = 96
  TextHeight = 13
  object comboJob: TComboBox
    Left = 18
    Top = 360
    Width = 145
    Height = 21
    Style = csDropDownList
    TabOrder = 0
  end
  object lbJobs: TListBox
    Left = 18
    Top = 387
    Width = 145
    Height = 102
    ItemHeight = 13
    TabOrder = 1
  end
  object GroupBox1: TGroupBox
    Left = 0
    Top = 0
    Width = 702
    Height = 354
    Anchors = [akLeft, akTop, akRight, akBottom]
    Caption = 'Jobs'
    TabOrder = 2
    object sgJobs: TStringGrid
      Left = 2
      Top = 15
      Width = 698
      Height = 337
      Align = alClient
      BevelInner = bvNone
      BevelOuter = bvNone
      ColCount = 8
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goHorzLine, goColSizing, goRowSelect]
      TabOrder = 0
      OnDrawCell = sgJobsDrawCell
      ColWidths = (
        196
        64
        64
        64
        64
        64
        64
        64)
    end
  end
  object clbTest: TCheckListBox
    Left = 456
    Top = 368
    Width = 121
    Height = 97
    ItemHeight = 13
    TabOrder = 3
  end
  object CategoryButtons1: TCategoryButtons
    Left = 224
    Top = 368
    ButtonFlow = cbfVertical
    Categories = <
      item
        Color = 16771818
        Collapsed = False
        Items = <>
      end
      item
        Color = 15400959
        Collapsed = False
        Items = <>
      end
      item
        Color = 16777194
        Collapsed = False
        Items = <>
      end
      item
        Color = 15395839
        Collapsed = False
        Items = <>
      end>
    RegularButtonColor = clWhite
    SelectedButtonColor = 15132390
    TabOrder = 4
  end
end
