program ExampleSimple;

{$ifdef MSWINDOWS}{$apptype CONSOLE}{$endif}
{$ifdef FPC}{$mode OBJFPC}{$H+}{$endif}

uses
  SysUtils, IPConnection, BrickletLoadCell;

type
  TExample = class
  private
    ipcon: TIPConnection;
    lc: TBrickletLoadCell;
  public
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change XYZ to the UID of your Load Cell Bricklet }

var
  e: TExample;

procedure TExample.Execute;
var weight: longint;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  lc := TBrickletLoadCell.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get current weight }
  weight := lc.GetWeight;
  WriteLn(Format('Weight: %d g', [weight]));

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
