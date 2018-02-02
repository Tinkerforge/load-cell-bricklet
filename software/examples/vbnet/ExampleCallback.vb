Imports System
Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Load Cell Bricklet

    ' Callback subroutine for weight callback
    Sub WeightCB(ByVal sender As BrickletLoadCell, ByVal weight As Integer)
        Console.WriteLine("Weight: " + weight.ToString() + " g")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lc As New BrickletLoadCell(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Register weight callback to subroutine WeightCB
        AddHandler lc.WeightCallback, AddressOf WeightCB

        ' Set period for weight callback to 1s (1000ms)
        ' Note: The weight callback is only called every second
        '       if the weight has changed since the last call!
        lc.SetWeightCallbackPeriod(1000)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
