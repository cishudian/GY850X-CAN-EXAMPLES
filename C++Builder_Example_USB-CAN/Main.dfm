object Form1: TForm1
  Left = 298
  Top = 166
  Width = 596
  Height = 481
  Caption = 'VCI_CAN C++Builder  sourcedemo'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  Menu = MainMenu1
  OldCreateOrder = False
  OnClose = FormClose
  PixelsPerInch = 96
  TextHeight = 13
  object Button5: TButton
    Left = 8
    Top = 24
    Width = 75
    Height = 25
    Caption = 'Open Device'
    TabOrder = 0
    OnClick = Button5Click
  end
  object Button6: TButton
    Left = 96
    Top = 24
    Width = 75
    Height = 25
    Caption = 'InitCAN &'
    TabOrder = 1
    OnClick = Button6Click
  end
  object Button7: TButton
    Left = 8
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Transmit'
    TabOrder = 2
    OnClick = Button7Click
  end
  object Button8: TButton
    Left = 96
    Top = 56
    Width = 75
    Height = 25
    Caption = 'Receive'
    TabOrder = 3
    OnClick = Button8Click
  end
  object ListBox1: TListBox
    Left = 0
    Top = 96
    Width = 561
    Height = 320
    Color = clScrollBar
    ItemHeight = 13
    TabOrder = 4
  end
  object Timer1: TTimer
    Enabled = False
    OnTimer = Timer1Timer
    Left = 176
    Top = 16
  end
  object MainMenu1: TMainMenu
    Left = 392
    Top = 65528
    object F1: TMenuItem
      Caption = #25991#20214'(&F)'
      object N1: TMenuItem
        Caption = #20445#23384#25968#25454'(&S)'
      end
      object N2: TMenuItem
        Caption = #27983#35272#21382#21490#25968#25454'(&B)'
      end
      object N3: TMenuItem
        Caption = '-'
      end
      object X1: TMenuItem
        Caption = #36864#20986'(&X)'
      end
    end
    object N4: TMenuItem
      Caption = #35774#32622
      object N5: TMenuItem
        Caption = #23494#23553#21442#25968#35774#32622
      end
      object N6: TMenuItem
        Caption = '-'
      end
      object N7: TMenuItem
        Caption = #21160#20316#21442#25968#35774#32622
      end
    end
    object N8: TMenuItem
      Caption = #20851#20110
      object N9: TMenuItem
        Caption = #24110#21161
      end
      object N10: TMenuItem
        Caption = '-'
      end
      object N11: TMenuItem
        Caption = #20851#20110#25105#20204
      end
    end
  end
end
