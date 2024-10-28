using System;
using System.Runtime.CompilerServices;
using System.Runtime.Remoting.Messaging;

namespace MantraxGameCore
{
    public class ModelComponent
    {
        private IntPtr _nativePointer;

        public string ModelPath { get; private set; }

        private IntPtr _textureNormal;
        private IntPtr _textureSampler;

        public ModelComponent(IntPtr nativePointer)
        {
            _nativePointer = nativePointer;
        }

        public void SetModel(string pathLoad)
        {
            ModelPath = pathLoad;

            SetModelNative(pathLoad);
        }

        //public Shader GetMeshShader()
        //{
        //    IntPtr shaderPtr = GetMeshShaderNative();
        //    return new Shader(shaderPtr);
        //}

        // Llamadas internas
        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern void SetModelNative(string pathLoad);

        [MethodImpl(MethodImplOptions.InternalCall)]
        private extern IntPtr GetMeshShaderNative();
    }
}