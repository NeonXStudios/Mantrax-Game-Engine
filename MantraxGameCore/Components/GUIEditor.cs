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
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void Begin();

        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static void End();
    }
}
