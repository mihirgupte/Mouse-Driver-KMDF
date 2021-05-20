# Mouse Driver KMDF
A mouse driver which connects to your mouse on the kernel level and displays the input of your button.

This driver was made by following a YouTube tutorial (mentioned in [Acknowledgements](#acknowledgements)), but there were a lot of compilation errors, vm errors, and driver loading errors that I encountered. I have made a list of all the errors I encountered and how to fix them below!

**NOTE: USE ONLY WIN 8.1 AS IT HAS ONLY BEEN TESTED ON IT**

## Visual Studio Errors

In Visual Studio, install [Windows Development Kit](https://docs.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk), Windows 10 SDK (I'm working on Windows 10 and SDK version 10.0.19041.0).

Make a new project using the **KMDF (Empty)** Framework, and add a ``Source.c`` file and paste the code I have given in the repo.

**Make sure you are building the solution for x64**

- **error MSB8040: Spectre-mitigated libraries are required for this project** - Go to properties of your project file --> C/C++ --> Code Generation --> you'll see an option at the bottom which will let you Disable Spectre-Mitigation
- **inf2cat signability test failed** - Go to properties --> Inf2Cat --> Use Local Time --> change to Yes (/useLocalTime)

## VM Settings and Requirements
**VM Used**
- Windows 8.1 (64-bit)
- [Virtual Box only](https://www.virtualbox.org/)
![Capture2 width=300](https://user-images.githubusercontent.com/72495317/119021340-f1674800-b9bc-11eb-9094-43ead50569a3.jpg)


**Disable Integrity Checks** -  
- Click the “Start” button.
- Type “command“.
- Right-click on “Command Prompt” and select “Run As Administrator“.
- Do one of the following:
- To disable device driver signing, type ``BCDEDIT /set nointegritychecks ON`` then press “Enter“
- To enable device driver signing, type ``BCDEDIT /set nointegritychecks OFF`` then press “Enter“

**Disable Windows Defender** -
- Open Windows Defender --> Real Time Protection --> Disabled/Paused

## OSR Load Errors/ Driver Load Errors
- **Driver Handle Error** - In Visual Studio, go to properties --> Driver Settings --> Driver Model. Here set `` KMDF version Major = 1`` and ``KMDF version Minor = 9``. Now build your solution.
### If you get "This driver has been blocked from loading"
Check if you have compiled the driver using **x64** in Visual Studio Code
For Driver signature enforcement issue --> ran ``bcdedit.exe /set nointegritychecks on``
Signing issue --> ran ``bcdedit /set testsigning on``

To disable Driver signing you can also go to Settings --> Update & Security --> Advanced Startup and click restart
Inside advanced startup, click Troublshoot --> Advanced Options --> Startup Settings --> Restart
Then Press F7 to disbale Driver Signature Enforcement
**Note-> This will disable Driver Signing till you restart the computer**
[Another resource to fix this problem](https://www.wintips.org/how-to-fix-windows-cannot-verify-the-digital-signature-for-this-file-error-in-windows-8-7-vista/)

## Debug View
- Click on Windows Icon and Gear Icon in the nav bar.
- Enable Kernel Verbose Log Option to see the output.
![Capture](https://user-images.githubusercontent.com/72495317/119021167-b5cc7e00-b9bc-11eb-89d8-47dd7af10a07.PNG)

## Software Requirements 
- [OSR Loader](http://www.osronline.com/OsrDown.cfm/osrloaderv30.zip%5Ename=osrloaderv30.zip&id=157)
- [Win Obj](https://docs.microsoft.com/en-us/sysinternals/downloads/winobj)
- [Debug View](https://docs.microsoft.com/en-us/sysinternals/downloads/debugview)

## Acknowledgements
I have follwed this youtube video about [Mouse Filter Driver](https://www.youtube.com/watch?v=PppMoZvW6L4&list=PLZ4EgN7ZCzJyUT-FmgHsW4e9BxfP-VMuo&index=9) by Programming LoL.
