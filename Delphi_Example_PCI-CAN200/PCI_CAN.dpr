program PCI_CAN;

uses
  Forms,
  MainProgram in 'MainProgram.pas' {Mainform};

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TMainform, Mainform);
  Application.Run;
end.
