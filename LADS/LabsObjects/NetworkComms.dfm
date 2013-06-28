object NetCommsForm: TNetCommsForm
  Left = 0
  Top = 0
  Caption = 'NetCommsForm'
  ClientHeight = 341
  ClientWidth = 643
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Button1: TButton
    Left = 32
    Top = 88
    Width = 75
    Height = 25
    Caption = 'Accept'
    TabOrder = 0
  end
  object Memo1: TMemo
    Left = 240
    Top = 24
    Width = 257
    Height = 233
    Lines.Strings = (
      'Memo1')
    ScrollBars = ssVertical
    TabOrder = 1
  end
  object ServerSocket1: TServerSocket
    Active = False
    Port = 45678
    ServerType = stNonBlocking
    Left = 32
    Top = 32
  end
end
