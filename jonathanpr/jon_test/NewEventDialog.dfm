object eventDialog: TeventDialog
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Add Event'
  ClientHeight = 106
  ClientWidth = 320
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 32
    Top = 5
    Width = 216
    Height = 19
    Caption = 'Enter the new event name'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object nameEdit: TEdit
    Left = 32
    Top = 30
    Width = 265
    Height = 21
    TabOrder = 0
    TextHint = 'Event name'
  end
  object SubmitButton: TButton
    Left = 32
    Top = 57
    Width = 89
    Height = 25
    Caption = 'Submit'
    TabOrder = 1
    OnClick = SubmitButtonClick
  end
  object CancelButton: TButton
    Left = 222
    Top = 57
    Width = 75
    Height = 25
    Caption = 'Cancel'
    TabOrder = 2
    OnClick = CancelButtonClick
  end
end
