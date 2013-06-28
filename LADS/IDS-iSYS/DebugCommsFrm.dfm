object debugComms: TdebugComms
  Left = 0
  Top = 0
  Caption = 'debugComms'
  ClientHeight = 475
  ClientWidth = 892
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    892
    475)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 276
    Width = 44
    Height = 13
    Caption = 'Received'
  end
  object Label2: TLabel
    Left = 32
    Top = 16
    Width = 94
    Height = 25
    Alignment = taCenter
    Caption = 'IDS-iSYS'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object Label3: TLabel
    Left = 459
    Top = 190
    Width = 29
    Height = 13
    Caption = 'action'
  end
  object Label4: TLabel
    Left = 415
    Top = 190
    Width = 34
    Height = 13
    Caption = 'priority'
  end
  object Label5: TLabel
    Left = 499
    Top = 190
    Width = 62
    Height = 13
    Caption = 'type (U,S,O)'
  end
  object Label6: TLabel
    Left = 567
    Top = 190
    Width = 55
    Height = 13
    Caption = 'report type'
  end
  object Label7: TLabel
    Left = 202
    Top = 190
    Width = 45
    Height = 13
    Caption = 'container'
  end
  object Label8: TLabel
    Left = 253
    Top = 190
    Width = 39
    Height = 13
    Caption = 'barcode'
  end
  object Label9: TLabel
    Left = 343
    Top = 190
    Width = 60
    Height = 13
    Caption = 'manuf. code'
  end
  object analyserOutput: TEdit
    Left = 32
    Top = 56
    Width = 386
    Height = 21
    TabOrder = 0
  end
  object sendButton: TButton
    Left = 424
    Top = 54
    Width = 75
    Height = 25
    Caption = 'send'
    TabOrder = 1
    OnClick = sendButtonClick
  end
  object receivedBytes: TMemo
    Left = 8
    Top = 305
    Width = 621
    Height = 145
    Anchors = [akLeft, akTop, akRight, akBottom]
    Lines.Strings = (
      '')
    ScrollBars = ssBoth
    TabOrder = 2
    WordWrap = False
  end
  object btnSendACK: TButton
    Left = 32
    Top = 114
    Width = 75
    Height = 25
    Caption = 'Send ACK'
    TabOrder = 3
    OnClick = btnSendACKClick
  end
  object btnSendNAK: TButton
    Left = 32
    Top = 145
    Width = 75
    Height = 25
    Caption = 'Send NAK'
    TabOrder = 4
    OnClick = btnSendNAKClick
  end
  object btnSendENQ: TButton
    Left = 32
    Top = 83
    Width = 75
    Height = 25
    Caption = 'Send ENQ'
    TabOrder = 5
    OnClick = btnSendENQClick
  end
  object Memo1: TMemo
    Left = 635
    Top = 305
    Width = 206
    Height = 89
    Lines.Strings = (
      'Memo1')
    TabOrder = 6
  end
  object btnSendHRecord: TButton
    Left = 113
    Top = 83
    Width = 75
    Height = 25
    Caption = 'Send H'
    TabOrder = 7
    OnClick = btnSendHRecordClick
  end
  object btnSendQRecord: TButton
    Left = 113
    Top = 114
    Width = 75
    Height = 25
    Caption = 'Send Q'
    TabOrder = 8
    OnClick = btnSendQRecordClick
  end
  object btnSendLRecord: TButton
    Left = 113
    Top = 269
    Width = 75
    Height = 25
    Caption = 'Send L'
    TabOrder = 9
    OnClick = btnSendLRecordClick
  end
  object editBarcode: TEdit
    Left = 194
    Top = 114
    Width = 95
    Height = 21
    TabOrder = 10
    Text = '999'
  end
  object btnSendPRecord: TButton
    Left = 113
    Top = 153
    Width = 75
    Height = 25
    Caption = 'Send P'
    TabOrder = 11
    OnClick = btnSendPRecordClick
  end
  object btnSendORecord: TButton
    Left = 113
    Top = 205
    Width = 75
    Height = 25
    Caption = 'Send O'
    TabOrder = 12
    OnClick = btnSendORecordClick
  end
  object btnSendRRecord: TButton
    Left = 113
    Top = 238
    Width = 75
    Height = 25
    Caption = 'Send R'
    TabOrder = 13
    OnClick = btnSendRRecordClick
  end
  object editOContainer: TEdit
    Left = 202
    Top = 209
    Width = 43
    Height = 21
    TabOrder = 14
    Text = '01'
  end
  object Memo2: TMemo
    Left = 656
    Top = 114
    Width = 185
    Height = 106
    Lines.Strings = (
      '        container,'
      '        sample,'
      '        manufacturerCode,'
      '        priority,'
      '        chaOAction,'
      '        chaOSampleType,'
      '        chaOReportType')
    TabOrder = 15
  end
  object editOSID: TEdit
    Left = 251
    Top = 209
    Width = 86
    Height = 21
    TabOrder = 16
    Text = '999'
  end
  object editOManu: TEdit
    Left = 343
    Top = 209
    Width = 66
    Height = 21
    TabOrder = 17
    Text = '25DRU'
  end
  object editOPriority: TEdit
    Left = 415
    Top = 209
    Width = 18
    Height = 21
    TabOrder = 18
    Text = 'S'
  end
  object editOAction: TEdit
    Left = 459
    Top = 209
    Width = 22
    Height = 21
    TabOrder = 19
    Text = 'A'
  end
  object editOSampleType: TEdit
    Left = 499
    Top = 209
    Width = 22
    Height = 21
    TabOrder = 20
    Text = 'S'
  end
  object editOReportType: TEdit
    Left = 567
    Top = 209
    Width = 22
    Height = 21
    TabOrder = 21
    Text = 'O'
  end
  object btnSendEOT: TButton
    Left = 32
    Top = 176
    Width = 75
    Height = 25
    Caption = 'Send EOT'
    TabOrder = 22
    OnClick = btnSendEOTClick
  end
  object StatusBar1: TStatusBar
    Left = 0
    Top = 456
    Width = 892
    Height = 19
    Panels = <>
    ExplicitLeft = 456
    ExplicitTop = 232
    ExplicitWidth = 0
  end
end
