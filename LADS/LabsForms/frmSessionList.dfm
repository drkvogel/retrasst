object frmSessionHistory: TfrmSessionHistory
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu]
  Caption = 'Session Summary'
  ClientHeight = 364
  ClientWidth = 780
  Color = clSkyBlue
  Constraints.MaxHeight = 1000
  Constraints.MaxWidth = 900
  Constraints.MinHeight = 310
  Constraints.MinWidth = 610
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = True
  Position = poMainFormCenter
  OnActivate = FormActivate
  OnHide = FormHide
  OnResize = FormResize
  DesignSize = (
    780
    364)
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 10
    Top = 20
    Width = 25
    Height = 13
    Caption = 'First:'
  end
  object Label2: TLabel
    Left = 10
    Top = 196
    Width = 24
    Height = 13
    Caption = 'Last:'
  end
  object grid: TStringGrid
    Left = 278
    Top = 8
    Width = 496
    Height = 349
    Anchors = [akLeft, akTop, akRight, akBottom]
    BevelKind = bkSoft
    BorderStyle = bsNone
    Color = clCream
    Ctl3D = True
    DefaultColWidth = 70
    DefaultRowHeight = 21
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goColSizing, goRowSelect]
    ParentCtl3D = False
    ScrollBars = ssVertical
    TabOrder = 0
    OnClick = gridClick
  end
  object mcEndDate: TMonthCalendar
    Left = 42
    Top = 190
    Width = 169
    Height = 153
    AutoSize = True
    Date = 40706.774097488430000000
    MaxDate = 73043.000000000000000000
    MinDate = 38355.000000000000000000
    TabOrder = 1
    OnClick = mcDateClick
    OnEnter = mcEndDateEnter
    OnMouseEnter = mcEndDateEnter
  end
  object mcStartDate: TMonthCalendar
    Left = 42
    Top = 12
    Width = 169
    Height = 153
    AutoSize = True
    Date = 40700.774097488430000000
    MaxDate = 73042.000000000000000000
    MinDate = 39448.000000000000000000
    TabOrder = 2
    OnClick = mcDateClick
    OnEnter = mcStartDateEnter
    OnMouseEnter = mcStartDateEnter
  end
end
