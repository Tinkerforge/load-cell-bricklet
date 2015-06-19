Imports Tinkerforge

Module ExampleThreshold
    Const HOST As String = "localhost"
    Const PORT As Integer = 4223
    Const UID As String = "XYZ" ' Change to your UID

    ' Callback for weight greater than 200 g
    Sub ReachedCB(ByVal sender As BrickletLoadCell, ByVal weight As Integer)
        System.Console.WriteLine("Weight: " + (weight/1000.0).ToString() + " kg")
    End Sub

    Sub Main()
        Dim ipcon As New IPConnection() ' Create IP connection
        Dim lc As New BrickletLoadCell(UID, ipcon) ' Create device object

        ipcon.Connect(HOST, PORT) ' Connect to brickd
        ' Don't use device before ipcon is connected

        ' Get threshold callbacks with a debounce time of 10 seconds (10000ms)
        lc.SetDebouncePeriod(10000)

        ' Register threshold reached callback to function ReachedCB
        AddHandler lc.WeightReached, AddressOf ReachedCB

        ' Configure threshold for "greater than 200 g"
        lc.SetWeightCallbackThreshold(">"C, 200, 0)

        System.Console.WriteLine("Press key to exit")
        System.Console.ReadLine()
        ipcon.Disconnect()
    End Sub
End Module
