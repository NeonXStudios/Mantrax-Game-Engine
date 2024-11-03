using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.CompilerServices;

namespace MantraxGameCore
{
    public class GUIEditor
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void Begin(string WindowsName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void End();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static string InputField(string _Name, string _Value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool Button(string _Name);
    }
}