object frmReport: TfrmReport
  Left = 0
  Top = 0
  Caption = 'frmReport'
  ClientHeight = 376
  ClientWidth = 449
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PixelsPerInch = 96
  TextHeight = 13
  object memoReport: TMemo
    Left = 0
    Top = 0
    Width = 449
    Height = 357
    Align = alClient
    Font.Charset = ANSI_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Courier New'
    Font.Style = [fsBold]
    ParentFont = False
    ReadOnly = True
    ScrollBars = ssBoth
    TabOrder = 0
    WordWrap = False
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 357
    Width = 449
    Height = 19
    Panels = <>
  end
  object MainMenu1: TMainMenu
    Left = 216
    Top = 16
    object Report1: TMenuItem
      Caption = '&Report'
      object Copytoclipboard1: TMenuItem
        Caption = '&Copy to clipboard'
        OnClick = Copytoclipboard1Click
      end
      object SaveAs1: TMenuItem
        Caption = 'Save &As...'
        OnClick = SaveAs1Click
      end
      object Exit1: TMenuItem
        Caption = 'E&xit'
        OnClick = Exit1Click
      end
    end
  end
  object SaveDialog1: TSaveDialog
    DefaultExt = 'txt'
    Filter = '*.txt'
    Left = 264
    Top = 16
  end
end
