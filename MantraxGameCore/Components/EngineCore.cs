using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MantraxGameCore
{
    public class EngineCore
    {
        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern IntPtr MakeEntity();
    
        public static Entity NewObject()
        {
            Entity entity = new Entity();
            IntPtr p_object = MakeEntity();
            entity.p_pointer = p_object;

            return entity;
        }
    }
}
