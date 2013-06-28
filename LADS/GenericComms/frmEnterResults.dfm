object formEnterResults: TformEnterResults
  Left = 0
  Top = 0
  Caption = 'Enter results from analyser'
  ClientHeight = 464
  ClientWidth = 910
  Color = 10999254
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox2: TGroupBox
    Left = 0
    Top = 0
    Width = 910
    Height = 439
    Align = alClient
    Caption = 'Results file:'
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 2
      Top = 337
      Width = 906
      Height = 3
      Cursor = crVSplit
      Align = alBottom
      ExplicitTop = 177
      ExplicitWidth = 221
    end
    object memoPreview: TMemo
      Left = 2
      Top = 63
      Width = 906
      Height = 274
      Align = alClient
      ScrollBars = ssVertical
      TabOrder = 0
    end
    object Panel2: TPanel
      Left = 2
      Top = 15
      Width = 906
      Height = 48
      Align = alTop
      BevelOuter = bvLowered
      TabOrder = 1
      object Panel3: TPanel
        Left = 1
        Top = 1
        Width = 904
        Height = 25
        Align = alTop
        BevelOuter = bvLowered
        Caption = 'Panel3'
        TabOrder = 0
        ExplicitWidth = 842
        object editFilename: TEdit
          Left = 1
          Top = 1
          Width = 827
          Height = 23
          Align = alClient
          Enabled = False
          TabOrder = 0
          ExplicitWidth = 765
          ExplicitHeight = 21
        end
        object btnBrowse: TButton
          Left = 828
          Top = 1
          Width = 75
          Height = 23
          Align = alRight
          Caption = 'Browse...'
          TabOrder = 1
          OnClick = btnBrowseClick
          ExplicitLeft = 766
        end
      end
      object panelFormatChooser: TPanel
        Left = 1
        Top = 26
        Width = 904
        Height = 25
        Align = alTop
        BevelOuter = bvLowered
        Caption = '---'
        TabOrder = 1
        object labelOutputFormat: TLabel
          Left = 1
          Top = 1
          Width = 72
          Height = 23
          Align = alLeft
          Caption = 'Output format '
          ExplicitHeight = 13
        end
        object comboFormat: TComboBox
          Left = 73
          Top = 1
          Width = 755
          Height = 21
          Align = alClient
          Style = csDropDownList
          ItemHeight = 13
          TabOrder = 0
          OnChange = comboFormatChange
        end
        object btnProcess: TButton
          Left = 828
          Top = 1
          Width = 75
          Height = 23
          Align = alRight
          Caption = 'Process'
          TabOrder = 1
          OnClick = btnProcessClick
          ExplicitLeft = 766
          ExplicitTop = 6
          ExplicitHeight = 25
        end
      end
    end
    object listboxInfo: TListBox
      Left = 2
      Top = 340
      Width = 906
      Height = 97
      Align = alBottom
      ItemHeight = 13
      TabOrder = 2
    end
  end
  object Panel1: TPanel
    Left = 0
    Top = 439
    Width = 910
    Height = 25
    Align = alBottom
    BevelOuter = bvNone
    TabOrder = 1
    ExplicitTop = 400
    ExplicitWidth = 848
    object btnOK: TButton
      Left = 760
      Top = 0
      Width = 75
      Height = 25
      Align = alRight
      Caption = 'Submit'
      ModalResult = 1
      TabOrder = 0
      OnClick = btnOKClick
      ExplicitLeft = 698
    end
    object Button2: TButton
      Left = 835
      Top = 0
      Width = 75
      Height = 25
      Align = alRight
      Caption = 'Cancel'
      ModalResult = 2
      TabOrder = 1
      ExplicitLeft = 773
    end
    object cbDummyRun: TCheckBox
      Left = 64
      Top = 4
      Width = 97
      Height = 17
      Caption = 'Dummy Run'
      TabOrder = 2
    end
    object cbGenerate: TCheckBox
      Left = 208
      Top = 4
      Width = 97
      Height = 17
      Caption = 'Generate CSV'
      TabOrder = 3
    end
  end
  object openDialogChooseFile: TOpenDialog
    Left = 89
    Top = 17
  end
end
