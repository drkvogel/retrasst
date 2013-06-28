object frmNewTank: TfrmNewTank
  Left = 0
  Top = 0
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Configure Vessel'
  ClientHeight = 591
  ClientWidth = 560
  Color = 12316364
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnShow = OnShow
  PixelsPerInch = 96
  TextHeight = 13
  object BtnNext: TButton
    Left = 243
    Top = 559
    Width = 75
    Height = 25
    Caption = 'Next'
    Default = True
    TabOrder = 0
    OnClick = NextClick
  end
  object BtnCancel: TButton
    Left = 357
    Top = 559
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    TabOrder = 1
    OnClick = CancelClick
  end
  object BtnBack: TButton
    Left = 129
    Top = 559
    Width = 75
    Height = 25
    Caption = 'Back'
    TabOrder = 5
    OnClick = BackClick
  end
  object pnlConfirm: TPanel
    Left = 8
    Top = 8
    Width = 545
    Height = 540
    TabOrder = 4
    object LblLayName1: TLabel
      Left = 15
      Top = 145
      Width = 37
      Height = 13
      Caption = 'Layout:'
    end
    object LblCount1: TLabel
      Left = 340
      Top = 184
      Width = 41
      Height = 13
      Caption = 'Number:'
    end
    object LblSrl1: TLabel
      Left = 340
      Top = 65
      Width = 30
      Height = 13
      Caption = 'Serial:'
    end
    object LblFull1: TLabel
      Left = 15
      Top = 224
      Width = 57
      Height = 13
      Caption = 'Description:'
    end
    object LblName1: TLabel
      Left = 15
      Top = 184
      Width = 54
      Height = 13
      Caption = 'Population:'
    end
    object Label4: TLabel
      Left = 15
      Top = 105
      Width = 69
      Height = 13
      Caption = 'Storage Type:'
    end
    object Label2: TLabel
      Left = 15
      Top = 27
      Width = 22
      Height = 13
      Caption = 'Site:'
    end
    object Label1: TLabel
      Left = 340
      Top = 27
      Width = 41
      Height = 13
      Caption = 'Position:'
    end
    object Label5: TLabel
      Left = 15
      Top = 66
      Width = 31
      Height = 13
      Caption = 'Name:'
    end
    object grdProps1: TStringGrid
      Left = 13
      Top = 265
      Width = 518
      Height = 260
      DefaultColWidth = 100
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing]
      TabOrder = 6
    end
    object TxtLayName1: TEdit
      Left = 97
      Top = 142
      Width = 265
      Height = 21
      MaxLength = 16
      ReadOnly = True
      TabOrder = 4
    end
    object TxtPop1: TEdit
      Left = 400
      Top = 180
      Width = 62
      Height = 21
      MaxLength = 3
      ReadOnly = True
      TabOrder = 5
    end
    object TxtSrl1: TEdit
      Left = 386
      Top = 62
      Width = 145
      Height = 21
      MaxLength = 30
      ReadOnly = True
      TabOrder = 3
    end
    object TxtName1: TEdit
      Left = 97
      Top = 180
      Width = 170
      Height = 21
      MaxLength = 60
      ReadOnly = True
      TabOrder = 1
    end
    object TxtFull1: TEdit
      Left = 97
      Top = 221
      Width = 346
      Height = 21
      MaxLength = 16
      ReadOnly = True
      TabOrder = 0
    end
    object txtStoreType1: TEdit
      Left = 97
      Top = 102
      Width = 265
      Height = 21
      MaxLength = 3
      ReadOnly = True
      TabOrder = 2
    end
    object TxtLocation1: TEdit
      Left = 97
      Top = 23
      Width = 170
      Height = 21
      MaxLength = 60
      ReadOnly = True
      TabOrder = 7
    end
    object TxtPos1: TEdit
      Left = 400
      Top = 23
      Width = 62
      Height = 21
      MaxLength = 60
      ReadOnly = True
      TabOrder = 8
    end
    object TxtPhysical1: TEdit
      Left = 97
      Top = 62
      Width = 170
      Height = 21
      MaxLength = 60
      ReadOnly = True
      TabOrder = 9
    end
  end
  object pnlLayout: TPanel
    Left = 8
    Top = 8
    Width = 545
    Height = 540
    TabOrder = 3
    object LblProps: TLabel
      Left = 17
      Top = 149
      Width = 92
      Height = 13
      Caption = 'Section information'
    end
    object LblCount: TLabel
      Left = 16
      Top = 108
      Width = 55
      Height = 13
      Caption = '# Sections:'
    end
    object LblLayFull: TLabel
      Left = 16
      Top = 70
      Width = 57
      Height = 13
      Caption = 'Description:'
    end
    object LblLayName: TLabel
      Left = 16
      Top = 32
      Width = 66
      Height = 13
      Caption = 'Layout name:'
    end
    object grdProps: TStringGrid
      Left = 15
      Top = 168
      Width = 516
      Height = 349
      DefaultColWidth = 100
      FixedCols = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
      TabOrder = 7
    end
    object BtnEdit: TButton
      Left = 366
      Top = 140
      Width = 75
      Height = 25
      Caption = 'Edit'
      TabOrder = 5
      OnClick = EditClick
    end
    object BtnAdd: TButton
      Left = 275
      Top = 140
      Width = 75
      Height = 25
      Caption = 'Add'
      TabOrder = 4
      OnClick = AddClick
    end
    object UpDownCap: TUpDown
      Left = 143
      Top = 104
      Width = 17
      Height = 21
      Associate = TxtCount
      Min = 1
      Max = 999
      Position = 3
      TabOrder = 3
    end
    object TxtCount: TEdit
      Left = 92
      Top = 104
      Width = 51
      Height = 21
      MaxLength = 3
      TabOrder = 2
      Text = '3'
      OnChange = CountChange
    end
    object TxtLayFull: TEdit
      Left = 92
      Top = 66
      Width = 260
      Height = 21
      MaxLength = 60
      TabOrder = 1
    end
    object TxtLayName: TEdit
      Left = 92
      Top = 28
      Width = 180
      Height = 21
      MaxLength = 16
      TabOrder = 0
    end
    object BtnDelete: TButton
      Left = 457
      Top = 140
      Width = 75
      Height = 25
      Caption = 'Delete'
      TabOrder = 6
      OnClick = DeleteClick
    end
  end
  object pnlTank: TPanel
    Left = 8
    Top = 8
    Width = 545
    Height = 540
    TabOrder = 2
    VerticalAlignment = taAlignTop
    object LblPhys: TLabel
      Left = 24
      Top = 87
      Width = 31
      Height = 13
      Caption = 'Name:'
    end
    object LblPos: TLabel
      Left = 378
      Top = 44
      Width = 41
      Height = 13
      Caption = 'Position:'
    end
    object LblSect: TLabel
      Left = 24
      Top = 266
      Width = 37
      Height = 13
      Caption = 'Layout:'
    end
    object LblSrl: TLabel
      Left = 24
      Top = 131
      Width = 70
      Height = 13
      Caption = 'Serial Number:'
    end
    object lblStoreType: TLabel
      Left = 24
      Top = 175
      Width = 67
      Height = 13
      Caption = 'Storage type:'
    end
    object Label3: TLabel
      Left = 24
      Top = 44
      Width = 22
      Height = 13
      Caption = 'Site:'
    end
    object lblPop: TLabel
      Left = 365
      Top = 220
      Width = 54
      Height = 13
      Caption = 'Population:'
    end
    object cbLayout: TComboBox
      Left = 110
      Top = 262
      Width = 250
      Height = 21
      DropDownCount = 12
      TabOrder = 9
    end
    object TxtPos: TEdit
      Left = 435
      Top = 40
      Width = 45
      Height = 21
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 0
      Text = '1'
      OnChange = TxtPosChange
    end
    object TxtSrl: TEdit
      Left = 110
      Top = 127
      Width = 200
      Height = 21
      CharCase = ecUpperCase
      MaxLength = 30
      TabOrder = 3
    end
    object UpDownPos: TUpDown
      Left = 480
      Top = 40
      Width = 17
      Height = 21
      Associate = TxtPos
      Min = 1
      Max = 999
      Position = 1
      TabOrder = 1
    end
    object cbStoreType: TComboBox
      Left = 110
      Top = 171
      Width = 250
      Height = 21
      TabOrder = 4
    end
    object btnNewType: TButton
      Left = 380
      Top = 168
      Width = 80
      Height = 26
      Caption = 'New type'
      TabOrder = 5
      OnClick = btnNewTypeClick
    end
    object btnNewLayout: TButton
      Left = 380
      Top = 259
      Width = 80
      Height = 26
      Caption = 'New layout'
      TabOrder = 10
      OnClick = btnNewLayoutClick
    end
    object TxtPhysical: TEdit
      Left = 110
      Top = 83
      Width = 200
      Height = 21
      MaxLength = 70
      TabOrder = 2
    end
    object TxtPop: TEdit
      Left = 435
      Top = 217
      Width = 45
      Height = 21
      MaxLength = 3
      NumbersOnly = True
      TabOrder = 7
      Text = '1'
    end
    object UpDownPop: TUpDown
      Left = 480
      Top = 217
      Width = 16
      Height = 21
      Associate = TxtPop
      Min = 1
      Max = 999
      Position = 1
      TabOrder = 8
    end
    object chkPop: TCheckBox
      Left = 24
      Top = 219
      Width = 120
      Height = 17
      Alignment = taLeftJustify
      Caption = 'Multiple populations:'
      TabOrder = 6
      OnClick = chkPopClick
    end
    object cbLocation: TComboBox
      Left = 110
      Top = 40
      Width = 200
      Height = 21
      TabOrder = 11
      OnChange = cbLocationChange
    end
  end
end
