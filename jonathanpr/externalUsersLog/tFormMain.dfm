object frmMain: TfrmMain
  Left = 44
  Top = 279
  Width = 896
  Height = 668
  AutoScroll = True
  Caption = 'BioBank research account'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  Visible = True
  OnCloseQuery = FormCloseQuery
  OnResize = FormResize
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object MainTabControl: TTabControl
    AlignWithMargins = True
    Left = 3
    Top = 1599
    Width = 857
    Height = 22
    Align = alTop
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    Style = tsFlatButtons
    TabOrder = 0
    Tabs.Strings = (
      'Details'
      'Users'
      'Correspondence')
    TabIndex = 0
    OnChange = MainTabControlChange
  end
  object Page1: TGridPanel
    AlignWithMargins = True
    Left = 3
    Top = 1036
    Width = 857
    Height = 557
    Align = alTop
    BevelOuter = bvNone
    ColumnCollection = <
      item
        SizeStyle = ssAuto
        Value = 23.076923076923080000
      end
      item
        Value = 100.000000000000000000
      end>
    ControlCollection = <
      item
        Column = 0
        Control = Label3
        Row = 0
      end
      item
        Column = 1
        Control = ApplicationID_Edit
        Row = 0
      end
      item
        Column = 0
        Control = Label4
        Row = 3
      end
      item
        Column = 1
        Control = PI_Memo
        Row = 3
      end
      item
        Column = 1
        Control = Project_Notes_Memo
        Row = 4
      end
      item
        Column = 0
        Control = Label5
        Row = 4
      end
      item
        Column = 1
        Control = project_summary_Edit
        Row = 1
      end
      item
        Column = 0
        Control = Label9
        Row = 1
      end
      item
        Column = 1
        Control = PI_Email_Edit
        Row = 2
      end
      item
        Column = 0
        Control = Label10
        Row = 2
      end
      item
        Column = 1
        Control = projectStatus_ComboBox
        Row = 5
      end
      item
        Column = 0
        Control = Label12
        Row = 5
      end>
    RowCollection = <
      item
        SizeStyle = ssAuto
        Value = 100.000000000000000000
      end
      item
        SizeStyle = ssAuto
        Value = 33.107150001696190000
      end
      item
        SizeStyle = ssAuto
        Value = 49.130150717740650000
      end
      item
        SizeStyle = ssAuto
      end
      item
        SizeStyle = ssAuto
        Value = 100.000000000000000000
      end
      item
        SizeStyle = ssAuto
        Value = 100.000000000000000000
      end
      item
        SizeStyle = ssAuto
        Value = 100.000000000000000000
      end>
    TabOrder = 1
    object Label3: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 3
      Width = 145
      Height = 27
      Align = alTop
      AutoSize = False
      Caption = 'Application ID'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Arial'
      Font.Style = []
      ParentFont = False
      ExplicitWidth = 124
    end
    object ApplicationID_Edit: TEdit
      AlignWithMargins = True
      Left = 154
      Top = 3
      Width = 700
      Height = 27
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 30
      ParentFont = False
      TabOrder = 0
      OnChange = ApplicationID_EditChange
    end
    object Label4: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 103
      Width = 145
      Height = 30
      Align = alTop
      AutoSize = False
      Caption = 'P.I. Details'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 131
      ExplicitWidth = 124
    end
    object PI_Memo: TMemo
      AlignWithMargins = True
      Left = 154
      Top = 103
      Width = 700
      Height = 170
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 255
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 3
      OnChange = PI_MemoChange
    end
    object Project_Notes_Memo: TMemo
      AlignWithMargins = True
      Left = 154
      Top = 279
      Width = 700
      Height = 250
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 5012
      ParentFont = False
      ScrollBars = ssVertical
      TabOrder = 4
      OnChange = Project_Notes_MemoChange
    end
    object Label5: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 279
      Width = 145
      Height = 55
      Align = alTop
      AutoSize = False
      Caption = 'Project Infomation'
      Font.Charset = ANSI_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 130
      ExplicitWidth = 124
    end
    object project_summary_Edit: TEdit
      AlignWithMargins = True
      Left = 154
      Top = 36
      Width = 700
      Height = 27
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 60
      ParentFont = False
      TabOrder = 1
    end
    object Label9: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 36
      Width = 145
      Height = 27
      Align = alTop
      AutoSize = False
      Caption = 'Project Summary'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitTop = 55
      ExplicitWidth = 124
    end
    object PI_Email_Edit: TEdit
      AlignWithMargins = True
      Left = 154
      Top = 69
      Width = 700
      Height = 27
      Align = alTop
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      MaxLength = 30
      ParentFont = False
      TabOrder = 2
    end
    object Label10: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 69
      Width = 145
      Height = 28
      Align = alTop
      AutoSize = False
      Caption = 'P.I. Email'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitLeft = 0
      ExplicitTop = 85
      ExplicitWidth = 130
    end
    object projectStatus_ComboBox: TComboBox
      AlignWithMargins = True
      Left = 154
      Top = 535
      Width = 263
      Height = 27
      Align = alLeft
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 5
      Text = 'projectStatus_ComboBox'
      Items.Strings = (
        'Active'
        'Archived')
    end
    object Label12: TLabel
      AlignWithMargins = True
      Left = 3
      Top = 535
      Width = 43
      Height = 27
      Align = alLeft
      Caption = 'Status'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ExplicitHeight = 19
    end
  end
  object Users_StringGrid: TStringGrid
    AlignWithMargins = True
    Left = 3
    Top = 1828
    Width = 857
    Height = 0
    Align = alClient
    Anchors = [akLeft, akRight]
    FixedColor = clGradientActiveCaption
    FixedCols = 0
    RowCount = 3
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Tahoma'
    Font.Style = []
    Options = [goFixedVertLine, goVertLine, goHorzLine, goRangeSelect, goRowSelect]
    ParentFont = False
    ScrollBars = ssVertical
    TabOrder = 2
    Visible = False
    OnMouseDown = Users_StringGridMouseDown
    ColWidths = (
      64
      64
      64
      64
      64)
  end
  object correspondence_pannel: TPanel
    Left = 0
    Top = 1624
    Width = 863
    Height = 201
    Align = alTop
    Anchors = [akLeft, akRight]
    AutoSize = True
    BevelOuter = bvNone
    TabOrder = 3
    Visible = False
    OnClick = correspondence_pannelClick
    object AddUser_TopPanel: TPanel
      Left = 0
      Top = 0
      Width = 863
      Height = 174
      Align = alTop
      Caption = 'AddUser_TopPanel'
      TabOrder = 0
      object Correspondence_Memo: TMemo
        Left = 1
        Top = 42
        Width = 861
        Height = 103
        Hint = 
          'Type or paste in any relevent corrispondance which you want to c' +
          'onnect with this project'
        Align = alTop
        Anchors = [akLeft, akTop, akRight, akBottom]
        BorderStyle = bsNone
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ParentShowHint = False
        ScrollBars = ssVertical
        ShowHint = True
        TabOrder = 1
      end
      object SummaryPanel: TPanel
        Left = 1
        Top = 1
        Width = 861
        Height = 41
        Align = alTop
        TabOrder = 0
        object Summary_Label: TLabel
          Left = 20
          Top = 16
          Width = 65
          Height = 17
          Align = alCustom
          Caption = 'Summary:'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -14
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
        end
        object Summary_Edit: TEdit
          Left = 91
          Top = 13
          Width = 630
          Height = 26
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
        end
      end
      object Panel1: TPanel
        Left = 1
        Top = 145
        Width = 861
        Height = 28
        Align = alTop
        TabOrder = 2
        object Add_Correspondence_Button: TButton
          Left = 1
          Top = 1
          Width = 200
          Height = 26
          Align = alLeft
          Caption = 'Submit Correspondence'
          Constraints.MaxWidth = 200
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 0
          OnClick = Add_Correspondence_ButtonClick
        end
        object Update_Correspondence_Button: TButton
          Left = 201
          Top = 1
          Width = 197
          Height = 26
          Align = alLeft
          Caption = 'Update Correspondence'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 1
          Visible = False
          OnClick = Update_Correspondence_ButtonClick
        end
        object Cancel_Correspondence_Button: TButton
          Left = 715
          Top = 1
          Width = 145
          Height = 26
          Align = alRight
          Caption = 'Cancel'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clWindowText
          Font.Height = -15
          Font.Name = 'Tahoma'
          Font.Style = []
          ParentFont = False
          TabOrder = 2
          Visible = False
          OnClick = Cancel_Correspondence_ButtonClick
        end
      end
    end
    object CorrispondenceHistory_Panel: TPanel
      Left = 0
      Top = 174
      Width = 863
      Height = 27
      Align = alTop
      AutoSize = True
      TabOrder = 1
    end
  end
  object footer_GroupBox: TGroupBox
    Left = 0
    Top = 1621
    Width = 863
    Height = 34
    Align = alBottom
    TabOrder = 4
    object Update_Button: TButton
      Left = 46
      Top = 3
      Width = 118
      Height = 25
      Caption = 'Save'
      TabOrder = 0
      OnClick = Update_ButtonClick
    end
    object Save_Exit_Button: TButton
      Left = 458
      Top = 7
      Width = 118
      Height = 25
      Caption = 'Cancel changes'
      TabOrder = 1
      OnClick = Save_Exit_ButtonClick
    end
    object Cancel_Button: TButton
      Left = 582
      Top = 7
      Width = 153
      Height = 25
      Caption = 'Goto pick project'
      TabOrder = 2
      OnClick = Cancel_ButtonClick
    end
  end
  object introPannel: TPanel
    Left = 0
    Top = 412
    Width = 863
    Height = 621
    Align = alTop
    BevelOuter = bvNone
    Constraints.MinHeight = 250
    TabOrder = 5
    Visible = False
    DesignSize = (
      863
      621)
    object projectTitle_Label: TLabel
      Left = 144
      Top = 68
      Width = 430
      Height = 23
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Load a pre-existing project or create a new one'
      Constraints.MinWidth = 430
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -19
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      WordWrap = True
    end
    object OpenProject_Button: TButton
      Left = 267
      Top = 256
      Width = 258
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Open'
      Constraints.MinWidth = 150
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = OpenProject_ButtonClick
    end
    object NewProject_Button: TButton
      Left = 267
      Top = 330
      Width = 258
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = 'New'
      Constraints.MinWidth = 150
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 3
      OnClick = NewProject_ButtonClick
    end
    object Projects_ComboBox: TComboBox
      Left = 195
      Top = 130
      Width = 438
      Height = 27
      Anchors = [akLeft, akTop, akRight]
      Constraints.MinWidth = 330
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
      Text = 'Projects_ComboBox'
      OnChange = Projects_ComboBoxChange
    end
    object DeleteProject_Button: TButton
      Left = 267
      Top = 432
      Width = 258
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Delete'
      Constraints.MinWidth = 150
      Enabled = False
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 4
      OnClick = DeleteProject_ButtonClick
    end
    object project_summery_infomation_Edit: TEdit
      AlignWithMargins = True
      Left = 216
      Top = 199
      Width = 387
      Height = 26
      TabStop = False
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      BevelInner = bvNone
      BevelOuter = bvNone
      BorderStyle = bsNone
      Color = cl3DLight
      Constraints.MinWidth = 378
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -13
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      ReadOnly = True
      TabOrder = 5
    end
    object showArchivedProjects_CheckBox: TCheckBox
      Left = 351
      Top = 163
      Width = 97
      Height = 17
      Caption = 'show archived'
      TabOrder = 1
      OnClick = showArchivedProjects_CheckBoxClick
    end
    object Export_Button: TButton
      Left = 267
      Top = 528
      Width = 257
      Height = 41
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Generate Report'
      Constraints.MinWidth = 150
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 6
      OnClick = Export_ButtonClick
    end
    object Signout_Button: TButton
      Left = 6
      Top = 6
      Width = 67
      Height = 25
      Caption = 'Sign out'
      TabOrder = 7
      TabStop = False
      OnClick = Signout_ButtonClick
    end
  end
  object UserLogin_Panel: TPanel
    Left = 0
    Top = 0
    Width = 863
    Height = 241
    Align = alTop
    BevelOuter = bvNone
    TabOrder = 6
    DesignSize = (
      863
      241)
    object Label6: TLabel
      Left = 97
      Top = 47
      Width = 71
      Height = 19
      Alignment = taCenter
      Caption = 'Username'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object Label8: TLabel
      Left = 101
      Top = 94
      Width = 67
      Height = 19
      Caption = 'Password'
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
    end
    object password_Edit: TEdit
      Left = 195
      Top = 92
      Width = 408
      Height = 26
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      Constraints.MinWidth = 300
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      PasswordChar = '*'
      TabOrder = 1
    end
    object username_Edit: TEdit
      Left = 195
      Top = 45
      Width = 408
      Height = 26
      Alignment = taCenter
      Anchors = [akLeft, akTop, akRight]
      Constraints.MinWidth = 300
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 0
    end
    object SignIn_Button: TButton
      Left = 267
      Top = 140
      Width = 258
      Height = 37
      Anchors = [akLeft, akTop, akRight]
      Caption = 'Login'
      Constraints.MinWidth = 150
      Default = True
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -16
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      TabOrder = 2
      OnClick = SignIn_ButtonClick
    end
  end
  object Page2: TPanel
    Left = 0
    Top = 241
    Width = 863
    Height = 171
    Align = alTop
    AutoSize = True
    TabOrder = 7
    object Page21: TGridPanel
      Left = 1
      Top = 141
      Width = 861
      Height = 29
      Align = alTop
      BevelOuter = bvNone
      BorderStyle = bsSingle
      ColumnCollection = <
        item
          Value = 23.566964213745870000
        end
        item
          Value = 11.369095130746070000
        end
        item
          Value = 22.369770777115380000
        end
        item
          Value = 10.705300611806140000
        end
        item
          Value = 21.924118679241660000
        end
        item
          Value = 10.064750587344870000
        end>
      ControlCollection = <
        item
          Column = 0
          Control = Label1
          Row = 0
        end
        item
          Column = 1
          Control = NoWinAccounts_Edit
          Row = 0
        end
        item
          Column = 2
          Control = Label2
          Row = 0
        end
        item
          Column = 3
          Control = NoLinuxAccounts_Edit
          Row = 0
        end
        item
          Column = 4
          Control = Label11
          Row = 0
        end
        item
          Column = 5
          Control = NoGPUAccounts_Edit
          Row = 0
        end>
      Font.Charset = DEFAULT_CHARSET
      Font.Color = clWindowText
      Font.Height = -15
      Font.Name = 'Tahoma'
      Font.Style = []
      ParentFont = False
      RowCollection = <
        item
          Value = 100.000000000000000000
        end
        item
          SizeStyle = ssAuto
        end>
      TabOrder = 0
      DesignSize = (
        857
        25)
      object Label1: TLabel
        Left = 6
        Top = 3
        Width = 189
        Height = 18
        Anchors = []
        Caption = 'Number of Window Accounts'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ExplicitLeft = 70
        ExplicitTop = 8
      end
      object NoWinAccounts_Edit: TEdit
        Left = 234
        Top = 0
        Width = 30
        Height = 25
        TabStop = False
        Anchors = []
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 0
        Text = '0'
      end
      object Label2: TLabel
        Left = 307
        Top = 3
        Width = 172
        Height = 18
        Anchors = []
        Caption = 'Number of Linux Accounts'
        ExplicitLeft = 437
        ExplicitTop = 5
      end
      object NoLinuxAccounts_Edit: TEdit
        Left = 519
        Top = 0
        Width = 30
        Height = 25
        TabStop = False
        Anchors = []
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        ReadOnly = True
        TabOrder = 1
        Text = '0'
      end
      object Label11: TLabel
        Left = 590
        Top = 3
        Width = 167
        Height = 18
        Anchors = []
        Caption = 'Number of GPU Accounts'
        ExplicitLeft = 651
      end
      object NoGPUAccounts_Edit: TEdit
        Left = 797
        Top = 0
        Width = 30
        Height = 25
        Anchors = []
        TabOrder = 2
        Text = '0'
      end
    end
    object TPanel
      Left = 1
      Top = 1
      Width = 861
      Height = 140
      Align = alTop
      BevelKind = bkSoft
      BevelOuter = bvNone
      BorderStyle = bsSingle
      TabOrder = 1
      DesignSize = (
        853
        132)
      object Label7: TLabel
        Left = 25
        Top = 13
        Width = 106
        Height = 19
        Anchors = [akLeft]
        AutoSize = False
        Caption = 'Account name '
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Label13: TLabel
        Left = 624
        Top = 13
        Width = 89
        Height = 19
        Caption = 'Next append'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
      end
      object Status_labal: TLabel
        Left = 232
        Top = 80
        Width = 43
        Height = 19
        BiDiMode = bdLeftToRight
        Caption = 'Status'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentBiDiMode = False
        ParentFont = False
      end
      object AccountName_Edit: TEdit
        Left = 137
        Top = 6
        Width = 300
        Height = 26
        Anchors = [akLeft, akTop, akRight]
        Constraints.MaxWidth = 310
        Constraints.MinWidth = 300
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        MaxLength = 10
        ParentFont = False
        TabOrder = 0
        TextHint = 'User account name'
        OnChange = AccountName_EditChange
      end
      object AccoutAttribs_ButtonGroup: TButtonGroup
        Left = 19
        Top = 38
        Width = 732
        Height = 32
        Anchors = [akLeft, akTop, akRight]
        ButtonWidth = 100
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        Items = <>
        TabOrder = 1
        TabStop = False
        OnButtonClicked = AccoutAttribs_ButtonGroupButtonClicked
        OnDrawButton = AccoutAttribs_ButtonGroupDrawButton
      end
      object AddLinuxAccountButton: TButton
        Left = 343
        Top = 105
        Width = 168
        Height = 25
        Caption = 'Add Linux Account'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 3
        OnClick = AddLinuxAccountButtonClick
      end
      object AddWinAccountButton: TButton
        Left = 153
        Top = 105
        Width = 168
        Height = 25
        Caption = 'Add Windows Account'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 2
        OnClick = AddWinAccountButtonClick
      end
      object updateUser_Button: TButton
        Left = 93
        Top = 105
        Width = 167
        Height = 25
        Caption = 'Update'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 4
        Visible = False
        OnClick = updateUser_ButtonClick
      end
      object cancelUser_Button: TButton
        Left = 383
        Top = 105
        Width = 168
        Height = 25
        Caption = 'Cancel'
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 5
        Visible = False
        OnClick = cancelUser_ButtonClick
      end
      object AddGPUAccountButton: TButton
        Left = 517
        Top = 105
        Width = 168
        Height = 25
        Caption = 'Add GPU Account'
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 6
        OnClick = AddGPUAccountButtonClick
      end
      object userStatus_ComboBox: TComboBox
        AlignWithMargins = True
        Left = 294
        Top = 72
        Width = 201
        Height = 27
        Anchors = [akLeft, akTop, akRight]
        Constraints.MinWidth = 200
        Enabled = False
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -16
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 7
        Items.Strings = (
          'New'
          'Active'
          'Deleted')
      end
      object userValue_Edit: TEdit
        Left = 722
        Top = 6
        Width = 47
        Height = 26
        Alignment = taCenter
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clWindowText
        Font.Height = -15
        Font.Name = 'Tahoma'
        Font.Style = []
        ParentFont = False
        TabOrder = 8
        Text = '0'
        OnChange = userValue_EditChange
      end
      object AccountNameLock_CheckBox: TCheckBox
        Left = 454
        Top = 15
        Width = 117
        Height = 17
        Caption = 'Account name locked'
        TabOrder = 9
      end
    end
  end
  object correspondence_PopupMenu: TPopupMenu
    Left = 752
    Top = 136
    object EditCorrespondence: TMenuItem
      Caption = 'Edit'
      OnClick = EditCorrespondenceClick
    end
    object DeleteCorrespondence: TMenuItem
      Caption = 'Delete'
      OnClick = DeleteCorrespondenceClick
    end
  end
  object BalloonHint: TBalloonHint
    HideAfter = 4000
    Left = 656
    Top = 112
  end
  object userAccount_PopupMenu: TPopupMenu
    Left = 784
    Top = 48
    object UserAccount_Edit: TMenuItem
      Caption = 'Edit'
      OnClick = UserAccount_EditClick
    end
    object Changestatusto2: TMenuItem
      Caption = 'Change status to'
      object New1: TMenuItem
        Caption = 'New'
        OnClick = statusChage_NewClick
      end
      object Active1: TMenuItem
        Caption = 'Active'
        OnClick = statusChage_ActiveClick
      end
      object Delete1: TMenuItem
        Caption = 'Deleted'
        OnClick = statusChage_DeletedClick
      end
    end
  end
  object changeUSerStatus_PopupMenu: TPopupMenu
    Left = 696
    Top = 40
    object Changestatusto1: TMenuItem
      Caption = 'Change status to'
      object statusChage_New: TMenuItem
        Caption = 'New'
        OnClick = statusChage_NewClick
      end
      object statusChage_Active: TMenuItem
        Caption = 'Active'
        OnClick = statusChage_ActiveClick
      end
      object statusChage_Deleted: TMenuItem
        Caption = 'Deleted'
        OnClick = statusChage_DeletedClick
      end
    end
  end
  object SaveTextFileDialog: TSaveTextFileDialog
    DefaultExt = '*.txt'
    Options = [ofOverwritePrompt, ofHideReadOnly, ofPathMustExist, ofEnableSizing]
    Title = 'Save Report'
    Encodings.Strings = (
      'ASCII')
    Left = 592
    Top = 455
  end
end
