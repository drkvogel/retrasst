object frmSelectDbSystem: TfrmSelectDbSystem
  Left = 0
  Top = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Switch databases'
  ClientHeight = 303
  ClientWidth = 332
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object BitBtn1: TBitBtn
    Left = 46
    Top = 248
    Width = 102
    Height = 37
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
    OnClick = BitBtn1Click
  end
  object rgDatabase: TRadioGroup
    Left = 17
    Top = 24
    Width = 299
    Height = 201
    Caption = 'Currently selected system'
    TabOrder = 1
    TabStop = True
  end
  object BitBtn2: TBitBtn
    Left = 184
    Top = 248
    Width = 102
    Height = 37
    Kind = bkCancel
    NumGlyphs = 2
    TabOrder = 2
  end
end
