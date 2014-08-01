object frmRetrAsstAddNote: TfrmRetrAsstAddNote
  Left = 0
  Top = 0
  Caption = 'Add a note'
  ClientHeight = 282
  ClientWidth = 418
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 241
    Width = 418
    Height = 41
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 0
    ExplicitTop = 247
    object btnAddNote: TBitBtn
      Left = 0
      Top = 0
      Width = 105
      Height = 41
      Align = alLeft
      Caption = 'Add Note'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 0
      OnClick = btnAddNoteClick
      ExplicitTop = 6
    end
    object btnCancel: TBitBtn
      Left = 320
      Top = 0
      Width = 98
      Height = 41
      Align = alRight
      Caption = 'Cancel'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Verdana'
      Font.Style = [fsBold]
      ParentFont = False
      TabOrder = 1
      OnClick = btnCancelClick
    end
  end
  object memoNote: TMemo
    Left = 0
    Top = 0
    Width = 418
    Height = 241
    Align = alClient
    Lines.Strings = (
      'memoNote')
    TabOrder = 1
    ExplicitLeft = 152
    ExplicitTop = 72
    ExplicitWidth = 185
    ExplicitHeight = 89
  end
end
