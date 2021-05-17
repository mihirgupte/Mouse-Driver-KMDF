# Mouse-Driver-KMDF
A mouse driver which connects to your mouse on the kernel level and displays the input of your button.

**USE ONLY WIN 8.1 VM AS IT HAS ONLY BEEN TESTED ON IT**

## If you get "This driver has been blocked from loading"
Check if you have compiled the driver using **x64** in Visual Studio Code
For Driver signature enforcement issue --> ran bcdedit.exe /set nointegritychecks on
Signing issue --> ran bcdedit /set testsigning on

To disable Driver signing you can also go to Settings --> Update & Security --> Advanced Startup and click restart
Inside advanced startup, click Troublshoot --> Advanced Options --> Startup Settings --> Restart
Then Press F7 to disbale Driver Signature Enforcement
#### Note-> This will disable Driver Signing till you restart the computer
**Another link to fix this**
[Another resource to fix this problem](https://www.wintips.org/how-to-fix-windows-cannot-verify-the-digital-signature-for-this-file-error-in-windows-8-7-vista/)


