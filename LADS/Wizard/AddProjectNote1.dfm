object AddProjectNote: TAddProjectNote
  Left = 192
  Top = 111
  Width = 400
  Height = 219
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Project Note'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object lblTimestamp: TLabel
    Left = 32
    Top = 112
    Width = 3
    Height = 13
  end
  object NoteMemo: TMemo
    Left = 16
    Top = 16
    Width = 361
    Height = 113
    Lines.Strings = (
      ''
      ''
      ''
      '')
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 151
    Width = 392
    Height = 41
    Align = alBottom
    Caption = 'Panel1'
    TabOrder = 1
    object Save: TButton
      Left = 264
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 32
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
    object DeleteNote: TButton
      Left = 148
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteNoteClick
    end
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 64
    Top = 56
  end
end
