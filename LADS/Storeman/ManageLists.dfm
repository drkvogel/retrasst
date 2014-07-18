inherited frmManageLists: TfrmManageLists
  Caption = 'Manage Lists'
  ClientHeight = 592
  ClientWidth = 837
  ExplicitWidth = 853
  ExplicitHeight = 630
  PixelsPerInch = 96
  TextHeight = 13
  inherited GroupBox1: TGroupBox
    Width = 837
    Height = 592
    ExplicitWidth = 116
    ExplicitHeight = 0
    inherited splitterDebug: TSplitter
      Top = 488
      Width = 833
    end
    inherited sgJobs: TStringGrid
      Width = 720
      Height = 473
    end
    inherited Panel2: TPanel
      Left = 722
      Height = 473
      ExplicitLeft = 1
      inherited groupStatusCheck: TGroupBox
        inherited cbDone: TCheckBox
          Checked = True
          State = cbChecked
        end
      end
      inherited btnExit: TButton
        Top = 432
      end
      inherited cbLog: TCheckBox
        Top = 415
      end
    end
    inherited panelDebug: TPanel
      Top = 491
      Width = 833
      ExplicitTop = -101
      ExplicitWidth = 112
      inherited memoDebug: TMemo
        Width = 719
      end
      inherited Panel1: TPanel
        Left = 720
        ExplicitLeft = -1
      end
    end
  end
end
