inherited frmManageLists: TfrmManageLists
  Caption = 'Manage Lists'
  ClientHeight = 505
  ClientWidth = 776
  OnCreate = FormCreate
  ExplicitWidth = 792
  ExplicitHeight = 543
  PixelsPerInch = 96
  TextHeight = 13
  inherited GroupBox1: TGroupBox
    Width = 776
    Height = 505
    ExplicitWidth = 776
    ExplicitHeight = 505
    inherited splitterDebug: TSplitter
      Top = 401
      Width = 772
      ExplicitTop = 401
      ExplicitWidth = 772
    end
    inherited sgJobs: TStringGrid
      Width = 659
      Height = 386
      ExplicitWidth = 659
      ExplicitHeight = 386
    end
    inherited Panel2: TPanel
      Left = 661
      Height = 386
      ExplicitLeft = 661
      ExplicitHeight = 386
      inherited btnExit: TButton
        Top = 345
        ExplicitTop = 345
      end
      inherited cbLog: TCheckBox
        Top = 328
        ExplicitTop = 328
      end
    end
    inherited panelDebug: TPanel
      Top = 404
      Width = 772
      ExplicitTop = 404
      ExplicitWidth = 772
      inherited memoDebug: TMemo
        Width = 658
        ExplicitWidth = 658
      end
      inherited Panel1: TPanel
        Left = 659
        ExplicitLeft = 659
      end
    end
  end
end
