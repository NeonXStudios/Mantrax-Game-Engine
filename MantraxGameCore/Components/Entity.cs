using System.Runtime.CompilerServices;
using System;

namespace MantraxGameCore
{
    public class Entity
    {
        public string Name
        {
            get => GetEntityName(p_pointer);
            set => SetEntityName(p_pointer, value);
        }

        public string Tag
        {
            get => GetEntityTag(p_pointer);
            set => SetEntityTag(p_pointer, value);
        }

        public IntPtr p_pointer;

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetEntityName(IntPtr entityPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetEntityName(IntPtr entityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetEntityTag(IntPtr entityPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetEntityTag(IntPtr entityPtr);
    }
}
