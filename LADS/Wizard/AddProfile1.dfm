object AddProfile: TAddProfile
  Left = 191
  Top = 210
  Width = 641
  Height = 609
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Add Profile'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = True
  Position = poScreenCenter
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 60
    Top = 16
    Width = 35
    Height = 13
    Caption = 'Profile: '
  end
  object Label2: TLabel
    Left = 60
    Top = 52
    Width = 56
    Height = 13
    Caption = 'Description:'
  end
  object ProfileName: TEdit
    Left = 128
    Top = 12
    Width = 280
    Height = 21
    TabOrder = 0
  end
  object Panel1: TPanel
    Left = 17
    Top = 120
    Width = 599
    Height = 45
    TabOrder = 6
    object Save: TButton
      Left = 398
      Top = 9
      Width = 76
      Height = 26
      Caption = 'Save'
      TabOrder = 0
      OnClick = SaveClick
    end
    object Cancel: TButton
      Left = 126
      Top = 9
      Width = 76
      Height = 26
      Caption = 'Cancel'
      TabOrder = 1
      OnClick = CancelClick
    end
  end
  object Panel2: TPanel
    Left = 108
    Top = 168
    Width = 417
    Height = 121
    TabOrder = 7
    object Label3: TLabel
      Left = 22
      Top = 40
      Width = 374
      Height = 13
      Caption = 
        'or click anywhere on the row you wish to edit, then click the ap' +
        'propriate button:'
    end
    object dragInstructions: TLabel
      Left = 59
      Top = 96
      Width = 299
      Height = 13
      Caption = 
        'To change the box filling order, drag box type to a new position' +
        '.'
    end
    object Add: TButton
      Left = 171
      Top = 8
      Width = 76
      Height = 26
      Caption = 'Add'
      TabOrder = 0
      OnClick = AddClick
    end
    object Change: TButton
      Left = 87
      Top = 64
      Width = 76
      Height = 26
      Caption = 'Change'
      TabOrder = 1
      OnClick = ChangeClick
    end
    object Delete: TButton
      Left = 255
      Top = 64
      Width = 76
      Height = 26
      Caption = 'Delete'
      TabOrder = 2
      OnClick = DeleteClick
    end
  end
  object Description: TMemo
    Left = 128
    Top = 48
    Width = 434
    Height = 33
    Lines.Strings = (
      '')
    TabOrder = 2
  end
  object pgProfile: TPageControl
    Left = 2
    Top = 296
    Width = 630
    Height = 241
    ActivePage = tabBoxTypes
    TabOrder = 8
    OnChange = pgProfileChange
    object tabTests: TTabSheet
      Caption = 'Tests'
      object grdTest: TStringGrid
        Left = 1
        Top = 4
        Width = 618
        Height = 206
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
        TabOrder = 0
        OnDrawCell = grdTestDrawCell
        OnMouseDown = grdTestMouseDown
        RowHeights = (
          24
          24
          24
          24
          24)
      end
    end
    object tabBoxTypes: TTabSheet
      Caption = 'Box Types'
      ImageIndex = 1
      object grdBoxType: TStringGrid
        Left = 1
        Top = 4
        Width = 618
        Height = 205
        FixedCols = 0
        Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine]
        TabOrder = 0
        OnDrawCell = grdBoxTypeDrawCell
        OnMouseDown = grdBoxTypeMouseDown
        RowHeights = (
          24
          24
          24
          24
          24)
      end
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 541
    Width = 633
    Height = 41
    Align = alBottom
    TabOrder = 9
    object btnClose: TButton
      Left = 48
      Top = 8
      Width = 75
      Height = 25
      Caption = 'Close'
      TabOrder = 0
      OnClick = btnCloseClick
    end
  end
  object cbxActive: TCheckBox
    Left = 496
    Top = 14
    Width = 66
    Height = 17
    Alignment = taLeftJustify
    BiDiMode = bdLeftToRight
    Caption = 'Current:'
    ParentBiDiMode = False
    TabOrder = 1
  end
  object cbxTests: TCheckBox
    Left = 60
    Top = 95
    Width = 104
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Run test profile:'
    TabOrder = 3
  end
  object cbxSpecimen: TCheckBox
    Left = 266
    Top = 95
    Width = 95
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Store source:'
    TabOrder = 4
  end
  object cbxAliquots: TCheckBox
    Left = 464
    Top = 95
    Width = 98
    Height = 17
    Alignment = taLeftJustify
    Caption = 'Take aliquots:'
    TabOrder = 5
  end
  object QueryProject: TQuery
    DatabaseName = 'projectData'
    Left = 24
    Top = 200
  end
end
