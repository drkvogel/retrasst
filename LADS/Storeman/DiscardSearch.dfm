object frmSearch: TfrmSearch
  Left = 27
  Top = 0
  BorderIcons = []
  Caption = 'Search'
  ClientHeight = 204
  ClientWidth = 552
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  DesignSize = (
    552
    204)
  PixelsPerInch = 96
  TextHeight = 13
  object pnlStep3c: TPanel
    Left = 0
    Top = 0
    Width = 551
    Height = 152
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 5
    DesignSize = (
      551
      152)
    object pnlFile: TPanel
      Left = 5
      Top = 6
      Width = 538
      Height = 43
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 602
      DesignSize = (
        538
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
        Width = 448
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        ReadOnly = True
        TabOrder = 1
        ExplicitWidth = 512
      end
    end
  end
  object pnlStep3a: TPanel
    Left = 0
    Top = 0
    Width = 551
    Height = 152
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 4
    DesignSize = (
      551
      152)
    object pnlText: TPanel
      Left = 8
      Top = 8
      Width = 538
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 602
      DesignSize = (
        538
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
        Width = 479
        Height = 21
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = ediTextChange
        OnKeyPress = ediTextKeyPress
        ExplicitWidth = 543
      end
    end
  end
  object pnlStep2: TPanel
    Left = 0
    Top = 0
    Width = 551
    Height = 152
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 3
    object rgSource: TRadioGroup
      Left = 10
      Top = 9
      Width = 89
      Height = 70
      Caption = 'Input'
      Items.Strings = (
        'Keyboard'
        'File')
      TabOrder = 0
      OnClick = rgSourceClick
    end
  end
  object pnlNavigate: TPanel
    Left = 0
    Top = 159
    Width = 551
    Height = 40
    Anchors = [akLeft, akRight, akBottom]
    TabOrder = 0
    DesignSize = (
      551
      40)
    object btnBack: TButton
      Left = 144
      Top = 8
      Width = 75
      Height = 24
      Anchors = [akLeft, akTop, akBottom]
      Caption = 'Back'
      TabOrder = 0
      OnClick = btnBackClick
      ExplicitHeight = 25
    end
    object btnNext: TButton
      Left = 240
      Top = 8
      Width = 75
      Height = 24
      Anchors = [akLeft, akTop, akBottom]
      Caption = 'Next'
      TabOrder = 1
      OnClick = btnNextClick
      ExplicitHeight = 25
    end
    object btnCancel: TButton
      Left = 460
      Top = 8
      Width = 75
      Height = 24
      Anchors = [akTop, akRight, akBottom]
      Caption = 'Cancel'
      TabOrder = 2
      OnClick = btnCancelClick
      ExplicitLeft = 467
      ExplicitHeight = 25
    end
  end
  object pnlStep3b: TPanel
    Left = 0
    Top = 1
    Width = 551
    Height = 152
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 1
    DesignSize = (
      551
      152)
    object pnlMenu: TPanel
      Left = 8
      Top = 5
      Width = 538
      Height = 42
      Anchors = [akLeft, akTop, akRight]
      TabOrder = 0
      ExplicitWidth = 602
      DesignSize = (
        538
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
        Width = 469
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        TabOrder = 0
        OnChange = cmbMenuChange
        ExplicitWidth = 533
      end
    end
  end
  object pnlStep1: TPanel
    Left = 0
    Top = 0
    Width = 551
    Height = 152
    Anchors = [akLeft, akTop, akRight, akBottom]
    TabOrder = 2
    object rgType: TRadioGroup
      Left = 10
      Top = 8
      Width = 89
      Height = 110
      Caption = 'Type'
      Items.Strings = (
        'Sample'
        'Cryovial'
        'Box'
        'Source')
      TabOrder = 0
      OnClick = rgTypeClick
    end
  end
  object openFile: TOpenDialog
    Options = [ofHideReadOnly, ofFileMustExist, ofEnableSizing]
    Left = 224
    Top = 32
  end
end
