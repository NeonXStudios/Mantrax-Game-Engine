using System.Runtime.CompilerServices;
using System;
using System.ComponentModel;

namespace MantraxGameCore
{
    public class Entity
    {
        public IntPtr p_pointer;

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

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetEntityName(IntPtr entityPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetEntityName(IntPtr entityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern void SetEntityTag(IntPtr entityPtr, string name);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private static extern string GetEntityTag(IntPtr entityPtr);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component AddComponent(Type type);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern Component GetComponent(Type type);

        public T AddComponent<T>() where T : Component
        {
            return (T)AddComponent(typeof(T));
        }

        public T GetComponent<T>() where T : Component
        {
            return (T)GetComponent(typeof(T));
        }
    }
}
