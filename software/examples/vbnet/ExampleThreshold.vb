Imports System
Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change XYZ to the UID of your Load Cell Bricklet

    ' Callback subroutine for weight reached callback (parameter has unit g)
    Sub WeightReachedCB(ByVal sender As BrickletLoadCell, ByVal weight As Integer)
        Console.WriteLine("Weight: " + weight.ToString() + " g")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lc As New BrickletLoadCell(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 1 second (1000ms)
        lc.SetDebouncePeriod(1000)

        ' Register weight reached callback to subroutine WeightReachedCB
        AddHandler lc.WeightReached, AddressOf WeightReachedCB

        ' Configure threshold for weight "greater than 200 g" (unit is g)
        lc.SetWeightCallbackThreshold(">"C, 200, 0)

        Console.WriteLine("Press key to exit")
        Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
