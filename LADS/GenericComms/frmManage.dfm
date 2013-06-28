object formManage: TformManage
  Left = 0
  Top = 0
  Caption = 'Manage file formats'
  ClientHeight = 492
  ClientWidth = 974
  Color = clBtnFace
  Constraints.MinHeight = 470
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poOwnerFormCenter
  PrintScale = poNone
  Scaled = False
  ScreenSnap = True
  OnCreate = FormCreate
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object StatusBar1: TStatusBar
    Left = 0
    Top = 473
    Width = 974
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = 'Ready.'
  end
  object groupboxAnalysers: TGroupBox
    Left = 0
    Top = 0
    Width = 273
    Height = 473
    Align = alLeft
    Caption = 'Analysers'
    TabOrder = 1
    object listboxAnalysers: TListBox
      Left = 2
      Top = 15
      Width = 269
      Height = 456
      Align = alClient
      ExtendedSelect = False
      ItemHeight = 13
      TabOrder = 0
      OnClick = listboxAnalysersClick
    end
  end
  object panelFormats: TPanel
    Left = 273
    Top = 0
    Width = 701
    Height = 473
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 2
    object groupboxInputFormats: TGroupBox
      Left = 0
      Top = 0
      Width = 701
      Height = 239
      Align = alClient
      Caption = 'Input formats'
      Color = 16770290
      ParentBackground = False
      ParentColor = False
      TabOrder = 0
      object GroupBox5: TGroupBox
        Left = 312
        Top = 15
        Width = 387
        Height = 222
        Align = alRight
        Anchors = [akLeft, akTop, akRight, akBottom]
        Caption = 'Available  (double-click to edit)'
        TabOrder = 0
        object listboxInputAvailable: TListBox
          Left = 2
          Top = 15
          Width = 383
          Height = 205
          Align = alClient
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
          OnDblClick = listboxInputAvailableDblClick
        end
      end
      object GroupBox4: TGroupBox
        Left = 2
        Top = 15
        Width = 239
        Height = 222
        Align = alLeft
        Caption = 'Assigned'
        TabOrder = 1
        object listboxInputAssigned: TListBox
          Left = 2
          Top = 15
          Width = 235
          Height = 205
          Align = alClient
          ItemHeight = 13
          TabOrder = 0
        end
      end
      object panelInputActions: TPanel
        Left = 241
        Top = 15
        Width = 71
        Height = 222
        Align = alClient
        BevelOuter = bvNone
        Constraints.MaxWidth = 71
        Constraints.MinWidth = 71
        TabOrder = 2
        object btnInputAdd: TButton
          Left = 0
          Top = 97
          Width = 71
          Height = 25
          Align = alBottom
          Caption = '<--'
          TabOrder = 0
          OnClick = btnInputAddClick
        end
        object btnInputRemove: TButton
          Left = 0
          Top = 122
          Width = 71
          Height = 25
          Align = alBottom
          Caption = '-->'
          TabOrder = 1
          OnClick = btnInputRemoveClick
        end
        object btnInputNew: TButton
          Left = 0
          Top = 147
          Width = 71
          Height = 25
          Align = alBottom
          Caption = 'New'
          TabOrder = 2
          OnClick = btnInputNewClick
        end
        object btnInputEdit: TButton
          Left = 0
          Top = 172
          Width = 71
          Height = 25
          Align = alBottom
          Caption = 'Edit'
          TabOrder = 3
          OnClick = btnInputEditClick
        end
        object btnInputDel: TButton
          Left = 0
          Top = 197
          Width = 71
          Height = 25
          Align = alBottom
          Caption = 'Del'
          TabOrder = 4
          OnClick = btnInputDelClick
        end
      end
    end
    object groupboxOutputFormats: TGroupBox
      Left = 0
      Top = 239
      Width = 701
      Height = 234
      Align = alBottom
      Caption = 'Output formats'
      Color = 10999254
      Constraints.MinHeight = 195
      ParentBackground = False
      ParentColor = False
      TabOrder = 1
      object GroupBox3: TGroupBox
        Left = 312
        Top = 15
        Width = 387
        Height = 217
        Align = alRight
        Anchors = [akLeft, akTop, akRight, akBottom]
        Caption = 'Available (double-click to edit)'
        Constraints.MinHeight = 164
        TabOrder = 0
        object listboxOutputAvailable: TListBox
          Left = 2
          Top = 15
          Width = 383
          Height = 200
          Align = alClient
          Anchors = [akTop, akRight, akBottom]
          ItemHeight = 13
          MultiSelect = True
          TabOrder = 0
          OnDblClick = listboxOutputAvailableDblClick
        end
      end
      object groupBoxOutputAssigned: TGroupBox
        Left = 2
        Top = 15
        Width = 237
        Height = 217
        Align = alLeft
        Caption = 'Assigned'
        TabOrder = 1
        object listboxOutputAssigned: TListBox
          Left = 2
          Top = 15
          Width = 233
          Height = 200
          Align = alClient
          Anchors = [akTop, akRight, akBottom]
          ItemHeight = 13
          TabOrder = 0
        end
      end
      object panelOutputActions: TPanel
        Left = 239
        Top = 15
        Width = 73
        Height = 217
        Align = alClient
        BevelOuter = bvNone
        Constraints.MaxWidth = 73
        Constraints.MinWidth = 71
        TabOrder = 2
        object btnOutputAdd: TButton
          Left = 0
          Top = 92
          Width = 73
          Height = 25
          Align = alBottom
          Caption = '<--'
          TabOrder = 0
          OnClick = btnOutputAddClick
          ExplicitLeft = 6
          ExplicitTop = 68
        end
        object btnOutputRemove: TButton
          Left = 0
          Top = 117
          Width = 73
          Height = 25
          Align = alBottom
          Caption = '-->'
          TabOrder = 1
          OnClick = btnOutputRemoveClick
        end
        object btnOutputNew: TButton
          Left = 0
          Top = 142
          Width = 73
          Height = 25
          Align = alBottom
          Caption = 'New'
          TabOrder = 2
          OnClick = btnOutputNewClick
        end
        object btnOutputEdit: TButton
          Left = 0
          Top = 167
          Width = 73
          Height = 25
          Align = alBottom
          Caption = 'Edit'
          TabOrder = 3
          OnClick = btnOutputEditClick
        end
        object btnOutputDel: TButton
          Left = 0
          Top = 192
          Width = 73
          Height = 25
          Align = alBottom
          Caption = 'Del'
          TabOrder = 4
          OnClick = btnOutputDelClick
        end
      end
    end
  end
  object Query1: TQuery
    Left = 64
    Top = 144
  end
end
