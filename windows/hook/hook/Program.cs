#region local hook
using System;
using System.Runtime.InteropServices;
class Program
{
    /*
      [DllImport(opengl32.dll)]
     * d3d9.dll d3d11.dll
     Direct3D的核心功能集中在IDirect3DDevice9的接口中，只要能hook其中的EndScence(), DrawPrimitive()或DrawIndexedPrimitive()就能感知绘图操作，进而实现我们想要的功能！
         */
    [DllImport("user32.dll")]
    static extern bool MessageBeep(uint uType);
    static bool MessageBeepHook(uint uType)
    {
        // Change the message beep to always be "Asterisk" (0x40)
        // see https://msdn.microsoft.com/en-us/library/windows/desktop/ms680356(v=vs.85).aspx
        Console.Write("NoBeep!");
        return false;
    }
    [UnmanagedFunctionPointer(CallingConvention.StdCall, SetLastError = true)]
    [return: MarshalAs(UnmanagedType.Bool)]
    delegate bool MessageBeepDelegate(uint uType);
    static void Main(string[] args)
    {
        Console.WriteLine("Calling MessageBeep with no hook:\n....\n");
        System.Threading.Thread.Sleep(5000);
        MessageBeep(0x40);
        //Console.Write("\nPress <enter> to call MessageBeep while hooked by MessageBeepHook:");
        //Console.ReadLine();

        // Create the local hook using our MessageBeepDelegate and MessageBeepHook handler
        using (var hook = EasyHook.LocalHook.Create(
                EasyHook.LocalHook.GetProcAddress("user32.dll", "MessageBeep"),
                new MessageBeepDelegate(MessageBeepHook),
                null))
        {

            // Only hook this thread (threadId == 0 == GetCurrentThreadId)
            hook.ThreadACL.SetInclusiveACL(new int[] { 0 });
            Console.WriteLine("Calling MessageBeep with hook:\n...\n");
            System.Threading.Thread.Sleep(5000);
            MessageBeep(0x40);
            Console.WriteLine("uninstall hook and\n");
            hook.ThreadACL.SetExclusiveACL(new int[] { 0 });
            Console.Write("\nPress <enter> t exit.");
            Console.ReadLine();
        }

    }
}
#endregion