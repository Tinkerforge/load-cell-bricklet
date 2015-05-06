Imports Tinkerforge

Module ExampleCallback
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback function for weight callback (parameter has unit g)
    Sub WeightCB(ByVal sender As BrickletLoadCell, ByVal weight As Integer)
        System.Console.WriteLine("Weight: " + (weight/1000.0).ToString() + " kg")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lc As New BrickletLoadCell(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Set Period for weight callback to 1s (1000ms)
        ' Note: The weight callback is only called every second if the
        '       weight has changed since the last call!
        lc.SetWeightCallbackPeriod(1000)

        ' Register weight callback to function WeightCB
        AddHandler lc.Weight, AddressOf WeightCB

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
