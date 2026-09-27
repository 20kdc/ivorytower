# W32Cross

W32Cross is basically intended to be 'like OSXCross, but targetting Windows'.

While it is used in ivorytower for now, it may get split given popular request.

W32Cross _'doesn't exist yet,'_ but here's the key points on which this all stands:

* From <https://learn.microsoft.com/en-us/windows/apps/windows-sdk/downloads>: `Windows SDK for Windows 10 2004 (10.0.19041.0)` ISO aka <https://go.microsoft.com/fwlink/?linkid=2312004÷
	* Has the useful property of _not being a Visual Studio SDK,_ and thus not subject to 'profit-cap licensing'.
* STL: Can be gotten in code form from <https://github.com/microsoft/STL>.
	* Header files have the useful property of being an STL's header files.

From this we'd basically get the same situation as OSXCross.

Of course, due to how messy this all is, we also kind of need to _run it_ the same way as OSXCross; i.e. with a whole lotta extremely carefully planned shell script.
