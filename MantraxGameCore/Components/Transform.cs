using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace MantraxGameCore
{
    public class Transform : Component
    {
        public Entity entity;

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern static Vector3 GetPosition(IntPtr transformObj);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetPosition(IntPtr transformObj, Vector3 pos);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Vector3 GetScale(IntPtr entityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetScale(Vector3 scale);


        public Vector3 Position
        {
            get
            {
                if (entity.p_pointer == IntPtr.Zero)
                {
                    Console.WriteLine("Error: entity pointer is null.");
                    return null;
                }

                return GetPosition(entity.p_pointer);
            }

            set
            {
                if (entity.p_pointer == IntPtr.Zero)
                {
                    Console.WriteLine("Error: entity pointer is null. Cannot set position.");
                    return;
                }

                Console.WriteLine(entity.p_pointer);

                SetPosition(entity.p_pointer, value);
            }
        }
    }
}
