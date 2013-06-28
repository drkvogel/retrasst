object frmQuestion: TfrmQuestion
  Left = 363
  Top = 131
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Please Confirm'
  ClientHeight = 206
  ClientWidth = 483
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  DesignSize = (
    483
    206)
  PixelsPerInch = 96
  TextHeight = 13
  object lblSummary: TLabel
    Left = 20
    Top = 23
    Width = 73
    Height = 16
    Caption = '[summary]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'MS Sans Serif'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object lblDetail: TLabel
    Left = 20
    Top = 56
    Width = 437
    Height = 64
    Anchors = [akLeft, akTop, akRight]
    Caption = 
      '[detail] [detail] [detail] [detail] [detail] [detail] [detail] [' +
      'detail] [detail] [detail] [detail] [detail] [detail] [detail] [d' +
      'etail] [detail] [detail] [detail] [detail] [detail] [detail] [de' +
      'tail] [detail] [detail] [detail] [detail] [detail] [detail] [det' +
      'ail] [detail] [detail] [detail] [detail] [detail] [detail] [deta' +
      'il]'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'MS Sans Serif'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object okBtn: TBitBtn
    Left = 117
    Top = 149
    Width = 102
    Height = 37
    Anchors = [akBottom]
    TabOrder = 0
    Kind = bkOK
    ExplicitTop = 154
  end
  object cancelBtn: TBitBtn
    Left = 263
    Top = 149
    Width = 102
    Height = 37
    Anchors = [akBottom]
    TabOrder = 1
    Kind = bkCancel
    ExplicitTop = 154
  end
end
