program ExampleCallback;

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
    procedure WeightCB(sender: TBrickletLoadCell; const weight: longint);
    procedure Execute;
  end;

const
  HOST = 'localhost';
  PORT = 4223;
  UID = 'XYZ'; { Change to your UID }

var
  e: TExample;

{ Callback procedure for weight callback (parameter has unit g) }
procedure TExample.WeightCB(sender: TBrickletLoadCell; const weight: longint);
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

  { Set period for weight callback to 1s (1000ms)
    Note: The weight callback is only called every second
          if the weight has changed since the last call! }
  lc.SetWeightCallbackPeriod(1000);

  { Register weight callback to procedure WeightCB }
  lc.OnWeight := {$ifdef FPC}@{$endif}WeightCB;

  WriteLn('Press key to exit');
  ReadLn;
  ipcon.Destroy; { Calls ipcon.Disconnect internally }
end;

begin
  e := TExample.Create;
  e.Execute;
  e.Destroy;
end.
