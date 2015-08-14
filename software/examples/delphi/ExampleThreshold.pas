program ExampleThreshold;

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
    procedure WeightReachedCB(sender: TBrickletLoadCell; const weight: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for weight greater than 2 kg (parameter has unit g) }
procedure TExample.WeightReachedCB(sender: TBrickletLoadCell; const weight: longint);
begin
  WriteLn(Format('Weight: %f kg', [weight/1000.0]));
end;

procedure TExample.Execute;
begin
  { Create IP connection }
  ipcon := TIPConnection.Create;

  { Create device object }
  lc := TBrickletLoadCell.Create(UID, ipcon);

  { Connect to brickd }
  ipcon.Connect(HOST, PORT);
  { Don't use device before ipcon is connected }

  { Get threshold callbacks with a debounce time of 1 second (1000ms) }
  lc.SetDebouncePeriod(1000);

  { Register threshold reached callback to procedure WeightReachedCB }
  lc.OnWeightReached := {$ifdef FPC}@{$endif}WeightReachedCB;

  { Configure threshold for "greater than 2 kg" (unit is g) }
  lc.SetWeightCallbackThreshold('>', 2*1000, 0);

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
