object Form1: TForm1
  Left = 0
  Top = 0
  Caption = 'Form1'
  ClientHeight = 481
  ClientWidth = 624
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Container: TPanel
    Left = 0
    Top = 0
    Width = 624
    Height = 481
    Margins.Left = 50
    Margins.Top = 50
    Margins.Right = 50
    Margins.Bottom = 50
    Align = alClient
    Caption = 'Container'
    Padding.Left = 20
    Padding.Top = 20
    Padding.Right = 20
    Padding.Bottom = 20
    TabOrder = 0
    ExplicitLeft = 104
    ExplicitTop = 80
    ExplicitWidth = 393
    ExplicitHeight = 329
    object Splitter1: TSplitter
      Left = 21
      Top = 171
      Width = 582
      Height = 3
      Cursor = crVSplit
      Align = alTop
      ExplicitLeft = 1
      ExplicitTop = 151
      ExplicitWidth = 177
    end
    object ListView1: TListView
      Left = 21
      Top = 21
      Width = 582
      Height = 150
      Align = alTop
      Columns = <
        item
          Caption = 'col1'
        end
        item
          Caption = 'col2'
        end
        item
          Caption = 'col3'
        end>
      GridLines = True
      TabOrder = 0
      ViewStyle = vsReport
      ExplicitLeft = 8
      ExplicitTop = 16
      ExplicitWidth = 250
    end
    object ListView2: TListView
      Left = 21
      Top = 174
      Width = 582
      Height = 286
      Align = alClient
      Columns = <
        item
          Caption = 'col1'
        end
        item
          Caption = 'col2'
        end
        item
          Caption = 'col3'
        end>
      GridLines = True
      ShowColumnHeaders = False
      TabOrder = 1
      ViewStyle = vsReport
      ExplicitLeft = 64
      ExplicitTop = 176
      ExplicitWidth = 250
      ExplicitHeight = 150
    end
  end
end
