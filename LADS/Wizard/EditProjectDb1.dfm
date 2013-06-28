object EditProjectDb: TEditProjectDb
  Left = 196
  Top = 112
  Width = 411
  Height = 194
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Edit Project Database '
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
  object Panel2: TPanel
    Left = 21
    Top = 8
    Width = 360
    Height = 105
    TabOrder = 0
    object EditDescriptors: TButton
      Left = 44
      Top = 17
      Width = 120
      Height = 30
      Caption = 'Descriptors'
      TabOrder = 0
      OnClick = EditDescriptorsClick
    end
    object EditProfiles: TButton
      Left = 44
      Top = 57
      Width = 120
      Height = 30
      Caption = 'Test Profiles'
      TabOrder = 1
      OnClick = EditProfilesClick
    end
    object EditProfileMap: TButton
      Left = 196
      Top = 57
      Width = 120
      Height = 30
      Caption = 'Profile Maps'
      TabOrder = 2
      OnClick = EditProfileMapClick
    end
    object btnBoxTypes: TButton
      Left = 196
      Top = 17
      Width = 120
      Height = 30
      Caption = 'Boxes'
      TabOrder = 3
      OnClick = btnBoxTypesClick
    end
  end
  object Panel3: TPanel
    Left = 0
    Top = 126
    Width = 403
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
