object frmClipboard: TfrmClipboard
  Left = 373
  Top = 316
  Caption = 'Clipboard'
  ClientHeight = 334
  ClientWidth = 557
  Color = 10079487
  Constraints.MinHeight = 100
  Constraints.MinWidth = 400
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 293
    Width = 557
    Height = 41
    Align = alBottom
    TabOrder = 0
    DesignSize = (
      557
      41)
    object Button1: TButton
      Left = 476
      Top = 10
      Width = 75
      Height = 25
      Anchors = [akTop, akRight]
      Caption = 'Cl&ose'
      TabOrder = 0
      OnClick = Button1Click
    end
    object btnCopy: TButton
      Left = 10
      Top = 10
      Width = 121
      Height = 25
      Caption = '&Copy to clipboard'
      TabOrder = 1
      OnClick = btnCopyClick
    end
  end
  object Memo1: TMemo
    Left = 0
    Top = 0
    Width = 557
    Height = 293
    Align = alClient
    ScrollBars = ssBoth
    TabOrder = 1
  end
end
