using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MantraxGameCore
{
    public class Time
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        public extern static float DeltaTime();

        public static float Delta
        {
            get
            {
                return DeltaTime();
            }
        }
    }
}
