object frmAboutBox: TfrmAboutBox
  Left = 749
  Top = 182
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'About the Labs System'
  ClientHeight = 200
  ClientWidth = 319
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = True
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 5
    Top = 8
    Width = 308
    Height = 129
    BevelInner = bvRaised
    BevelOuter = bvLowered
    ParentColor = True
    TabOrder = 1
    object ProgramIcon: TImage
      Left = 16
      Top = 15
      Width = 36
      Height = 36
      Picture.Data = {
        055449636F6E0000010001002020100000000000E80200001600000028000000
        2000000040000000010004000000000080020000000000000000000000000000
        0000000000000000000080000080000000808000800000008000800080800000
        80808000C0C0C0000000FF0000FF000000FFFF00FF000000FF00FF00FFFF0000
        FFFFFF0000000000000000000000000000000000000000000000BBBBBBBB0000
        00000000000000000BBBBBBBBBBBBBB00000000000000000BBBBBBBBBBBBBBBB
        00000000000000BBBBBBBBBBBBBBBBBBBB00000000000BBBBBBBBB0000BBBBBB
        BBB000000000BBBBBBBBB099990BBBBBBBBB00000000BBBBBBBB09999990BBBB
        BBBB0000000BBBBBBBB0900000090BBBBBBBB00000BBBBBBBB000BBBBBB000BB
        BBBBBB0000BBBBBBB00BBBBBBBBBB00BBBBBBB0000BBBBBBB0BBBBBBBBBBBB0B
        BBBBBB000BBBBBBB0BBBBBBBBBBBBBB0BBBBBBB00BBB0000BBBBBBBBBBBBBBBB
        0000BBB00BBBBBB0BBBBBBBBBBBBBBBB0BBBBBB00BBBBBBB0BBBBBBBBBBBBBB0
        BBBBBBB00BBBBBBB0BBBBBBBBBBBBBB0BBBBBBB00BBBBBBBBBB00BBBBBB00BBB
        BBBBBBB00BBBBBBBBB0000BBBB0000BBBBBBBBB00BBBBBBBBB0000BBBB0000BB
        BBBBBBB000BBBBBBBB0000BBBB0000BBBBBBBB0000BBBBBBBB0000BBBB0000BB
        BBBBBB0000BBBBBBBB0000BBBB0000BBBBBBBB00000BBBBBBBB00BBBBBB00BBB
        BBBBB0000000BBBBBBBBBBBBBBBBBBBBBBBB00000000BBBBBBBBBBBBBBBBBBBB
        BBBB000000000BBBBBBBBBBBBBBBBBBBBBB00000000000BBBBBBBBBBBBBBBBBB
        BB00000000000000BBBBBBBBBBBBBBBB00000000000000000BBBBBBBBBBBBBB0
        00000000000000000000BBBBBBBB000000000000000000000000000000000000
        00000000FFF00FFFFF8001FFFE00007FFC00003FF800001FF000000FE0000007
        C0000003C0000003800000018000000180000001000000000000000000000000
        0000000000000000000000000000000000000000800000018000000180000001
        C0000003C0000003E0000007F000000FF800001FFC00003FFE00007FFF8001FF
        FFF00FFF}
    end
    object ProductName: TLabel
      Left = 66
      Top = 15
      Width = 51
      Height = 13
      Caption = 'Labs Utility'
    end
    object Version: TLabel
      Left = 66
      Top = 38
      Width = 40
      Height = 13
      Caption = '[version]'
    end
    object Copyright: TLabel
      Left = 16
      Top = 92
      Width = 36
      Height = 13
      Caption = '[author]'
    end
    object Comments: TLabel
      Left = 16
      Top = 67
      Width = 69
      Height = 13
      Caption = 'Produced by...'
    end
  end
  object okButton: TBitBtn
    Left = 120
    Top = 153
    Width = 80
    Height = 32
    Kind = bkOK
    NumGlyphs = 2
    TabOrder = 0
  end
end
