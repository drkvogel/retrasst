object FrmErrorReport: TFrmErrorReport
  Left = 0
  Top = 0
  Align = alCustom
  Anchors = []
  AutoSize = True
  BorderStyle = bsDialog
  Caption = 'Report'
  ClientHeight = 273
  ClientWidth = 377
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poOwnerFormCenter
  OnResize = FormResize
  OnShow = FormShow
  ExplicitWidth = 320
  ExplicitHeight = 240
  PixelsPerInch = 96
  TextHeight = 13
  object ErrorTop_Panel: TPanel
    Left = 0
    Top = 0
    Width = 377
    Height = 86
    Align = alTop
    AutoSize = True
    Constraints.MaxHeight = 98
    Constraints.MinHeight = 86
    TabOrder = 0
    object report_Memo: TMemo
      Left = 1
      Top = 1
      Width = 375
      Height = 65
      Cursor = crHelp
      TabStop = False
      Align = alTop
      Anchors = []
      Lines.Strings = (
        'Memo1')
      ReadOnly = True
      TabOrder = 0
    end
    object Details_Button: TButton
      AlignWithMargins = True
      Left = 291
      Top = 69
      Width = 82
      Height = 13
      Align = alRight
      Caption = 'Show Details'
      Constraints.MaxHeight = 25
      Constraints.MaxWidth = 82
      Constraints.MinHeight = 13
      TabOrder = 1
      OnClick = Details_ButtonClick
    end
    object CloseError_Button: TButton
      AlignWithMargins = True
      Left = 4
      Top = 69
      Width = 141
      Height = 13
      Align = alLeft
      Caption = 'Close'
      Constraints.MaxHeight = 25
      Constraints.MinHeight = 13
      Default = True
      TabOrder = 2
      OnClick = CloseError_ButtonClick
    end
  end
  object errorDetails_Panel: TPanel
    Left = 0
    Top = 86
    Width = 377
    Height = 187
    Align = alClient
    AutoSize = True
    Caption = 'errorDetails_Panel'
    TabOrder = 1
    object details_Memo: TMemo
      Left = 1
      Top = 1
      Width = 375
      Height = 185
      TabStop = False
      Align = alClient
      Lines.Strings = (
        'Memo2')
      ReadOnly = True
      TabOrder = 0
    end
  end
end
