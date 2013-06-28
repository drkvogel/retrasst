object MachineCReport: TMachineCReport
  Left = 192
  Top = 111
  BorderIcons = [biMinimize, biMaximize]
  Caption = 'Machine Report (Central)'
  ClientHeight = 566
  ClientWidth = 792
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Query1: TQuery
    DatabaseName = 'centralData'
    SQL.Strings = (
      'select * from c_machine m, c_test_machine tm, c_test t'
      '    where m.machine_cid = tm.machine_cid and'
      '               tm.test_cid = t.test_cid'
      '    order by m.external_name, t.external_name')
    Left = 728
    Top = 16
  end
end
