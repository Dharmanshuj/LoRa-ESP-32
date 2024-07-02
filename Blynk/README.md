# Control LED using Blynk

Install the necessary librabries. Open Arduino IDE, go to (**Sketch -> Include Library -> Manage Libraries**). Search for **Blynk by Volodymyr Shymanskyy**
and install the latest version. Also install **BlynkNcpDriver by Volodymyr Shymanskyy**.

You can also take references from youtube like - "https://www.youtube.com/watch?v=W1xG_XJb0FU&t=132s".

Open your Web browser and search for **Blynk Iot**. Create an account and sign in.

On your DashBoard under Developer Zone, Select **New Template**. Enter name of template link **LED on and off**, Hardware :- **ESP32**,
Connection Type :- **Wifi**. Click Done.

Go to Datastreams section and select **New Datastream** -> **Virtual Pin**, Create.

Go to Web Dashboard section and drag drop the widget. Click **Save**.

Under Devices, select **New Device** -> **From Template** ->**Choose LED on and off** -> **Create**.

Download Blynk Iot for mobile. Set up the device in your mobile.

Now connect the LoRa ESP device to your laptop. Upload the **Sender code** into new sketch in Arduino IDE.
Connect another LoRa ESP device to your laptop and upload the **Receiver code** into new sketch in Arduino IDE.
