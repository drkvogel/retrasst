object Help: THelp
  Left = 196
  Top = 112
  Width = 800
  Height = 600
  BorderIcons = []
  Caption = 'Help'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Memo1: TMemo
    Left = 16
    Top = 16
    Width = 329
    Height = 417
    BorderStyle = bsNone
    Color = clInactiveBorder
    Lines.Strings = (
      
        'To enter quality control information open the `qc'#39' project datab' +
        'ase, '
      
        'press the `Quality Control'#39' button and follow the instructions o' +
        'n the '
      'screen.'
      ''
      'The cust_in table in the project database always contains the '
      'following:'
      '      status '
      '      external name'
      '      appt_time_stamp (appointment date) '
      ''
      
        'Descriptors are always read from the descriptor table in order o' +
        'f '
      '      `source'#39' (alphabetically)'
      '      order number.'
      ''
      
        'The above comments apply to version 1 databases, not to version ' +
        '2.')
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 0
    Top = 532
    Width = 792
    Height = 41
    Align = alBottom
    TabOrder = 1
    object btnClose: TButton
      Left = 40
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
end
