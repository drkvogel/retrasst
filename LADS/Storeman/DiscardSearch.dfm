object frmSearch: TfrmSearch
  Left = 27
  Top = 0
  BorderIcons = []
  Caption = 'Search'
  ClientHeight = 208
  ClientWidth = 578
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  DesignSize = (
    578
    208)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlStep3c: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 155
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 5
    DesignSize = (
      577
      155)
    object pnlFile: TPanel
      Left = 5
      Top = 6
      Width = 564
      Height = 43
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      DesignSize = (
        564
        43)
      object btnFile: TButton
        Left = 16
        Top = 8
        Width = 49
        Height = 25
        Caption = 'Browse'
        TabOrder = 0
        OnClick = btnBrowseClick
      end
      object ediFile: TEdit
        Left = 71
        Top = 10
        Width = 474
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ReadOnly = True
        TabOrder = 1
      end
    end
  end
  object pnlStep3a: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 155
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 4
    DesignSize = (
      577
      155)
    object pnlText: TPanel
      Left = 8
      Top = 8
      Width = 564
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      DesignSize = (
        564
        41)
      object labText: TLabel
        Left = 16
        Top = 14
        Width = 26
        Height = 13
        Caption = 'Enter'
      end
      object ediText: TEdit
        Left = 48
        Top = 11
        Width = 505
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = ediTextChange
        OnKeyPress = ediTextKeyPress
      end
    end
  end
  object pnlStep1: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 155
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
    object rgType: TRadioGroup
      Left = 8
      Top = 8
      Width = 89
      Height = 97
      Caption = 'Type'
      Items.Strings = (
        'Tube'
        'Cryovial'
        'Box'
        'Person')
      TabOrder = 0
      OnClick = rgTypeClick
    end
  end
  object pnlStep2: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 155
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 3
    object rgSource: TRadioGroup
      Left = 8
      Top = 9
      Width = 89
      Height = 56
      Caption = 'Input'
      Items.Strings = (
        'Text'
        'File')
      TabOrder = 0
      OnClick = rgSourceClick
    end
  end
  object pnlNavigate: TPanel
    Left = 0
    Top = 159
    Width = 577
    Height = 41
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 0
    DesignSize = (
      577
      41)
    object btnBack: TButton
      Left = 176
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akLeft, akTop, akBottom]
      Caption = 'Back'
      TabOrder = 0
      OnClick = btnBackClick
    end
    object btnNext: TButton
      Left = 264
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akLeft, akTop, akBottom]
      Caption = 'Next'
      TabOrder = 1
      OnClick = btnNextClick
    end
    object btnCancel: TButton
      Left = 495
      Top = 8
      Width = 75
      Height = 25
      Anchors = [akTop, akRight, akBottom]
      Caption = 'Cancel'
      TabOrder = 2
      OnClick = btnCancelClick
    end
  end
  object pnlStep3b: TPanel
    Left = 0
    Top = 0
    Width = 577
    Height = 155
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    DesignSize = (
      577
      155)
    object pnlMenu: TPanel
      Left = 8
      Top = 5
      Width = 564
      Height = 42
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      DesignSize = (
        564
        42)
      object labMenu: TLabel
        Left = 16
        Top = 16
        Width = 36
        Height = 13
        Caption = 'Choose'
      end
      object cmbMenu: TComboBox
        Left = 58
        Top = 13
        Width = 495
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = cmbMenuChange
      end
    end
  end
  object openFile: TOpenDialog
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 296
    Top = 65528
  end
end
