object frmReferred: TfrmReferred
  Left = 265
  Top = 279
  Caption = 'Handle Referred Boxes'
  ClientHeight = 653
  ClientWidth = 975
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  WindowState = wsMaximized
  OnClose = FormClose
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Splitter3: TSplitter
    Left = 790
    Top = 0
    Width = 8
    Height = 617
    Align = alRight
    Beveled = True
    ExplicitLeft = 792
    ExplicitTop = 8
    ExplicitHeight = 695
  end
  object panelProjectsAndBoxTypes: TPanel
    Left = 798
    Top = 0
    Width = 177
    Height = 617
    Align = alRight
    BevelOuter = bvNone
    TabOrder = 0
    object Splitter1: TSplitter
      Left = 0
      Top = 540
      Width = 177
      Height = 8
      Cursor = crVSplit
      Align = alTop
      Beveled = True
      ExplicitTop = 192
    end
    object groupboxProjects: TGroupBox
      Left = 0
      Top = 0
      Width = 177
      Height = 540
      Align = alTop
      Caption = 'Projects'
      TabOrder = 0
      OnClick = groupboxProjectsClick
      object listboxProjects: TListBox
        Left = 2
        Top = 15
        Width = 173
        Height = 523
        Align = alClient
        ItemHeight = 13
        Items.Strings = (
          'dev_stics'
          'dev_ceu'
          'dev_reveal'
          'dev_PINUP'
          'dev_Mexico'
          'dev_hps2-thrive'
          'dev_ex_qa'
          'dev_breakthrough'
          'dev_ascend'
          'dev_kadoorie'
          'dev_experiment'
          'dev_RIHD'
          'dev_sharp'
          'dev_reveal_unexpected')
        MultiSelect = True
        TabOrder = 0
        OnClick = listboxProjectsClick
      end
    end
    object groupboxBoxTypes: TGroupBox
      Left = 0
      Top = 548
      Width = 177
      Height = 28
      Align = alClient
      Caption = 'Box types'
      TabOrder = 1
      OnClick = groupboxBoxTypesClick
      object listboxBoxTypes: TListBox
        Left = 2
        Top = 15
        Width = 173
        Height = 11
        Align = alClient
        ItemHeight = 13
        Items.Strings = (
          'Buffy_Coat'
          'EDTA_1'
          'EDTA_1_FU12'
          'EDTA_2'
          'EDTA_2_FU12'
          'EDTA_3_FU12'
          'Red_Cell'
          'Red_Cell_1_FU12'
          'Red_Cell_2_FU12'
          'Urine_1')
        MultiSelect = True
        Sorted = True
        TabOrder = 0
        OnClick = listboxBoxTypesClick
      end
    end
    object panelBottom: TPanel
      Left = 0
      Top = 576
      Width = 177
      Height = 41
      Align = alBottom
      BevelOuter = bvNone
      TabOrder = 2
      DesignSize = (
        177
        41)
      object btnSignOff: TBitBtn
        Left = 5
        Top = 4
        Width = 85
        Height = 36
        Anchors = [akRight, akBottom]
        Caption = 'Sign off'
        Default = True
        Glyph.Data = {
          76010000424D7601000000000000760000002800000020000000100000000100
          04000000000000010000130B0000130B00001000000000000000000000000000
          800000800000008080008000000080008000808000007F7F7F00BFBFBF000000
          FF0000FF000000FFFF00FF000000FF00FF00FFFF0000FFFFFF00333333333333
          33333333FF33333333FF333993333333300033377F3333333777333993333333
          300033F77FFF3333377739999993333333333777777F3333333F399999933333
          33003777777333333377333993333333330033377F3333333377333993333333
          3333333773333333333F333333333333330033333333F33333773333333C3333
          330033333337FF3333773333333CC333333333FFFFF77FFF3FF33CCCCCCCCCC3
          993337777777777F77F33CCCCCCCCCC3993337777777777377333333333CC333
          333333333337733333FF3333333C333330003333333733333777333333333333
          3000333333333333377733333333333333333333333333333333}
        NumGlyphs = 2
        TabOrder = 0
        OnClick = btnSignOffClick
      end
      object btnDone: TButton
        Left = 100
        Top = 4
        Width = 75
        Height = 36
        Anchors = [akRight, akBottom]
        Caption = 'Exit'
        TabOrder = 1
        OnClick = btnDoneClick
      end
    end
  end
  object panelBoxes: TPanel
    Left = 0
    Top = 0
    Width = 790
    Height = 617
    Align = alClient
    BevelOuter = bvNone
    TabOrder = 1
    object Splitter2: TSplitter
      Left = 0
      Top = 225
      Width = 790
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      Beveled = True
      ExplicitLeft = 6
      ExplicitTop = 255
      ExplicitWidth = 783
    end
    object Splitter4: TSplitter
      Left = 0
      Top = 506
      Width = 790
      Height = 8
      Cursor = crVSplit
      Align = alBottom
      Beveled = True
      ExplicitTop = 587
      ExplicitWidth = 783
    end
    object groupboxBoxDetails: TGroupBox
      Left = 0
      Top = 167
      Width = 790
      Height = 58
      Align = alBottom
      Caption = 'Box details'
      TabOrder = 1
      DesignSize = (
        790
        58)
      object Label2: TLabel
        Left = 12
        Top = 14
        Width = 27
        Height = 13
        Caption = 'Name'
      end
      object Label4: TLabel
        Left = 25
        Top = 38
        Width = 15
        Height = 13
        Caption = '1st'
      end
      object Label1: TLabel
        Left = 156
        Top = 38
        Width = 20
        Height = 13
        Caption = 'Last'
      end
      object Label3: TLabel
        Left = 296
        Top = 14
        Width = 31
        Height = 13
        Caption = 'Status'
      end
      object Label5: TLabel
        Left = 296
        Top = 39
        Width = 23
        Height = 13
        Caption = 'Tank'
      end
      object Label6: TLabel
        Left = 429
        Top = 39
        Width = 23
        Height = 13
        Caption = 'Rack'
      end
      object Label7: TLabel
        Left = 573
        Top = 38
        Width = 18
        Height = 13
        Caption = 'Slot'
      end
      object editBoxName: TEdit
        Left = 45
        Top = 12
        Width = 243
        Height = 21
        TabOrder = 0
        OnChange = editBoxNameChange
      end
      object editFirstPos: TEdit
        Left = 45
        Top = 35
        Width = 28
        Height = 21
        TabOrder = 1
        Text = '1'
      end
      object editLastPos: TEdit
        Left = 183
        Top = 35
        Width = 28
        Height = 21
        TabOrder = 2
        Text = '10'
      end
      object editFirstID: TEdit
        Left = 79
        Top = 35
        Width = 70
        Height = 21
        TabOrder = 3
        Text = 'XXX'
        OnChange = editFirstIDChange
      end
      object editLastID: TEdit
        Left = 218
        Top = 35
        Width = 70
        Height = 21
        TabOrder = 4
        Text = 'YYY'
        OnChange = editLastIDChange
      end
      object comboStatus: TComboBox
        Left = 333
        Top = 12
        Width = 184
        Height = 21
        TabOrder = 6
        Items.Strings = (
          'IN_TANK'
          'SLOT_ALLOCATED'
          'SLOT_CONFIRMED'
          'REFERRED')
      end
      object editSlot: TEdit
        Left = 597
        Top = 34
        Width = 25
        Height = 21
        TabOrder = 8
        Text = '1'
      end
      object cbLog: TCheckBox
        Left = 634
        Top = 37
        Width = 68
        Height = 17
        Anchors = [akTop, akRight]
        Caption = 'Show Log'
        TabOrder = 9
        OnClick = cbLogClick
      end
      object comboEventHistory: TComboBox
        Left = 520
        Top = 12
        Width = 195
        Height = 21
        Style = csDropDownList
        Anchors = [akLeft, akTop, akRight]
        Constraints.MaxWidth = 250
        TabOrder = 10
      end
      object comboRack: TComboBox
        Left = 459
        Top = 35
        Width = 110
        Height = 21
        Style = csDropDownList
        TabOrder = 11
        OnDropDown = comboRackDropDown
      end
      object comboTank: TComboBox
        Left = 333
        Top = 35
        Width = 93
        Height = 21
        Style = csDropDownList
        TabOrder = 12
        OnDropDown = comboTankDropDown
      end
      object updownSlot: TUpDown
        Left = 622
        Top = 34
        Width = 16
        Height = 21
        Associate = editSlot
        Min = 1
        Max = 24
        Position = 1
        TabOrder = 13
      end
      object btnSaveBox: TButton
        Left = 727
        Top = 12
        Width = 59
        Height = 23
        Anchors = [akTop, akRight]
        Caption = 'OK'
        Enabled = False
        TabOrder = 5
        OnClick = btnSaveBoxClick
      end
      object btnDiscard: TButton
        Left = 727
        Top = 35
        Width = 59
        Height = 21
        Anchors = [akTop, akRight]
        Caption = 'Discard'
        TabOrder = 7
        OnClick = btnDiscardClick
      end
    end
    object groupboxReferredBoxes: TGroupBox
      Left = 0
      Top = 0
      Width = 790
      Height = 167
      Align = alClient
      Caption = 'Referred boxes'
      TabOrder = 0
      object sgReferredBoxes: TStringGrid
        Left = 2
        Top = 15
        Width = 373
        Height = 150
        Align = alClient
        ColCount = 11
        Ctl3D = True
        DefaultColWidth = 72
        DefaultDrawing = False
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect]
        ParentCtl3D = False
        TabOrder = 0
        OnDrawCell = sgReferredBoxesDrawCell
        OnSelectCell = sgReferredBoxesSelectCell
      end
      object memoDebug: TMemo
        Left = 375
        Top = 15
        Width = 413
        Height = 150
        Align = alRight
        Lines.Strings = (
          'memoDebug')
        ScrollBars = ssBoth
        TabOrder = 1
        Visible = False
      end
    end
    object groupboxFoundIn: TGroupBox
      Left = 0
      Top = 233
      Width = 790
      Height = 273
      Align = alBottom
      Caption = 'Possible matches found in project database:'
      TabOrder = 2
      object sgMatches: TStringGrid
        Left = 2
        Top = 15
        Width = 786
        Height = 256
        Align = alClient
        ColCount = 7
        DefaultDrawing = False
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
        TabOrder = 0
        OnDrawCell = sgMatchesDrawCell
        OnMouseUp = sgMatchesMouseUp
        ColWidths = (
          64
          64
          64
          64
          64
          64
          64)
      end
    end
    object groupboxStorageHistory: TGroupBox
      Left = 0
      Top = 514
      Width = 790
      Height = 103
      Align = alBottom
      Caption = 'Box Storage History'
      TabOrder = 3
      object sgStorage: TStringGrid
        Left = 2
        Top = 15
        Width = 786
        Height = 86
        Margins.Left = 0
        Margins.Top = 0
        Margins.Right = 0
        Margins.Bottom = 0
        Align = alClient
        ColCount = 6
        DefaultDrawing = False
        FixedCols = 0
        RowCount = 2
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goColSizing, goRowSelect, goThumbTracking]
        TabOrder = 0
        OnDrawCell = sgStorageDrawCell
        OnMouseUp = sgStorageMouseUp
        ColWidths = (
          64
          62
          64
          64
          64
          64)
      end
    end
  end
  object progressBottom: TProgressBar
    Left = 0
    Top = 617
    Width = 975
    Height = 17
    Align = alBottom
    TabOrder = 2
  end
  object statusBar: TStatusBar
    Left = 0
    Top = 634
    Width = 975
    Height = 19
    Panels = <>
    SimplePanel = True
    SimpleText = '<not set>'
  end
  object timerReferredBoxClicked: TTimer
    Enabled = False
    Interval = 500
    OnTimer = timerReferredBoxClickedTimer
    Left = 224
    Top = 64
  end
  object timerBoxNameEdited: TTimer
    Enabled = False
    Interval = 1500
    OnTimer = timerBoxNameEditedTimer
    Left = 96
    Top = 384
  end
  object timerBarcodeEdited: TTimer
    Enabled = False
    Interval = 1500
    OnTimer = timerBarcodeEditedTimer
    Left = 200
    Top = 384
  end
end
