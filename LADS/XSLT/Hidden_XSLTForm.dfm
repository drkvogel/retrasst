object HiddenXSLTForm: THiddenXSLTForm
  Left = 0
  Top = 0
  Caption = 'HiddenXSLTForm'
  ClientHeight = 293
  ClientWidth = 426
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object transformer: TXSLPageProducer
    Left = 56
    Top = 160
    DOMVendorDesc = 'MSXML'
  end
  object xml: TXMLDocument
    Left = 56
    Top = 96
    DOMVendorDesc = 'MSXML'
  end
end
