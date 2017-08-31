
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
namespace OpenGLHook
{

    /// <summary>
    /// EasyHook will look for a class implementing <see cref="EasyHook.IEntryPoint"/> during injection. This
    /// becomes the entry point within the target process after injection is complete.
    /// </summary>
    public class InjectionEntryPoint : EasyHook.IEntryPoint
    {
        public string GetLastStr(string str, int num)
        {
            int count = 0;
            if (str.Length > num)
            {
                count = str.Length - num;
                str = str.Substring(count, num);
            }
            return str;
        }
        /* #region SetPixelFormat Hook
         [StructLayout(LayoutKind.Sequential)]
         public struct PIXELFORMATDESCRIPTOR
         {
             public ushort nSize;
             public ushort nVersion;
             public UInt32 dwFlags;
             public byte iPixelType;
             public byte cColorBits;
             public byte cRedBits;
             public byte cRedShift;
             public byte cGreenBits;
             public byte cGreenShift;
             public byte cBlueBits;
             public byte cBlueShift;
             public byte cAlphaBits;
             public byte cAlphaShift;
             public byte cAccumBits;
             public byte cAccumRedBits;
             public byte cAccumGreenBits;
             public byte cAccumBlueBits;
             public byte cAccumAlphaBits;
             public byte cDepthBits;
             public byte cStencilBits;
             public byte cAuxBuffers;
             public byte iLayerType;
             public byte bReserved;
             public UInt32 dwLayerMask;
             public UInt32 dwVisibleMask;
             public UInt32 dwDamageMask;
         }
         [UnmanagedFunctionPointer(CallingConvention.StdCall,
                     CharSet = CharSet.Unicode,
                     SetLastError = true)]
         delegate bool SetPixelFormat_Delegate(
         int hdc,
         int iPixelFormat,
         ref PIXELFORMATDESCRIPTOR ppfd
 );
         /// <summary>
         /// Using P/Invoke to call original method.
         /// https://msdn.microsoft.com/en-us/library/windows/desktop/aa363858(v=vs.85).aspx
         /// </summary>
         /// <param name="filename"></param>
         /// <param name="desiredAccess"></param>
         /// <param name="shareMode"></param>
         /// <param name="securityAttributes"></param>
         /// <param name="creationDisposition"></param>
         /// <param name="flagsAndAttributes"></param>
         /// <param name="templateFile"></param>
         /// <returns></returns>
         [DllImport("Gdi32.dll",
             CharSet = CharSet.Unicode,
             SetLastError = true,
             CallingConvention = CallingConvention.StdCall)]
         static extern bool SetPixelFormat(
             int hdc,
             int iPixelFormat,
             ref PIXELFORMATDESCRIPTOR ppfd
 );
         bool SetPixelFormat_Hook(
             int hdc,
             int iPixelFormat,
             ref PIXELFORMATDESCRIPTOR ppfd
 )
         {
             MessageBox.Show("hello");
             //#define PFD_GENERIC_FORMAT 0x00000040 
             //#define PFD_GENERIC_ACCELERATED 0x00001000                            PFD_GENERIC_FORMAT =1 PFD_GENERIC_ACCELERATED=0
             //switch the pixel format to software acceration
             string param = " ";
             try
             {
                 if (((ppfd.dwFlags & 0x00001000) == 0) && (ppfd.dwFlags & 0x00000040) != 0)
                 {
                     param = "This is pure softwate acceleration!Terminate the procedure\n";
                 }
                 //ppfd.dwFlags = (uint)((ppfd.dwFlags | 0x00000040) & ~0x00001000);                             //set the pixel to software mode
                 else
                 {
                     param = "We will terminate the procedure\n";
                 }
                 lock (this._messageQueue)
                 {
                     if (this._messageQueue.Count < 1000)
                     {

                         string msg = "the program's SetPixelFormat() has changed\n" + param;
                         // Add message to send to FileMonitor
                         this._messageQueue.Enqueue(
                             string.Format("[ProcessId:{0}ThreadId:{1}]:   ({2}) ",
                             EasyHook.RemoteHooking.GetCurrentProcessId(), EasyHook.RemoteHooking.GetCurrentThreadId()
                             , msg));
                     }
                 }
             }
             catch
             {

             }

             if (((ppfd.dwFlags & 0x00001000) == 0) && (ppfd.dwFlags & 0x00000040) != 0)
             {
                 // now call the original API...
                 return SetPixelFormat(hdc, iPixelFormat, ref ppfd);
             }
             else
             {
                 return false;
             }
         }
         #endregion*/
        #region LoadLibrary Hook
        [UnmanagedFunctionPointer(CallingConvention.StdCall,
                    CharSet = CharSet.Ansi,
                    SetLastError = true)]
        delegate IntPtr LoadLibrary_Delegate(
        string lpFileName);
        [DllImport("Kernel32.dll",
            CharSet = CharSet.Ansi,
            SetLastError = true,
            CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr LoadLibraryA(
        string lpFileName);
        IntPtr LoadLibrary_Hook(
        string lpFileName)
        {
            MessageBox.Show(lpFileName);
            if (GetLastStr(lpFileName, 12).ToUpper() == "opengl32.dll".ToUpper())
            {
                MessageBox.Show(lpFileName);
                System.Environment.Exit(0);
                return IntPtr.Zero; ;
            }
            else if (GetLastStr(lpFileName, 8).ToUpper() == "d3d9.dll".ToUpper())
            {
                MessageBox.Show(lpFileName);
                System.Environment.Exit(0);
                return IntPtr.Zero;
            }
            else
            {
                return LoadLibraryA(lpFileName);
            }

        }
        #endregion
        #region LoadLibraryEx Hook
        [UnmanagedFunctionPointer(CallingConvention.StdCall,
                    CharSet = CharSet.Ansi,
                    SetLastError = true)]
        delegate IntPtr LoadLibraryEx_Delegate(
        string lpFileName,
        IntPtr  hFile,
        uint   dwFlags
);
        [DllImport("Kernel32.dll",
            CharSet = CharSet.Ansi,
            SetLastError = true,
            CallingConvention = CallingConvention.StdCall)]
        static extern IntPtr LoadLibraryExA(
        string lpFileName,
        IntPtr hFile,
        uint dwFlags
);
        IntPtr LoadLibraryEx_Hook(
        string lpFileName,
        IntPtr  hFile,
        uint   dwFlags
)
        {
            MessageBox.Show(lpFileName);
            if (GetLastStr(lpFileName, 12).ToUpper() == "opengl32.dll".ToUpper())
            {
                MessageBox.Show(lpFileName);
                System.Environment.Exit(0);
                return IntPtr.Zero; ;
            }
            else if (GetLastStr(lpFileName, 8).ToUpper() == "d3d9.dll".ToUpper())
            {
                MessageBox.Show(lpFileName);
                System.Environment.Exit(0);
                return IntPtr.Zero;
            }
            else
            {
                return LoadLibraryExA(lpFileName,hFile,dwFlags);
            }

        }
        #endregion
     
        ServerInterface _server = null;

        Queue<string> _messageQueue = new Queue<string>();
        public InjectionEntryPoint(
            EasyHook.RemoteHooking.IContext context,
            string channelName)
        {
            // Connect to server object using provided channel name
            _server = EasyHook.RemoteHooking.IpcConnectClient<ServerInterface>(channelName);

            // If Ping fails then the Run method will be not be called
            _server.Ping();
        }

        public void Run(
            EasyHook.RemoteHooking.IContext context,
            string channelName)
        {

            // Injection is now complete and the server interface is connected
            _server.IsInstalled(EasyHook.RemoteHooking.GetCurrentProcessId());
            // Install hooks
            try
            {
                // SetPixelFormat  https://msdn.microsoft.com/en-us/library/windows/desktop/dd369049(v=vs.85).aspx
               /* var SetPixelFormatHook = EasyHook.LocalHook.Create(
                    EasyHook.LocalHook.GetProcAddress("gdi32.dll", "SetPixelFormat"),
                    new SetPixelFormat_Delegate(SetPixelFormat_Hook),
                    this);*/
                var LoadLibraryHook = EasyHook.LocalHook.Create(
                       EasyHook.LocalHook.GetProcAddress("Kernel32.dll", "LoadLibraryA"),
                    new LoadLibrary_Delegate(LoadLibrary_Hook),
                    this);
                var LoadLibraryExHook = EasyHook.LocalHook.Create(
                      EasyHook.LocalHook.GetProcAddress("Kernel32.dll", "LoadLibraryExA"),
                   new LoadLibraryEx_Delegate(LoadLibraryEx_Hook),
                   this);

                //SetPixelFormatHook.ThreadACL.SetExclusiveACL(new Int32[] { 0 });
                LoadLibraryHook.ThreadACL.SetExclusiveACL(new Int32[] { 0 });
                LoadLibraryExHook.ThreadACL.SetExclusiveACL(new Int32[] { 0 });   // Activate hooks on all threads except the current thread

                _server.ReportMessage(" hooks installed");

                // Wake up the process (required if using RemoteHooking.CreateAndInject)
                EasyHook.RemoteHooking.WakeUpProcess();


                // Loop until FileMonitor closes (i.e. IPC fails)
                while (true)
                {
                    System.Threading.Thread.Sleep(500);

                    string[] queued = null;

                    lock (_messageQueue)
                    {
                        queued = _messageQueue.ToArray();
                        _messageQueue.Clear();
                    }

                    // Send newly monitored file accesses to FileMonitor
                    if (queued != null && queued.Length > 0)
                    {
                        _server.ReportMessages(queued);
                    }
                    else
                    {
                        _server.Ping();
                    }
                }
                // Remove hooks
                //SetPixelFormatHook.Dispose();
                LoadLibraryHook.Dispose();
                LoadLibraryExHook.Dispose();
            }
            catch (Exception e)
            {
                MessageBox.Show(e.ToString());
            }
            finally
            {
                // Finalise cleanup of hooks
                EasyHook.LocalHook.Release();
            }
        }


    }
}