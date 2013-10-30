object Form1: TForm1
  Left = 0
  Top = 0
  BorderStyle = bsDialog
  Caption = 'Rationalise Tanks'
  ClientHeight = 576
  ClientWidth = 709
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Scan: TPanel
    Left = 0
    Top = 0
    Width = 708
    Height = 403
    Align = alCustom
    Anchors = [akLeft, akTop, akRight]
    TabOrder = 0
    object GroupBox1: TGroupBox
      Left = 8
      Top = 0
      Width = 693
      Height = 65
      Caption = 'Scan'
      TabOrder = 0
      object Label6: TLabel
        Left = 14
        Top = 14
        Width = 18
        Height = 13
        Caption = 'Site'
      end
      object Label7: TLabel
        Left = 348
        Top = 14
        Width = 30
        Height = 13
        Caption = 'Vessel'
      end
      object Site_ComboBox: TComboBox
        Left = 14
        Top = 33
        Width = 328
        Height = 21
        TabOrder = 0
        Text = 'Please select a site'
        OnChange = Site_ComboBoxChange
      end
      object Vessel_ComboBox: TComboBox
        Left = 348
        Top = 33
        Width = 330
        Height = 21
        TabOrder = 1
        OnChange = Vessel_ComboBoxChange
      end
    end
    object GroupBox2: TGroupBox
      Left = 8
      Top = 71
      Width = 693
      Height = 68
      Caption = 'Refine search'
      TabOrder = 1
      object Label4: TLabel
        Left = 14
        Top = 18
        Width = 34
        Height = 13
        Caption = 'Project'
      end
      object Label5: TLabel
        Left = 348
        Top = 18
        Width = 33
        Height = 13
        Caption = 'Aliquot'
      end
      object Aliquot_ComboBox: TComboBox
        Left = 348
        Top = 37
        Width = 330
        Height = 21
        TabOrder = 0
        OnChange = Aliquot_ComboBoxChange
      end
      object Project_ComboBox: TComboBox
        Left = 14
        Top = 37
        Width = 330
        Height = 21
        TabOrder = 1
        OnChange = Project_ComboBoxChange
      end
    end
    object GroupBox3: TGroupBox
      Left = 5
      Top = 145
      Width = 696
      Height = 249
      Caption = 'Results'
      TabOrder = 2
      object Label1: TLabel
        Left = 205
        Top = 44
        Width = 11
        Height = 13
        Caption = '%'
      end
      object Label2: TLabel
        Left = 17
        Top = 22
        Width = 81
        Height = 13
        Caption = 'Rebox Threshold'
      end
      object Label3: TLabel
        Left = 274
        Top = 22
        Width = 71
        Height = 13
        Caption = 'Distination Box'
      end
      object DefragButton: TButton
        Left = 306
        Top = 214
        Width = 75
        Height = 25
        Caption = 'Defrag'
        TabOrder = 0
        OnClick = DefragButtonClick
      end
      object Memo: TMemo
        Left = 17
        Top = 77
        Width = 664
        Height = 131
        ReadOnly = True
        TabOrder = 1
      end
      object ThreshHoldEdit: TEdit
        Left = 166
        Top = 41
        Width = 33
        Height = 21
        Enabled = False
        ReadOnly = True
        TabOrder = 2
        Text = '5'
      end
      object TrackBarThreshold: TTrackBar
        Left = 17
        Top = 41
        Width = 150
        Height = 30
        Max = 100
        Min = 1
        Position = 1
        TabOrder = 3
        OnChange = TrackBarThresholdChange
      end
      object ShowDebugButton: TButton
        Left = 606
        Top = 214
        Width = 75
        Height = 25
        Caption = 'Show debug'
        TabOrder = 4
        Visible = False
        OnClick = ShowDebugButtonClick
      end
      object BoxInfoEdit: TEdit
        Left = 351
        Top = 50
        Width = 330
        Height = 21
        Enabled = False
        ReadOnly = True
        TabOrder = 5
      end
      object BoxSelection_ComboBox: TComboBox
        Left = 351
        Top = 16
        Width = 330
        Height = 21
        TabOrder = 6
        OnChange = BoxSelection_ComboBoxChange
      end
    end
  end
  object DebugPanel: TPanel
    Left = 0
    Top = 400
    Width = 708
    Height = 175
    Caption = 'DebugPanel'
    TabOrder = 1
    object debugMemo: TMemo
      Left = 8
      Top = 9
      Width = 685
      Height = 168
      ReadOnly = True
      ScrollBars = ssVertical
      TabOrder = 0
    end
  end
  object Timer1: TTimer
    OnTimer = TrackBarTimer
    Left = 672
    Top = 128
  end
end
